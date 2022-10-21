#include "enemy.h"

const float Enemy::MIN_FLOAT_HEIGHT = 2.5f;
const float Enemy::MAX_FLOAT_HEIGHT = 4.0f;
const float Enemy::ATTACK_INTERVAL = 2.0f;
const float Enemy::INTERVAL = 3.0f;
const float Enemy::DAMAGE = 10.0f;
const float Enemy::SPEED = 0.02f;

Enemy::Enemy()
{
    // load model and shaders
    drone = Model("resources/models/drone/E 45 Aircraft_obj.obj", false);
    laserBeam = Model("resources/models/handgun/Handgun_obj.obj", false);
    shaderDrone = Shader("shaders/explode.vert", "shaders/explode.frag", "shaders/explode.geom");
    shaderLaser = Shader("shaders/model.vert", "shaders/laser.frag");

    // set default values
    isDead = false;
    explodeTime = 0;
    magnitude = 0;
    soundCount = 0;
    laserCount = 0;
    laserSoundCount = 0;
    attackTime = 0;
    spawnInterval = 0;
    attack = false;
    canIncreaseScore = true;

    // generate random spawning position
    generatePosition();

    // wav file paths
    soundExplosionPath = "resources/audio/mixkit-shatter-shot-explosion-1693.wav";
    soundHoverPath = "resources/audio/helicopter-hovering-01.wav";
    soundLaserPath = "resources/audio/blaster-2-81267.wav";

    // miniaudio engine setup
    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
    {
        std::cout << "ERROR: failed to initialize audio engine." << std::endl;
    }

    // load hover sound
    result = ma_sound_init_from_file(&engine, soundHoverPath.c_str(), 0, NULL, NULL, &hoverSound);
    if (result != MA_SUCCESS)
    {
        std::cout << "ERROR: failed to load hover sound." << std::endl;
    }
}

Enemy::~Enemy()
{
    ma_sound_uninit(&hoverSound);
    ma_engine_uninit(&engine);
}

void Enemy::spawn()
{
    // depth test
    glEnable(GL_DEPTH_TEST);

    // update position
    moveToPlayer();

    // generate model matrix
    generateModelMatrix();

    // set unifroms
    shaderDrone.use();
    shaderDrone.setBool("isDead", isDead);
    shaderDrone.setFloat("magnitude", magnitude);
    shaderDrone.setMat4("view", view);
    shaderDrone.setMat4("projection", projection);
    shaderDrone.setMat4("model", modelMatrix);

    // draw enemy
    drone.Draw(shaderDrone);

    // draw laser beam
    if (attack && laserCount == 0)
    {
        laserCount++;
        generateLaserModelMatrix();
        shaderLaser.use();
        shaderLaser.setMat4("view", view);
        shaderLaser.setMat4("projection", projection);
        shaderLaser.setMat4("model", modelMatrixFire);
        laserBeam.drawSpecificMesh(shaderLaser, 5);
        playLaserSound();
    }
}

