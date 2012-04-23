// *********************************************************
// Scene Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#include "Scene.h"
#include "Noise.h"
#include "SkyBoxMaterial.h"

using namespace std;

static Scene * instance = NULL;

Scene * Scene::getInstance () {
    if (instance == NULL)
        instance = new Scene ();
    return instance;
}

void Scene::destroyInstance () {
    if (instance != NULL) {
        delete instance;
        instance = NULL;
    }
}

Scene::Scene () {
    buildDefaultScene ();
    updateBoundingBox ();
}

Scene::~Scene () {
}

void Scene::updateBoundingBox () {
    if (objects.empty ())
        bbox = BoundingBox ();
    else {
        bbox = objects[0].getBoundingBox ();
        for (unsigned int i = 1; i < objects.size (); i++)
            bbox.extendTo (objects[i].getBoundingBox ());
    }
}

// Changer ce code pour creer des scenes originales
void Scene::buildDefaultScene () {
    Mesh groundMesh;
    groundMesh.loadOFF ("models/ground.off");
    Material *groundMat = new Material(1.f, 0.f, Vec3Df (.2f, 0.6f, .2f),
                                       [](const Vertex & v){
                                           static const Perlin perlin(0.5f, 4, 10);
                                           return perlin(v.getPos());
                                       });
    Object ground (groundMesh, groundMat);
    objects.push_back (ground);

    Mesh mirrorMesh;
    mirrorMesh.loadOFF ("models/mirror.off");
    mirrorMesh.scale(2.0);
    Object mirror(mirrorMesh, new Mirror());
    mirror.setTrans(Vec3Df(-1.95251, 0, 3));
    objects.push_back (mirror);

    Mesh mirror2Mesh;
    mirror2Mesh.loadOFF ("models/mirror.off");
    mirror2Mesh.rotate(Vec3Df(0, 0, 1), M_PI);
    Object mirror2(mirror2Mesh, new Mirror());
    mirror2.setTrans(Vec3Df(1.95251, 0, 1.5));
    objects.push_back (mirror2);

    Mesh ramMesh;
    ramMesh.loadOFF ("models/ram.off");
    Material *ramMat = new Material(1.f, 1.f, Vec3Df (1.f, .6f, .2f));
    Object ram (ramMesh, ramMat);
    ram.setTrans (Vec3Df (-1.f, -1.0f, 0.f));
    objects.push_back (ram);

    Mesh rhinoMesh;
    rhinoMesh.loadOFF ("models/rhino.off");
    Material *rhinoMat = new Material(1.0f, 0.2f, Vec3Df (0.6f, 0.6f, 0.7f),
                                      [](const Vertex & v) -> float{
                                          static const Perlin perlin(0.5f, 4, 5);
                                          return sqrt(fabs(sin(2 * M_PI * perlin(v.getPos()))));
                                      });
    Object rhino (rhinoMesh, rhinoMat);
    rhino.setTrans (Vec3Df (1.f, 0.f, 0.4f));
    objects.push_back (rhino);

    Mesh gargMesh;
    gargMesh.loadOFF ("models/gargoyle.off");
    Material *gargMat = new Material(0.7f, 0.4f, Vec3Df (0.5f, 0.8f, 0.5f));
    Object garg (gargMesh, gargMat);
    garg.setTrans (Vec3Df (-1.f, 1.0f, 0.f));
    objects.push_back (garg);

    Mesh skyBoxMesh;
    skyBoxMesh.loadOFF("models/skybox.off");
    Material *skyBoxMaterial = new SkyBoxMaterial("textures/skybox.ppm");
    Object skyBox(skyBoxMesh, skyBoxMaterial);
    objects.push_back(skyBox);

    Light l (Vec3Df (.5f, -3.f, 5.5f), 0.5, Vec3Df(0, 0, 1), Vec3Df (1.f, 1.f, 1.f), 1.0f);
    lights.push_back (l);
    Light l1 (Vec3Df (.5f, 3.f, 5.5f), 0.5, Vec3Df(0, 0, 1), Vec3Df (1.0f, 0.0f, 0.0f), 1.0f);
    lights.push_back (l1);
}
