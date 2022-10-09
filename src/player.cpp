#include "player.h"

// screen dimensions
const int Player::SCR_HEIGHT = 600;
const int Player::SCR_WIDTH = 800;

// default player values
const float Player::YAW = -90.0f;
const float Player::PITCH = 0.0f;
const float Player::SPEED = 5.0f;
const float Player::SENSITIVITY = 0.1f;
const float Player::ZOOM = 45.0f;

// player movement limitations
const float Player::BOTTOM_LIMIT_X = -World::TERRAIN_SIZE / 2;
const float Player::UPPER_LIMIT_X = World::TERRAIN_SIZE / 2;
const float Player::BOTTOM_LIMIT_Z = -World::TERRAIN_SIZE / 2;
const float Player::UPPER_LIMIT_Z = World::TERRAIN_SIZE / 2;

Player::Player(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
{
    // default mouse related values
    lastX = SCR_WIDTH / 2.0f;
    lastY = SCR_HEIGHT / 2.0f;
    firstMouse = true;
    // set player values
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updatePlayerVectors();

    // set base position of gun
    gunPosition = glm::vec3(0.45f, -0.5f, -1.5f); // bottom right corner of screen

    // set default values
    angle = 0;
    soundCount = 0;
    shot = false;
    startRecoil = false;
    goDown = false;
    kills = 0;
    isAlive = true;
    health = 100;

    // set matrices
    setProjectionMatrix(); // this matrix does not change while running the program
    setGunModelMatrix();

    // load model and compile shaders
    gun = Model("resources/models/handgun/Handgun_obj.obj", false);
    shader = Shader("shaders/model.vert", "shaders/model.frag");

    // wav file paths
    gunshotSoundPath = "resources/audio/gun-gunshot-02.wav";
    walkSoundPath = "resources/audio/footsteps.wav";

    // miniaudio engines setup
    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        std::cout << "ERROR: failed to initialize audio engine." << std::endl;
    }
    
    // load walking sound
    result = ma_sound_init_from_file(&engine, walkSoundPath.c_str(), 0, NULL, NULL, &walkingSound);
    if (result != MA_SUCCESS) 
    {
        std::cout << "ERROR: failed to load walking sound." << std::endl;
    }
    ma_sound_set_looping(&walkingSound, true);
}

Player::Player(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
{
    // default mouse related values
    lastX = SCR_WIDTH / 2.0f;
    lastY = SCR_HEIGHT / 2.0f;
    firstMouse = true;
    // set player values
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updatePlayerVectors();

    // set base position of gun
    gunPosition = glm::vec3(0.45f, -0.5f, -1.5f); // bottom right corner of screen

    // set default values
    angle = 0;
    soundCount = 0;
    shot = false;
    startRecoil = false;
    goDown = false;

    // set matrices
    setProjectionMatrix(); // this matrix does not change while running the program
    setGunModelMatrix();

    // load model and compile shaders
    gun = Model("resources/models/handgun/Handgun_obj.obj", false);
    shader = Shader("shaders/model.vert", "shaders/model.frag");

    // wav file paths
    gunshotSoundPath = "resources/audio/gun-gunshot-02.wav";
    walkSoundPath = "resources/audio/footsteps.wav";

    // miniaudio engines setup
    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        std::cout << "ERROR: failed to initialize audio engine." << std::endl;
    }
    
    // load walking sound
    result = ma_sound_init_from_file(&engine, walkSoundPath.c_str(), 0, NULL, NULL, &walkingSound);
    if (result != MA_SUCCESS) 
    {
        std::cout << "ERROR: failed to load walking sound." << std::endl;
    }
    ma_sound_set_volume(&walkingSound, 1.0);
    ma_sound_set_looping(&walkingSound, true);
}

Player::~Player()
{
    ma_sound_uninit(&walkingSound);
    ma_engine_uninit(&engine);
}

glm::mat4 Player::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Player::ProcessKeyboard(Player_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
    {
        Position += Front * velocity;
    }
    if (direction == BACKWARD)
    {
        Position -= Front * velocity;
    }
    if (direction == LEFT)
    {
        Position -= Right * velocity;
    }
    if (direction == RIGHT)
    {
        Position += Right * velocity;
    }

    // make sure player stays at ground level
    Position.y = 0;

    // also make sure player stays between the accepted x and z limits
    if (Position.x < BOTTOM_LIMIT_X)
    {
        Position.x = BOTTOM_LIMIT_X;
    }
    if (Position.x > UPPER_LIMIT_X - 1.0f)
    {
        Position.x = UPPER_LIMIT_X - 1.0f;
    }
    if (Position.z < BOTTOM_LIMIT_Z)
    {
        Position.z = BOTTOM_LIMIT_Z;
    }
    if (Position.z > UPPER_LIMIT_Z - 1.0f)
    {
        Position.z = UPPER_LIMIT_Z - 1.0f;
    }
}

void Player::processInput(GLFWwindow *window, float deltaTime)
{

    // close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // player moves
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        isWalking = true;
        ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        isWalking = true;
        ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        isWalking = true;
        ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        isWalking = true;
        ProcessKeyboard(RIGHT, deltaTime);
    }

    // player shoots gun
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
    {
        if (soundCount == 0)
        {
            soundCount++;
            ma_engine_set_volume(&engine, 1.3);
            ma_engine_play_sound(&engine, gunshotSoundPath.c_str(), NULL); 
        }
        shot = true;
    }

    // add walking motion and sound if player moves
    if (isWalking)
    {
        ma_sound_start(&walkingSound);
        walkingMotion();
    }
    else // stop playing sound if player stops walking
    {
        ma_sound_stop(&walkingSound);
    }

    isWalking = false; // set back to false again
}