void Enemy::controlEnemyLife(Player &player, float bulletRange)
{
    // set variables
    playerPosition = player.Position;
    projection = player.getProjectionMatrix();
    view = player.GetViewMatrix();

    // draw enemy and optionally laser beam if enemy attacks
    if (!isDead)
    {
        // update attack time
        attackTime += deltaTime;

        // check if enemy can attack
        if (attackTime >= ATTACK_INTERVAL)
        {
            attack = true;
            attackPlayer(player);
        }

        // draw
        spawn();

        // play hover sound
        playHoverSound();
    }

    // check for collisions
    if (player.shot)
    {
        collisionDetection(playerPosition, player.Front, bulletRange);
    }

    // if enemy died: stop hover sound, play explosion sound and make enemy explode
    if (isDead)
    {
        // increase player's kill count
        if (canIncreaseScore)
        {
            player.kills++;
            canIncreaseScore = false;
        }

        // stop hover sound
        ma_sound_stop(&hoverSound);

        // play explosion sound
        playExplosionSound();

        // enemy explosion
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
    explodeTime += deltaTime;

    // duration of explosion is about a second
    if (explodeTime <= 0.25)
    {
        magnitude += 1.0f; // increase with every frame
        spawn();
    }
    // spawn enemy again in new position after time interval has passed
    else
    {
        spawnInterval += deltaTime;
        if (spawnInterval >= INTERVAL)
        {
            setDefaultValues();
        }
    }
}

void Enemy::generatePosition()
{
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    std::uniform_int_distribution<> xzPlane(-World::TERRAIN_SIZE + 2, World::TERRAIN_SIZE - 2); // define the range for x and z axis
    std::uniform_int_distribution<> yPlane(MIN_FLOAT_HEIGHT, MAX_FLOAT_HEIGHT);                 // define the range for y axis

    float posX = xzPlane(gen); // generate x position
    float posZ = xzPlane(gen); // generate z position

    // add limitation: enemy can not spawn in the inner square of the terrain square
    if (posX > -World::TERRAIN_SIZE / 2 && posX < World::TERRAIN_SIZE / 2)
    {
        if (posX < 0)
        {
            posX = -World::TERRAIN_SIZE / 2;
        }
        else
        {
            posX = World::TERRAIN_SIZE / 2;
        }
    }
    if (posZ > -World::TERRAIN_SIZE / 2 && posZ < World::TERRAIN_SIZE / 2)
    {
        if (posZ < 0)
        {
            posZ = -World::TERRAIN_SIZE / 2;
        }
        else
        {
            posZ = World::TERRAIN_SIZE / 2;
        }
    }

    position = glm::vec3(posX, yPlane(gen), posZ);
}

void Enemy::generateModelMatrix()
{
    // calculate rotation angle so that the enemy points towards the player's position
    float x = playerPosition.x - position.x;
    float z = playerPosition.z - position.z;
    rotation = atan2(z, x);
    rotation += glm::radians(90.0f);

    // create model matrix
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0.0f, -1.0f, 0.0f));
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

Important note: in this program the y coordinate remains untouched because the height of the box does
not change when rotating it around the y - axis. Also, the current implementation of calculateBoundingBox
is based on the drone model scaled by factor 0.6. The bounding box does not contain the whole model,
but this is not a problem because the model is quite big, so it is not too difficult to hit it.
See /doc/bounding_box for a visualization.
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

    // calculate new points/bounds (note that the y from the formula is replaced by z)
    float newMinX = (cos(rotation) * (initialMinX - Cx)) - (sin(rotation) * (initialMinZ - Cz)) + Cx;
    float newMinZ = (sin(rotation) * (initialMinX - Cx)) + (cos(rotation) * (initialMinZ - Cz)) + Cz;
    float newMaxX = (cos(rotation) * (initialMaxX - Cx)) - (sin(rotation) * (initialMaxZ - Cz)) + Cx;
    float newMaxZ = (sin(rotation) * (initialMaxX - Cx)) + (cos(rotation) * (initialMaxZ - Cz)) + Cz;

    // create bounding box with the new coordinates
    glm::vec3 vmin = glm::vec3(newMinX, position.y, newMinZ);
    glm::vec3 vmax = glm::vec3(newMaxX, position.y + 0.7f, newMaxZ);
    boundingBox = AABBox(vmin, vmax);
}

float Enemy::distanceToPLayer()
{
    float dx = pow((position.x - playerPosition.x), 2);
    float dy = pow((position.y - playerPosition.y), 2);
    float dz = pow((position.z - playerPosition.z), 2);
    return sqrt(dx + dy + dz);
}

