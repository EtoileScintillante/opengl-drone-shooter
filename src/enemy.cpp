#include "enemy.h"

const float Enemy::MIN_FLOAT_HEIGHT = 2.5f;
const float Enemy::MAX_FLOAT_HEIGHT = 7.0f;

Enemy::Enemy()
{
    // load model and shader
    drone = Model("resources/models/drone/E 45 Aircraft_obj.obj", false);
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

void Enemy::controlEnemyLife(bool shot, glm::vec3 bulletStartPos, glm::vec3 bulletDir, float bulletRange)
{
    // draw enemy
    spawn();

    // check for collisions
    if (shot)
    {
        collisionDetection(bulletStartPos, bulletDir, bulletRange);
    }

    // if enemy got hit, make it explode
    if (isDead)
    {
        dyingAnimation();
    }
}

void Enemy::collisionDetection(glm::vec3 bulletStartPos, glm::vec3 bulletDir, float bulletRange)
{
    // construct ray object with start position and direction of bullet
    glm::vec3 orig = bulletStartPos;
    glm::vec3 dir = bulletDir;
    Ray ray(orig, dir);

    // calculate bounding box
    calculateBoundingBox();

    // check for collision
    if (boundingBox.intersect(ray, bulletRange) == true)
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

    std::uniform_int_distribution<> xzPlane(-World::TERRAIN_SIZE + 2, World::TERRAIN_SIZE - 2); // define the range for x and z axis
    std::uniform_int_distribution<> yPlane(MIN_FLOAT_HEIGHT, MAX_FLOAT_HEIGHT);                 // define the range for y axis

    position = glm::vec3(xzPlane(gen), yPlane(gen), xzPlane(gen));
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
    modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0.0f, 1.0f, 0.0f)); // add (semi)random rotation (counter clockwise)
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f));
}

/* 
To construct a bounding box we need two things: a minimum bound and a maximum bound.
We also want the bounding box to rotate with the model.
We can imagine the bounding box like a quadrilateral inscribed in a circle where the center is 
the position of the enemy on the x z plane. The two bounds represent two points on that circle. 
We don't actually need all 4 points to create a bounding box, the two min and max bounds are enough.
If the model rotates N degrees, we also want those two points to rotate N degrees. 
So we baically we rotate those two points (the bounds) about the center by N degrees. 
The formula (for counter clockwise rotation) is as follows:

newX = cos(θ) * (X - Cx) - sin(θ) * (Y - Cy) + Cx.
newY = sin(θ) * (X - Cx) + cos(θ) * (Y - Cy) + Cy.

Cx and Cy are the center coordinates, θ is the angle in radians, 
X and Y are the initial coordinates and newX and newY are the rotated coordinates.

Formula from https://math.stackexchange.com/a/814981.

Important note: in this program the bounding box is shaped like a rectangle,
the z sides are bigger than the x sides since the model of the enemy is longer than it is wide. 
The bounding box does not contain the whole model, but this is not a problem because the model is quite big anyway, 
so it is not too difficult to hit it. Also, the y coordinate remains untouched because the height of the box does 
not change when rotating it around the y - axis. Lastly, the current implementation of calculateBoundingBox 
is based on the drone model scaled by factor 0.6. See /doc/bounding_box for a visualization.
*/

void Enemy::calculateBoundingBox()
{   
    // center of circle
    float Cx = position.x;
    float Cz = position.z;

    // initialize starting points/bounds
    float initialMinX = Cx - 0.3f; // x of min bound
    float initialMinZ = Cz - 1.0f; // z of min bound
    float initialMaxX = Cx + 0.3f; // x of max bound
    float initialMaxZ = Cz + 1.0f; // z of max bound

    // convert degrees to radians (sin and cos functions expect radians)
    float radians = (rotation * 3.14159265) / 180;

    // calculate new points/bounds (note that the y from the formula is replaced by z)
    float newMinX = (cos(radians) * (initialMinX - Cx)) - (sin(radians) * (initialMinZ - Cz)) + Cx;
    float newMinZ = (sin(radians) * (initialMinX - Cx)) + (cos(radians) * (initialMinZ - Cz)) + Cz;
    float newMaxX = (cos(radians) * (initialMaxX - Cx)) - (sin(radians) * (initialMaxZ - Cz)) + Cx;
    float newMaxZ = (sin(radians) * (initialMaxX - Cx)) + (cos(radians) * (initialMaxZ - Cz)) + Cz;

    // create bounding box with the new coordinates
    glm::vec3 vmin = glm::vec3(newMinX, position.y, newMinZ);
    glm::vec3 vmax = glm::vec3(newMaxX, position.y + 0.7f, newMaxZ);
    boundingBox = AABBox(vmin, vmax);
}