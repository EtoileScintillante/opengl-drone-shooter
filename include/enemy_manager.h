/**
  * enemy_manager.h
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
    static const int MAX_ENEMIES; // max number of enemies that can exist in the game
    static const float INTERVAL;  // time in seconds between spawning of new enemies
    float currentTime;            // current time/frame
    float deltaTime;              // time passed between two frames

    /// Construct a Enemy Manager object. This also initializes the enemy objects.
    EnemyManager();

    /**
     * @brief Manages the enemy objects. 
     * 
     * @param player player object.
     * @param bulletRange range of bullet (maximum distance the bullet can travel).
     */
    void manage(Player &player, float bulletRange);

    /// Resets all values (for when you want to restart the game).
    void reset();

private:
    std::vector<std::shared_ptr<Enemy>> enemies; // vector containing pointers to the enemy objects
    float spawnTime;                             // needed to control spawning of enemies
    int enemyCount;                              // keeps track of already spawned enemies
};

#endif /*__ENEMY_MANAGER__*/