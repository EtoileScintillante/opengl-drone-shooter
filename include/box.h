/**
 * box.h
 * 
 * code from https://www.scratchapixel.com/code.php?id=10&origin=/lessons/3d-basic-rendering/ray-tracing-rendering-simple-shapes&src=1.
 * Modified by: EtoileScintillante
 */

#ifndef __BOX_H__
#define __BOX_H__

#include "geometry.h"
#include "ray.h"

class AABBox 
{ 
public: 
    /**
     * @brief Construct a new AABBox object.
     * 
     * @param vmim mimimum bound of box.
     * @param vmax maximum bound of box.
     */
    AABBox(const Vec3<float> &vmin, const Vec3<float> &vmax) { bounds[0] = vmin, bounds[1] = vmax; }

    /**
     * @brief checks whether a ray hits the bounding box.
     * 
     * @param r ray.
     * @param t constant. Ray (basically a line) is defined by O + Dt. 
     * Where O is the origin, D the direction and t could be any value.
     * @return true if hit.
     * @return false if no hit.
     */
    bool intersect(const Ray &r, float &t) const
    {
        float tmin, tmax, tymin, tymax, tzmin, tzmax;

        tmin = (bounds[r.sign[0]].x - r.orig.x) * r.invdir.x;
        tmax = (bounds[1 - r.sign[0]].x - r.orig.x) * r.invdir.x;
        tymin = (bounds[r.sign[1]].y - r.orig.y) * r.invdir.y;
        tymax = (bounds[1 - r.sign[1]].y - r.orig.y) * r.invdir.y;

        if ((tmin > tymax) || (tymin > tmax))
            return false;

        if (tymin > tmin)
            tmin = tymin;
        if (tymax < tmax)
            tmax = tymax;

        tzmin = (bounds[r.sign[2]].z - r.orig.z) * r.invdir.z;
        tzmax = (bounds[1 - r.sign[2]].z - r.orig.z) * r.invdir.z;

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;

        if (tzmin > tmin)
            tmin = tzmin;
        if (tzmax < tmax)
            tmax = tzmax;

        t = tmin;

        if (t < 0)
        {
            t = tmax;
            if (t < 0)
                return false;
        }

        return true;
    };
    Vec3<float> bounds[2]; 
}; 

#endif /*__BOX__*/