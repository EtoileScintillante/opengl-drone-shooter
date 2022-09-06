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
 * @brief draws the gun model.
 * 
 * @param gun gun model.
 * @param[out] shader shader for gun model.
 */
void drawhandGun(Model gun, Shader &shader);

/**
 * @brief calculates the model matrix for the gun and sets all the uniforms for the handgun shader.
 * 
 * @param[out] shader shader for gun model.
 * @param gunPosition position vector for gun.
 * @param cameraView camera view matrix.
 * @param scaleFactor make gun smaller/larger by a certain factor.
 * @param rotation y-axis rotation (counter clockwise).
 */
void configureGunShader(Shader &shader,  glm::vec3 gunPosition, glm::mat4 cameraView, float scaleFactor, float rotation);

#endif /*__GUN__*/

