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

    /// Default constructor.
    AABBox();
    
    /**
     * @brief Constructs a new AABBox object.
     * 
     * @param vmim mimimum bound of box.
     * @param vmax maximum bound of box.
     */
    AABBox(glm::vec3 vmin, glm::vec3 vmax);

    /// @brief Checks whether the min values are acually smaller than max values.
    /// @return true if valid, else false.
    bool isValid();

    /// Swaps values when necessary (when they are not valid).
    void fix();

    /**
     * @brief Checks whether a ray hits the bounding box.
     * 
     * @param ray ray object.
     * @param length length of ray.
     * @return true if intersect, else false.
     */
    bool intersect(Ray ray, float length);
}; 

#endif /*__BOX__*/