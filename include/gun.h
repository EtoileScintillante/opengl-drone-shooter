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
 * @brief Get the gun model matrix.
 * 
 * @param gunPosition position vector of gun.
 * @param cameraView camera view matrix.
 * @param scaleFactor make gun larger/smaller by this factor.
 * @param rotation y-axis counter clockwise rotation (in degrees)
 * @return glm::mat4 gun model matrix
 */
glm::mat4 getGunModelMatrix(glm::vec3 gunPosition, glm::mat4 cameraView, float scaleFactor, float rotation);

/**
 * @brief first part recoil animation: gun rotates up untill the angle has reached a certain value.
 * 
 * @param[out] shader shader for gun.
 * @param[out] modelMatrixGun model matrix of gun (output of getGunModelMatrix).
 * @param[out] angle x - axis rotation angle (upwards). The angle will be increased a little with every frame.
 * @param[out] down does gun need to rotate down? (should be false in this context). This will be set to true
 * if the angle has reached a certain value.
 * @see getGunModelMatrix().
 */
void startRecoilAnimation(Shader &shader, glm::mat4 &modelMatrixGun, float &angle, bool &down);

/**
 * @brief second part of recoil animation: gun rotates down, back to base position. The recoil animation
 * has finished when the gun is back to base position: x-axis rotation is back to zero.
 * 
 * @param[out] shader shader for gun.
 * @param[out] modelMatrixGun model matrix of gun (output of getGunModelMatrix).
 * @param[out] angle x - axis rotation angle (downwards). The angle will be decreased a little with every frame and will be set back to 
 * zero after the recoil animation has finished.
 * @param[out] shot has player takes a shot? This will be set back to false after the recoil animation has finished.
 * @param[out] down does gun need to rotate down? This should be true in this context and it will be set back to
 * false after recoil animation has finished.
 * @param[out] startRecoil start the recoil animation? This should be true in this context and it will be set back to false
 * after the recoil animation has finished. 
 */
void goBackToBase(Shader &shader, glm::mat4 &modelMatrixGun, float &angle, bool &shot, bool &down, bool &startRecoil);

#endif /*__GUN__*/

