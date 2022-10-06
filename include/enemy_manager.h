/**
 * enemy_manager.h
 * 
 * Created by EtoileScintillante.
 */

#ifndef __ENEMY_MANAGER_H__
#define __ENEMY_MANAGER_H__

#include "enemy.h"

class EnemyManager
{
    /* TODO: create a class that manages the enemies
    - max number of enemies (static const unsigned int MAX_ENEMIES) that can be spawned
    - time interval: static const float SPAWN_INTERVAL; every SPAWN_INTERVAL seconds a new enemy spawns
    - data needed to spawn enemies: current time, delta time, player front, player position,
      player shot and bullet range (= TERRAIN_SIZE * 2) 
    - as for the above: maybe just make a method which takes as input the player object (by &), this is
      easier than passing all the player stuff to the enemymanager in the render loop in main.cpp (if
      we pass player by reference, it is just one line of code):
      controlEnemies(Player &player). Current and delta time are also needed but those will be attributes
      of the enemymanager class. 
    - variable to keep track of spawned enemies (enemyCount, should start at 1)
    - variable to keep track of spawn interval: spawnTime += deltaTime and everytime
      spawnTime >= SPAWN_INTERVAL a new enemy can spawn --> enemyCount++ as long as
      enemyCount <= MAX_ENEMIES.*/
};

#endif /*__ENEMY_MANAGER__*/