void Enemy::moveToPlayer()
{
    // create direction vector
    glm::vec3 direction;
    direction.x = playerPosition.x - position.x;
    direction.y = position.y; // y level of enemy does not change
    direction.z = playerPosition.z - position.z;

    // normalize vector
    float magnitude = distanceToPLayer();
    if (magnitude < 1)
    {
        magnitude = 1;
    }
    direction.x /= magnitude;
    direction.z /= magnitude;

    // update position
    position.x += direction.x * SPEED;
    position.z += direction.z * SPEED;
}

void Enemy::generateLaserModelMatrix()
{
    // initialize fire model matrix
    modelMatrixFire = glm::mat4(1.0f);

    // add offsets (note: these offsets are based on enemy scale factor 0.6)
    rotation += glm::radians(270.0f);
    modelMatrixFire = glm::translate(modelMatrixFire, {position.x, position.y - 0.6f, position.z}); // position offset
    modelMatrixFire = glm::rotate(modelMatrixFire, rotation, glm::vec3(0.0f, -1.0f, 0.0f)); // rotation offset
    modelMatrixFire = glm::scale(modelMatrixFire, glm::vec3(0.6f));
}

void Enemy::playLaserSound()
{
    // compute distance to player
    float d = distanceToPLayer();

    // play sound if conditions are true
    if (d <= 50 && laserSoundCount == 0)
    {
        laserSoundCount++;
        // same calculation as in playHoverSound, only the range differs
        float volume = 1.5 - ((d / 50) * (1.5 - 0.01) + 0.1);
        ma_engine_set_volume(&engine, volume);
        ma_engine_play_sound(&engine, soundLaserPath.c_str(), NULL);
        attack = false;
        laserSoundCount = 0;
        laserCount = 0;
        attackTime = 0;
    }
}

void Enemy::playHoverSound()
{
    // compute distance to player
    float d = distanceToPLayer();

    // play sound if enemy close enough to player
    if (d <= 30)
    {
        /* Here the distance in range 0 - 30 is mapped to the volume in range
        0.01 - 1.5. The volume is then subtracted from 1.5 to make sure that
        a lower distance results in a higher volume and not the other way around */
        float volume = 1.5 - ((d / 30) * (1.5 - 0.01) + 0.1);
        ma_sound_set_volume(&hoverSound, volume);
        ma_sound_set_looping(&hoverSound, true);
        ma_sound_start(&hoverSound);
    }
}

void Enemy::playExplosionSound()
{
    // compute distance to player
    float d = distanceToPLayer();

    // play explosion sound if enemy is close enough to player
    if (soundCount == 0 && d <= 50)
    {
        soundCount++;
        // same calculation as in playHoverSound, only the range differs
        float volume = 1.5 - ((d / 50) * (1.5 - 0.01) + 0.1);
        ma_engine_set_volume(&engine, volume);
        ma_engine_play_sound(&engine, soundExplosionPath.c_str(), NULL);
    }
}

void Enemy::setDefaultValues()
{
    isDead = false;
    attack = false;
    explodeTime = 0;
    magnitude = 0;
    soundCount = 0;
    laserCount = 0;
    laserSoundCount = 0;
    attackTime = 0;
    spawnInterval = 0;
    canIncreaseScore = true;
    ma_sound_stop(&hoverSound);
    generatePosition();
}

void Enemy::attackPlayer(Player &player)
{
    // create a direction vector and normalize it
    float dx, dy, dz, d;
    dx = player.Position.x - position.x;
    dy = player.Position.y - position.y;
    dz = player.Position.z - position.z;
    d = distanceToPLayer(); 
    glm::vec3 dir = {dx/d, dy/d, dz/d}; // normalized direction vector

    // add random offset between min and max (otherwise the enemy's aim would too good)
    float min = -0.1;
    float max = 0.1;
    float offset = ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
    dir.x += offset;
    dir.y += offset;
    dir.z += offset;

    // perform collision detection
    Ray r = Ray(position, dir);
    if (player.boundingBox.intersect(r, World::TERRAIN_SIZE * 2))
    {
        player.gotAttacked(DAMAGE);
    }
}