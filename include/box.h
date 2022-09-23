/**
 * box.h
 * 
 * Created by EtoileScintillante.
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
    AABBox(glm::vec3 vmin, glm::vec3 vmax);

    /// @brief checks whether the min values are acually smaller than max values.
    /// @return true if valid, else false.
    bool isValid();

    /// @brief swaps values when necessary (when they are not valid).
    void fix();

    /**
     * @brief checks whether a ray hits the bounding box.
     * 
     * @param r ray.
     * @param t length of ray.
     * @return true if hit.
     * @return false if no hit.
     */
    bool intersect(Ray r, float t);
}; 

#endif /*__BOX__*/