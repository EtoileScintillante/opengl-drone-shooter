/**
  * collision_detection.h
  * 
  * This file is used to handle collision detection
  * between the player's bullet and enemy and between
  * the enemy's laser and the player.
  * 
  * Created by EtoileScintillante.
  */

#ifndef __COLLISION_DETECTION__
#define __COLLISION_DETECTION__

#include "player.h"
#include "enemy.h"
#include "enemy_manager.h"
#include "box.h"
#include "ray.h"

class CollisionDetector
{
public:
  /// initializes new CollisionDetector object.
  CollisionDetector();

  /**
   * @brief Handles all collisions between player and enemy.
   * 
   * @param player player object.
   * @param manager enemy manager object.
   */
  void Detect(Player &player, EnemyManager &manager);

private:
  /**
   * @brief Handles collision between player's bullet and enemy.
   * 
   * @param player Player object.
   * @param enemy shared pointer enemy.
   */
  void PlayerAttacksEnemy(Player &player, std::shared_ptr<Enemy> enemy);

  /**
   * @brief Handles collsision detection between enemy's laser and player.
   * 
   * @param player Player object.
   * @param enemy shared pointer enemy.
   */
  void EnemyAttacksPlayer(Player &player, std::shared_ptr<Enemy> enemy);
};



#endif /*__COLLISION_DETECTION__*/