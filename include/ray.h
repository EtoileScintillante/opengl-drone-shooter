/**
 * ray.h
 * 
 * Created by: EtoileScintillante
 * Resource: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
 */

#ifndef __RAY_H__
#define __RAY_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Ray 
{ 
public: 
glm::vec3 orig, dir;      
glm::vec3 invdir; 
int sign[3]; 
    /**
     * @brief Construct a new Ray object.
     * 
     * @param orig origin position of ray.
     * @param dir direction of ray.
     */
    Ray(glm::vec3 orig, glm::vec3 dir) : orig(orig), dir(dir) 
    { 
        invdir = 1.0f / dir;
        sign[0] = (invdir.x < 0); 
        sign[1] = (invdir.y < 0); 
        sign[2] = (invdir.z < 0); 
    } 
}; 

#endif /*__RAY__*/
