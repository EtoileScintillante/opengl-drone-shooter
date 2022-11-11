#include "collision_detection.h"

CollisionDetector::CollisionDetector(){}

void CollisionDetector::Detect(Player &player, EnemyManager &manager)
{
    if (player.shot)
    {
        // collision detection between player;s bullet and enemy
        for (unsigned int i = 0; i < manager.enemyCount - 1; i++)
        {
            PlayerAttacksEnemy(player, manager.enemies[i]);
        }
    }

    for (unsigned int i = 0; i < manager.enemyCount - 1; i++)
    {
        if (manager.enemies[i]->canDamage) 
        {
            // collision detection between enemy's laser and player
            EnemyAttacksPlayer(player, manager.enemies[i]);
        }
    }
}

void CollisionDetector::PlayerAttacksEnemy(Player &player, std::shared_ptr<Enemy> enemy)
{
    // construct ray object with start position and direction of bullet
    Ray ray(player.Position, player.Front);

    // check for collision
    if (enemy->boundingBox.intersect(ray, player.range) == true)
    {
        if (enemy->getLifeState()) // check if enemy is alive
        {
            enemy->gotHit(); // kill enemy
            player.updateKills(); // update killcount by one
        }
    }
}

void CollisionDetector::EnemyAttacksPlayer(Player &player, std::shared_ptr<Enemy> enemy)
{
    // construct ray object with enemy position and laser direction
    Ray ray = Ray(enemy->position, enemy->laserDirection);

    // check for collision
    if (player.boundingBox.intersect(ray, enemy->range))
    {
        player.gotAttacked(Enemy::DAMAGE); // decrease player's health
        enemy->canDamage = false; // set to false to ensure that player's health only decreases once per hit
    }
}