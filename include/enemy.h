/**
 * enemy.h
 *
 * Created by EtoileScintillante.
 */

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "world.h"
#include "ray.h"
#include "box.h"

class Enemy
{
public:
    static const float MAX_FLOAT_HEIGHT;  // maximum floating height of enemy, measured from y = 0
    static const float MIN_FLOAT_HEIGHT;  // minimum floating height of enemy, measured from y = 0
    Model drone;                          // enemy model (in this program it's a drone)
    Shader shader;                        // enemy shader (must include geometry shader for explosion effect)
    glm::mat4 projection;                 // projection matrix
    glm::mat4 view;                       // camera view matrix
    float currentFrame;                   // current frame/time
    float deltaTime;                      // time passed between two frames

    /// initialize new enemy object.
    Enemy();

    /**
     * @brief controls life of enemy: spawning and dying (collision detection is part of it).
     *
     * @param shot has player taken a shot?
     * @param bulletStartPos starting position of the bullet (in this program that is the camera position).
     * @param bulletDir direction of the bullet (in this program that is the front vector of the camera).
     * @param bulletRange range of bullet (maximum distance the bullet can travel).
     */
    void controlEnemyLife(bool shot, glm::vec3 bulletStartPos, glm::vec3 bulletDir, float bulletRange);

private:
    glm::vec3 position;    // position of enemy
    glm::mat4 modelMatrix; // model matrix for enemy
    bool isDead;           // is enemy dead?
    float deathTime;       // used to control the duration of the dying animation
    float magnitude;       // used to control the explosion (dying animation)
    float rotation;        // used in the creation the model matrix for enemy

    /// spawns the enemy.
    void spawn();

    /**
     * @brief detects whether the enemy has been hit by a bullet (using ray-box intersection).
     * Info from https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.
     *
     * @param bulletStartPos starting position of the bullet (in this program that is the camera position).
     * @param bulletDir direction of the bullet (in this program that is the front vector of the camera).
     * @param bulletRange range of bullet (maximum distance the bullet can travel).
     */
    void collisionDetection(glm::vec3 bulletStartPos, glm::vec3 bulletDir, float bulletRange);

    /// make enemy explode when it gets shot (using the geometry shader).
    void dyingAnimation();

    /// generates random position for enemy.
    void generatePosition();

    /// generate nodel matrix for enemy.
    void generateModelMatrix();
};

#endif /*__ENEMY__*/