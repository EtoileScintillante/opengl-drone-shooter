/**
 * enemy.h
 *
 * Created by EtoileScintillante.
 */

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <cmath>
#include "world.h"
#include "ray.h"
#include "box.h"
#include "miniaudio.h"

class Enemy
{
public:
    static const float MAX_FLOAT_HEIGHT;  // maximum floating height of enemy, measured from y = 0
    static const float MIN_FLOAT_HEIGHT;  // minimum floating height of enemy, measured from y = 0
    static const float SPEED;             // movement speed of enemy
    Model drone;                          // enemy model (in this program it's a drone)
    Shader shader;                        // enemy shader (must include geometry shader for explosion effect)
    glm::mat4 projection;                 // projection matrix
    glm::mat4 view;                       // camera view matrix
    float currentFrame;                   // current frame/time
    float deltaTime;                      // time passed between two frames
    glm::vec3 playerPosition;             // position of player
    ma_engine engine;                     // miniaudio engine 
    ma_sound hoverSound;                  // miniaudio sound object for hover sound (to control looping of sound)
    std::string soundExplosionPath;       // path to explosion wav file
    std::string soundHoverPath;           // path to helicopter hovering wav file
    float soundCount;                     // needed to make sure that the explosion can only be heard once per enemy death

    /// Initializes new enemy object. Also initialisez 3D enemy model and audio related objects.
    Enemy();

    /// Destructor.
    ~Enemy();

    /**
     * @brief Controls life of enemy: spawning and dying (collision detection is part of it).
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
    AABBox boundingBox;    // enemy bounding box 
    bool isDead;           // is enemy dead?
    float rotation;        // rotation angle of enemy in radians
    float deathTime;       // used to control the duration of the dying animation
    float magnitude;       // used to control the explosion (dying animation)

    /// Spawns the enemy.
    void spawn();

    /**
     * @brief Detects whether the enemy has been hit by a bullet (using ray-box intersection).
     *
     * @param bulletStartPos starting position of the bullet (in this program that is the camera position).
     * @param bulletDir direction of the bullet (in this program that is the front vector of the camera).
     * @param bulletRange range of bullet (maximum distance the bullet can travel).
     */
    void collisionDetection(glm::vec3 bulletStartPos, glm::vec3 bulletDir, float bulletRange);

    /// Make enemy explode when it gets shot (using the geometry shader).
    void dyingAnimation();

    /// Generates random position for enemy.
    void generatePosition();

    /// Generate model matrix for enemy.
    void generateModelMatrix();

    /// Calculates bounding box for enemy using the enemy's current position.
    void calculateBoundingBox();

    /// Returns the distance between the enemy position and player position on the x z plane.
    float distanceToPLayer();

    /// Updates the position vector of the enemy so that it moves towards the player.
    void MoveToPlayer();
};

#endif /*__ENEMY__*/