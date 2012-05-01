// *********************************************************
// Material Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#include <vector>

#include "Material.h"

#include "RayTracer.h"
#include "Controller.h"
#include "Object.h"
#include "Ray.h"

using namespace std;

Vec3Df Material::genColor (const Vec3Df & camPos, const Vertex & closestIntersection,
                           const std::vector<Light> & lights, Brdf::Type type) const {
    float ambientOcclusionContribution = (type & Brdf::Ambient)?
        controller->getRayTracer()->getAmbientOcclusion(closestIntersection):
        0.f;

    const Brdf brdf(lights,
                    noise(closestIntersection)*color,
                    controller->getRayTracer()->getBackgroundColor(),
                    diffuse,
                    specular,
                    ambientOcclusionContribution,
                    1.5);

    if(glossyRatio == 0)
        return brdf(closestIntersection.getPos(), closestIntersection.getNormal(), camPos, type);

    /* Glossy Material */
    const Vec3Df spec = brdf(closestIntersection.getPos(), closestIntersection.getNormal(), camPos,
                             Brdf::Type(Brdf::Specular&type));
    const Vec3Df color = glossyRatio<=1?
        brdf(closestIntersection.getPos(), closestIntersection.getNormal(), camPos,
             Brdf::Type((Brdf::Ambient|Brdf::Diffuse)&type)):
        Vec3Df();
    const Vec3Df & pos = closestIntersection.getPos();
    Vec3Df dir = (camPos-pos).reflect(closestIntersection.getNormal());
    dir.normalize();

    const Vec3Df reflectedColor = controller->getRayTracer()->getColor(dir, pos, false);

    return spec + Vec3Df::interpolate(color, reflectedColor, glossyRatio);
}

Vec3Df Glass::genColor (const Vec3Df & camPos, const Vertex & closestIntersection,
                        const std::vector<Light> &, Brdf::Type) const {
    float size = o->getBoundingBox().getRadius();
    const Vec3Df & pos = closestIntersection.getPos();
    Vec3Df dir = camPos-pos;

    dir = dir.refract(1, closestIntersection.getNormal(), coeff);
    dir.normalize();

    //Works only for convex object
    Ray ray(pos-o->getTrans()+2*size*dir, -dir);
    if (!o->getKDtree().intersect(ray)) {
        return controller->getRayTracer()->getColor(pos+size*dir, pos-camPos);
    }

    const Vertex i = ray.getIntersection();
    dir = (-dir).refract(coeff,-i.getNormal(), 1);

    return controller->getRayTracer()->getColor(dir, i.getPos()+o->getTrans(), false);
}
