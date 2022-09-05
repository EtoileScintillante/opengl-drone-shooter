/**
 * gun.h
 * 
 * Created by EtoileScintillante.
 * This file contains functions that help with controlling the handgun movement.
 */

#ifndef __GUN_H__
#define __GUN_H__

#include "shader.h"

/**
 * @brief adds up & down and forward & backward motion to the gun when player is walking.
 * 
 * @param[out] gunY y position of gun 
 * @param[out] gunZ z position of gun
 * @param currentFrame current frame
 */
void walkingMotion(float &gunY, float &gunZ, float currentFrame);

#endif /*__GUN__*/

