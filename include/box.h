/**
 * box.h
 * 
 * Created by: EtoileScintillante
 * Resource: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
 */

#ifndef __BOX_H__
#define __BOX_H__

#include "ray.h"

class AABBox 
{ 
public: 
    glm::vec3 bounds[2]; 

    /**
     * @brief Construct a new AABBox object.
     * 
     * @param vmim mimimum bound of box.
     * @param vmax maximum bound of box.
     */
    AABBox(glm::vec3 vmin, glm::vec3 vmax)
    {
        this->bounds[0] = vmin;
        this->bounds[1] = vmax;

        if (!isValid())
        {
            fix();
        }
    };

    /// @brief checks whether the min values are acually smaller than max values.
    /// @return true if valid, else false.
    bool isValid()
    {
        return bounds[0].x < bounds[1].x && bounds[0].y < bounds[1].y && bounds[0].z < bounds[1].z;
    };

    /// @brief swaps values when necessary (when they are not valid).
    void fix()
    {
        if (bounds[0].x > bounds[1].x)
        {
            float temp = bounds[0].x;
            bounds[0].x = bounds[1].x;
            bounds[1].x = temp;
        }
        if (bounds[0].y > bounds[1].y)
        {
            float temp = bounds[0].y;
            bounds[0].y = bounds[1].y;
            bounds[1].y = temp;
        }
        if (bounds[0].z > bounds[1].z)
        {
            float temp = bounds[0].z;
            bounds[0].z = bounds[1].z;
            bounds[1].z = temp;
        }
    }

    /**
     * @brief checks whether a ray hits the bounding box.
     * 
     * @param r ray.
     * @param t length of ray.
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
}; 

#endif /*__BOX__*/