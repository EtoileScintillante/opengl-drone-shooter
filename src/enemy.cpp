#include "enemy.h"

const float Enemy::MIN_FLOAT_HEIGHT = 2.5f;
const float Enemy::MAX_FLOAT_HEIGHT = 7.0f;

Enemy::Enemy()
{
    // load model and shader
    drone = Model("resources/models/aircraft/E 45 Aircraft_obj.obj", false);
    shader = Shader("shaders/explode.vert", "shaders/explode.frag", "shaders/explode.geom");

    // set default values
    isDead = false;
    deathTime = 0;
    magnitude = 0;
    rotation = static_cast<float>((rand() % 360)); 

    // generate random spawning position
    generatePosition();
}

Enemy::Enemy(std::vector<glm::vec3> treePos)
{
    this->treePositions = treePos;

    // load model and shader
    drone = Model("resources/models/aircraft/E 45 Aircraft_obj.obj", false);
    shader = Shader("shaders/explode.vert", "shaders/explode.frag", "shaders/explode.geom");

    // set default values
    isDead = false;
    deathTime = 0;
    magnitude = 0;
    rotation = static_cast<float>((rand() % 360)); 

    // generate random spawning position
    generatePosition();
}

void Enemy::spawn()
{
    // generate model matrix
    generateModelMatrix();

    // set unifroms
    shader.use();
    shader.setBool("isDead", isDead);
    shader.setFloat("magnitude", magnitude);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setMat4("model", modelMatrix);

    // draw
    drone.Draw(shader);
}

void Enemy::collisionDetection(glm::vec3 bulletStartPos, glm::vec3 bulletDir, float bulletRange)
{
    // comstruct ray object with start position and direction of bullet
    glm::vec3 orig = bulletStartPos;
    glm::vec3 dir = bulletDir;
    Ray ray(orig, dir);

    // construct bounding box object using the current position of the enemy
    glm::vec3 vmin = {position.x - 1.0f, position.y - 0.5f, position.z - 0.5f};
    glm::vec3 vmax = {position.x + 1.0f, position.y + 0.5f, position.z + 0.5f};
    AABBox box(vmin, vmax);

    if (box.intersect(ray, bulletRange) == true)
    {
        isDead = true;
    }
}

void Enemy::dyingAnimation()
{
    deathTime += deltaTime;

    // duration of explosion is about a second
    if (deathTime <= 0.25)
    {
        magnitude += 1.0f; // increase with every frame
        spawn();
    }
    else
    {
        isDead = false;
        deathTime = 0;
        magnitude = 0;
        rotation = static_cast<float>((rand() % 360)); 
        generatePosition();
    }
}

void Enemy::generatePosition()
{
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    std::uniform_int_distribution<> xzPlane(-World::TERRAIN_SIZE + 2, World::TERRAIN_SIZE - 2);  // define the range for x and z axis
    std::uniform_int_distribution<> yPlane(MIN_FLOAT_HEIGHT, MAX_FLOAT_HEIGHT); // define the range for y axis

    glm::vec3 newPos = glm::vec3(xzPlane(gen), yPlane(gen), xzPlane(gen));

    if (!spawnsInTree(newPos)) {position = newPos;} // make sure the mob does not spawn inside a tree
    else {generatePosition();}
}

void Enemy::generateModelMatrix()
{
    // create up and down motion so that the enemy is not standing still
    float newPosY = position.y + sin(currentFrame) * 0.05f;
    if (newPosY > (World::GROUND_Y + 1.0f) && newPosY < (position.y + 2.0f)) 
    {
        position.y = newPosY;
    }

    // create model matrix
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0.0f, 1.0f, 0.0f)); // add (semi)random rotation
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f));
}

bool Enemy::spawnsInTree(glm::vec3 position)
{
    for (unsigned int i = 0; i < treePositions.size(); i++)
    {
        if (treePositions[i].x == position.x && treePositions[i].z == position.z) // is there an overlap?
        {
            return true;
        }
    }

    return false; // no overlap
}