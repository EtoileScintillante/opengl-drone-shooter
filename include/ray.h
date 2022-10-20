/**
 * ray.h
 * 
 * This file contains a Ray class (needed for collision detection).
 * Resource: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
 * 
 * Created by EtoileScintillante.
 */

#ifndef __RAY_H__
#define __RAY_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Ray 
{ 
public: 
    glm::vec3 orig, dir; // origin, direction vector
    glm::vec3 invdir;    // inverse direction vector
    int sign[3];         // holds 3 vectors: one for each plane (x, y, z)

    /**
     * @brief Constructs a new Ray object.
     * 
     * @param orig origin position of ray.
     * @param dir direction of ray.
     */
    Ray(glm::vec3 orig, glm::vec3 dir);
}; 

#endif /*__RAY__*/
