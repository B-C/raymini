// *********************************************************
// Object Class
// Author : Tamy Boubekeur (boubek@gmail.com).
// Copyright (C) 2010 Tamy Boubekeur.
// All rights reserved.
// *********************************************************

#pragma once

#include <iostream>
#include <vector>

#include "Mesh.h"
#include "Material.h"
#include "BoundingBox.h"
#include "KDtree.h"
#include "Brdf.h"
#include "Light.h"
#include "Texture.h"

class Ray;

class Object {
public:
    inline Object (std::string name = "No name") : texture(nullptr), tree(nullptr), enabled(false), name(name) {}

    Object (const Mesh & mesh, const Material * mat, std::string name="No name",
            const Vec3Df &trans=Vec3Df(), const Vec3Df &mobile=Vec3Df()) :
        mesh (mesh), texture(nullptr), mat (mat), trans(trans), origTrans(trans),
        tree(nullptr), mobile(mobile), enabled(true), name(name) {
        updateBoundingBox ();
        tree = new KDtree(*this);
    }

    virtual ~Object () {
        delete tree;
    }

    inline const Vec3Df & getTrans () const { return trans;}
    inline void setTrans (const Vec3Df & t) {
        trans = t;
        origTrans = trans;
    }

    inline void move(unsigned nbImages) { trans += mobile/nbImages ; }
    inline void reset() { trans = origTrans; }

    inline bool isMobile() const {return mobile!=Vec3Df(); }
    inline void setMobile(const Vec3Df & mobile) { this->mobile = mobile; }
    inline Vec3Df getMobile() const {return mobile;}

    inline const Mesh & getMesh () const { return mesh; }
    inline Mesh & getMesh () { return mesh; }

    inline const Material & getMaterial () const { return *mat; }
    inline void setMaterial(const Material *material) {mat = material;}

    inline const KDtree & getKDtree () const { return *tree; }

    inline void setEnabled(bool e) { enabled = e; }
    inline bool isEnabled() const { return enabled; }

    inline const std::string &getName() const {return name;}

    inline const BoundingBox & getBoundingBox () const { return bbox; }
    void updateBoundingBox () { bbox = computeBoundingBox(mesh); }
    static BoundingBox computeBoundingBox(const Mesh & mesh);

    /** Be sure to configure u,v for each vertex of the mesh */
    inline void setTexture(Texture *t) {texture = t;}
    inline const Texture *getTexture() const {return texture;}

    /**
     * Return the texture color of a point intersected by the ray.
     * Be sure to configure u,v for each vertex of the mesh
     * Return Vec3Df() if texture equals null pointer, or if the ray didn't intersect
     **/
    Vec3Df getTextureColor(const Ray *intersectingRay) const;

protected:
    Mesh mesh;
    const Texture *texture;
    const Material * mat;

private:
    BoundingBox bbox;
    Vec3Df trans;
    Vec3Df origTrans;
    KDtree *tree;
    Vec3Df mobile;
    bool enabled;
    std::string name;
};

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
