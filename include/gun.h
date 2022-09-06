/**
 * gun.h
 * 
 * Created by EtoileScintillante.
 * This file contains functions that help with rendering the handgun + controlling the movements.
 */

#ifndef __GUN_H__
#define __GUN_H__

#include "shader.h"
#include "model.h"

/**
 * @brief adds up & down and forward & backward motion to the gun when player is walking.
 * 
 * @param[out] gunY y position of gun 
 * @param[out] gunZ z position of gun
 * @param currentFrame current frame
 */
void walkingMotion(float &gunY, float &gunZ, float currentFrame);

/**
 * @brief draws the handgun model.
 * 
 * @param gun gun model.
 * @param shader shader for gun model.
 */
void drawhandGun(Model gun, Shader &shader);

#endif /*__GUN__*/

