/**
 * enemy.h
 *
 * This file contains an Enemy class.
 * 
 * Created by EtoileScintillante.
 */

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <cmath>
#include <random>
#include "world.h"
#include "player.h"
#include "ray.h"
#include "box.h"
#include "miniaudio.h"

class Enemy
{
public:
    // default enemy values
    static const float MAX_FLOAT_HEIGHT; // maximum floating height of enemy, measured from y = 0
    static const float MIN_FLOAT_HEIGHT; // minimum floating height of enemy, measured from y = 0
    static const float ATTACK_INTERVAL;  // time in seconds between attacks
    static const float INTERVAL;         // time in seconds between enemy dying and spawning again
    static const float DAMAGE;           // amount of damage the enemy can do to the player per hit
    static const float SPEED;            // movement speed of enemy
    // matrices
    glm::mat4 projection; // projection matrix
    glm::mat4 view;       // camera view matrix
    // time
    float currentFrame; // current frame/time
    float deltaTime;    // time passed between two frames
    // enemy attack related
    bool attack; // did enemy attack?

    /// Initializes new enemy object. Also initialisez 3D enemy model and audio related objects.
    Enemy();

    /// Destructor.
    ~Enemy();

    /// @brief Controls life of enemy: spawning and dying (collision detection is part of it).
    /// @param player Player object.
    /// @param bulletRange range of bullet (maximum distance the bullet can travel).

    /**
     * @brief Controls life of enemy: spawning and dying (collision detection is part of it).
     * 
     * @param player Player object.
     * @param bulletRange range of bullet (maximum distance the bullet can travel).
     */
    void controlEnemyLife(Player &player, float bulletRange);

    /// Sets all values back to default (for when enemy dies and then respawns).
    void setDefaultValues();

private:
    // 3D models
    Model drone;     // enemy model (in this program it's a drone)
    Model laserBeam; // model for fire (shown when enemy shoots bullet)
    // shaders
    Shader shaderDrone; // enemy shader (must include geometry shader for explosion effect)
    Shader shaderLaser; // laser beam shader (no geometry shader)
    // player related
    bool canIncreaseScore;    // used to ensure that the player's kill count only increases by 1 point every enemy death
    glm::vec3 playerPosition; // position of player
    // audio
    ma_engine engine;               // miniaudio engine
    ma_sound hoverSound;            // miniaudio sound object for hover sound (to control looping of sound)
    std::string soundExplosionPath; // path to explosion wav file
    std::string soundHoverPath;     // path to helicopter hovering wav file
    std::string soundLaserPath;     // path to laser beam wav file
    int soundCount;                 // needed to make sure that the explosion can only be heard once per enemy death
    int laserSoundCount;            // needed to make sure that the laser beam sound effect can only be heard once per shot
    // enemy attack related
    glm::mat4 modelMatrixFire; // model matrix for bullet fire
    int laserCount;            // needed to make sure that the laser beam can only be seen once per shot
    float attackTime;          // to control enemy attacks
    // other
    glm::vec3 position;    // position of enemy
    glm::mat4 modelMatrix; // model matrix for enemy
    AABBox boundingBox;    // enemy bounding box
    bool isDead;           // is enemy dead?
    float spawnInterval;   // used to control the time interval between enemy dying and spawning again
    float rotation;        // rotation angle of enemy in radians
    float explodeTime;     // used to control the duration of the dying animation (enemy explodes)
    float magnitude;       // used to control how the explosion of the enemy looks

    /// Spawns the enemy.
    void spawn();

    /**
     * @brief Detects whether the enemy has been hit by a bullet (using ray-box intersection).
     * 
     * @param bulletStartPos starting position of the bullet (in this program that is the player's position).
     * @param bulletDir direction of the bullet (in this program that is the front vector of the player).
     * @param bulletRange range of bullet (maximum distance the bullet can travel).
     */
    void collisionDetection(glm::vec3 bulletStartPos, glm::vec3 bulletDir, float bulletRange);

    /// Makes enemy explode when it gets shot (using the geometry shader).
    void dyingAnimation();

    /// Generates random position for enemy.
    void generatePosition();

    /// Generates model matrix for enemy.
    void generateModelMatrix();

    /// Calculates bounding box for enemy using the enemy's current position.
    void calculateBoundingBox();

    /// Returns the distance between the enemy position and player position on the x z plane.
    float distanceToPLayer();

    /// Updates the position vector of the enemy so that it moves towards the player.
    void moveToPlayer();

    /// Generates model matrix for the laser beam model.
    void generateLaserModelMatrix();

    /// Plays laser beam sound.
    void playLaserSound();

    /// Plays hover sound.
    void playHoverSound();

    /// Plays explosion sound.
    void playExplosionSound();

    /**
     * @brief Attacks player: shoot laser beam in player's direction. 
     * This also handles collision detection between the enemy's laser and the player.
     * 
     * @param player Player object.
     */
    void attackPlayer(Player &player);
};

#endif /*__ENEMY__*/