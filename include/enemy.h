/**
 * enemy.h
 *
 * This file contains an Enemy class.
 * The enemy is a drone that shoots laser beams in the direction of the player.
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
    static const float SPAWN_INTERVAL;   // time in seconds between enemy dying and spawning again
    static const float DAMAGE;           // amount of damage the enemy can do to the player per hit
    static const float SPEED;            // movement speed of enemy
    // matrices
    glm::mat4 projection; // projection matrix
    glm::mat4 view;       // camera view matrix
    // time
    float currentFrame; // current frame/time
    float deltaTime;    // time passed between two frames
    // enemy attack related
    bool canDamage;           // to ensure that enemy only damages player once per laser shot
    float range;              // range of laser beam 
    glm::vec3 laserDirection; // direction of laser beam
    // other
    glm::vec3 position; // position of enemy
    AABBox boundingBox; // enemy bounding box

    /// Initializes new enemy object. Also sets up enemy and laser beam models and audio related objects.
    Enemy();

    /// Destructor.
    ~Enemy();

    /**
     * @brief Controls life of enemy: spawning and dying.
     * 
     * @param playerPos player position.
     * @param viewMatrix view matrix.
     * @param projectionMatrix projection matrix.
     */
    void controlEnemyLife(glm::vec3 playerPos, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

    /// Sets all values back to default (for when enemy dies and then respawns).
    void setDefaultValues();

    /// Enemy got hit by player: update isDead value.
    void gotHit();

    /// Returns true if enemy is alive, else false.
    bool getLifeState() const;

private:
    // 3D models
    Model drone;     // enemy model (in this program it's a drone)
    Model laserBeam; // laser beam model 
    // shaders
    Shader shaderDrone; // enemy shader (includes geometry shader for explosion effect)
    Shader shaderLaser; // laser beam shader (no geometry shader)
    // audio
    ma_engine engine;               // miniaudio engine
    ma_sound hoverSound;            // miniaudio sound object for hover sound (to control looping of sound)
    std::string soundExplosionPath; // path to explosion wav file
    std::string soundHoverPath;     // path to helicopter hovering wav file
    std::string soundLaserPath;     // path to laser beam wav file
    int explosionSoundCount;        // needed to make sure that the explosion can only be heard once per enemy death
    // enemy attack related
    glm::mat4 modelMatrixLaser; // model matrix for laser beam
    bool renderLaser;           // did enemy attack? If so, render laser beam and play laser sound
    float attackTime;           // to control enemy attacks
    // other
    glm::mat4 modelMatrix;    // model matrix for enemy
    glm::vec3 playerPosition; // used to calculate enemy's model matrix, distance between enemy and player and more
    bool isDead;              // is enemy dead?
    float spawnInterval;      // used to control the time interval between enemy dying and spawning again
    float rotation;           // rotation angle of enemy in radians
    float explodeTime;        // used to control the duration of the dying animation (enemy explodes)
    float magnitude;          // used to control how the explosion of the enemy looks

    /// Spawns the enemy.
    void spawn();

    /// Makes enemy explode when it gets shot (using the geometry shader).
    void dyingAnimation();

    /// Generates random position for enemy.
    void generatePosition();

    /// Generates model matrix for enemy.
    void generateModelMatrix();

    /// Returns the distance between the enemy position and player position in 3D space (x y z).
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

    /// Calculates the direction of the laser beam.
    void calculateLaserDirection();

    /// Calculates bounding box for enemy using the enemy's current position.
    void calculateBoundingBox();
};

#endif /*__ENEMY__*/