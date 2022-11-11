#include "enemy_manager.h"

const int EnemyManager::MAX_ENEMIES = 3;
const float EnemyManager::INTERVAL = 3.0f;

EnemyManager::EnemyManager()
{
    // set default values
    enemyCount = 1; // always start with at least one enemy
    spawnTime = 0;

    // initialize enemies
    for (unsigned int i = 0; i < MAX_ENEMIES; i++)
    {
        enemies.push_back(std::make_shared<Enemy>());
    }
}

void EnemyManager::manage(glm::vec3 playerPos, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    if (enemyCount < MAX_ENEMIES + 1) // enemyCount starts at 1
    {
        spawnTime += deltaTime;
        if (spawnTime >= INTERVAL)
        {
            enemyCount++;
            spawnTime = 0;
        }
    }
    
    for (unsigned int i = 0; i < enemyCount - 1; i++)
    {
        enemies[i]->currentFrame = currentTime;
        enemies[i]->deltaTime = deltaTime;
        enemies[i]->controlEnemyLife(playerPos, viewMatrix, projectionMatrix);
    }
}

void EnemyManager::reset()
{
    for (unsigned int i = 0; i < enemyCount - 1; i++)
    {
        enemies[i]->setDefaultValues(); // also reset enemies
    }
    enemyCount = 1; 
    spawnTime = 0;
}