void Player::ProcessMouseMovement(GLboolean constrainPitch)
{
    float xpos = static_cast<float>(xPosIn);
    float ypos = static_cast<float>(yPosIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top.

    lastX = xpos;
    lastY = ypos;

    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
        {
            Pitch = 89.0f;
        }
        if (Pitch < -44.0f)
        {
            Pitch = -44.0f;
        }
    }

    // update Front, Right and Up vectors using the updated Euler angles
    updatePlayerVectors();
}

void Player::passiveMotion()
{
    if (!isWalking)
    {
        Position.y = sin(currentFrame * 2) * 0.02f;
    }
}

void Player::walkingMotion()
{
    float newPosZ = gunPosition.z + sin(currentFrame * 15) * 0.004f;
    if (newPosZ > -1.52 && newPosZ < -1.48)
    {
        gunPosition.z = newPosZ;
    }
    float newPosY = gunPosition.y + sin(currentFrame * 15) * 0.004f;
    if (newPosY > -0.52 && newPosY < -0.48)
    {
        gunPosition.y = newPosY;
    }
    gunModelMatrix[3] = glm::vec4(gunPosition, 1.0);
}

void Player::drawGun()
{
    // set uniforms and draw gun
    shader.use();
    shader.setMat4("view", viewLocalMat);
    shader.setMat4("projection", projection);
    shader.setMat4("model", gunModelMatrix);
    gun.drawSpecificMesh(shader, 1);
    gun.drawSpecificMesh(shader, 3);
    gun.drawSpecificMesh(shader, 4);
    gun.drawSpecificMesh(shader, 6);
}

void Player::drawGunFire()
{
    // set uniforms and draw gun fire
    shader.use();
    shader.setMat4("view", viewLocalMat);
    shader.setMat4("projection", projection);
    shader.setMat4("model", gunModelMatrix);
    gun.drawSpecificMesh(shader, 5);
}

void Player::setProjectionMatrix()
{
    projection = glm::perspective(glm::radians(ZOOM), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

void Player::setGunModelMatrix()
{
    gunModelMatrix = glm::mat4(1.0);

    // orient the gun in the same direction as where the player is looking at
    gunModelMatrix[0] = glm::vec4(Right, 0.0);
    gunModelMatrix[1] = glm::vec4(Up, 0.0);
    gunModelMatrix[2] = glm::vec4(Front, 0.0);
    gunModelMatrix[3] = glm::vec4(gunPosition, 1.0); // place gun in bottom right corner
    gunModelMatrix = glm::scale(gunModelMatrix, glm::vec3(0.6f)); // make gun a bit smaller
    /* here two rotations are applied; one to make the gun barrel face the same direction as the player,
    so that it doesn't point towards the player's face and another rotation to make the gun
    point slightly inwards so that it matches the bullet direction (which is just the camera lookAt matrix) */
    gunModelMatrix = glm::rotate(gunModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    gunModelMatrix = glm::rotate(gunModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // position the gun relative to the player
    viewLocalMat = GetViewMatrix();
    viewLocalMat[3] = glm::vec4(0.0, 0.0, 0.0, 1.0);
}

glm::mat4 Player::getProjectionMatrix() const
{
    return projection;
}

glm::mat4 Player::getOrthoProjectionMatrix() const
{
    return glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
}

glm::mat4 Player::getGunModelMatrix() const
{
    return gunModelMatrix;
}

std::string Player::getKillsString() const
{
    return "Kills: " + std::to_string(kills);
}

std::string Player::getHealthString() const
{   
    return "Health: " + std::to_string(health);
}

void Player::startRecoilAnimation()
{
    if (angle > 20.0f) // rotate up as long as angle has not surpassed this value
    {
        goDown = true;
    }

    gunModelMatrix = glm::rotate(gunModelMatrix, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate upwards

    angle += 6.0; // increase angle with every frame
}

void Player::endRecoilAnimation()
{
    if (angle > 0) // rotate down as long as base position is not reached
    {
        angle -= 6.0; // decrease angle with every frame
        gunModelMatrix = glm::rotate(gunModelMatrix, glm::radians(angle), glm::vec3(0.0f, 0.0f, -1.0f)); // rotate downwards
    }

    if (angle <= 0) // set all values back to what they were before player shot gun
    {
        angle = 0;
        soundCount = 0;
        shot = false;
        goDown = false;
        startRecoil = false;
    }
}

void Player::controlGunRendering()
{
    // add passive player motion
    if (!isWalking)
    {
        passiveMotion();
    }

    // draw the handgun in base position
    if (!shot)
    {
        drawGun();
    }

    // draw the gunfire (only for one frame, otherwise the gunfire is visible for too long, which just looks weird)
    if (shot && !startRecoil)
    {
        drawGunFire();
        startRecoil = true;
    }

    // start the recoil animation
    if (startRecoil)
    {
        if (!goDown)
        {
            startRecoilAnimation(); // start moving up
        }
        else
        {
            endRecoilAnimation(); // start moving down
        }
        drawGun(); // render rotating handgun
    }
}

void Player::updatePlayerVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors
    Up = glm::normalize(glm::cross(Right, Front));
}