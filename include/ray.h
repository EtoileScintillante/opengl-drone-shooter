/**
 * ray.h
 * 
 * code from https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
 * Modified by: EtoileScintillante
 */

#ifndef __RAY_H__
#define __RAY_H__

#include "geometry.h"

class Ray 
{ 
public: 
    /**
     * @brief Construct a new Ray object.
     * 
     * @param orig original of ray.
     * @param dir direction of ray.
     */
    Ray(const Vec3<float> &orig, const Vec3<float> &dir) : orig(orig), dir(dir) 
    { 
        invdir = 1 / dir; // inverse direction 
        sign[0] = (invdir.x < 0); 
        sign[1] = (invdir.y < 0); 
        sign[2] = (invdir.z < 0); 
    } 
    Vec3<float> orig, dir;      
    Vec3<float> invdir; 
    int sign[3]; 
}; 

#endif /*__RAY__*/
