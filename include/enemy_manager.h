/**
  * enemy_manager.h
  * 
  * This file contains a class to control the lives of the enemies.
  * 
  * Created by EtoileScintillante.
  */

#ifndef __ENEMY_MANAGER_H__
#define __ENEMY_MANAGER_H__

#include <memory>

#include "enemy.h"
#include "player.h"

class EnemyManager
{
public:
    static const int MAX_ENEMIES;                // max number of enemies that can exist in the game
    static const float INTERVAL;                 // time in seconds between spawning of new enemies
    std::vector<std::shared_ptr<Enemy>> enemies; // vector containing pointers to the enemy objects
    float spawnTime;                             // needed to control spawning of enemies
    int enemyCount;                              // keeps track of already spawned enemies
    float currentTime;                           // current time/frame
    float deltaTime;                             // time passed between two frames

    /// Constructs a Enemy Manager object. This also initializes the enemy objects.
    EnemyManager();

    /**
     * @brief Manages the lives of the enemies.
     * 
     * @param playerPos player position.
     * @param viewMatrix view matrix.
     * @param projectionMatrix projection matrix.
     */
    void manage(glm::vec3 playerPos, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

    /// Resets all values (in case the game gets restarted).
    void reset();
};

#endif /*__ENEMY_MANAGER__*/