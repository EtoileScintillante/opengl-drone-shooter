#include "player.h"

// screen dimensions
const int Player::SCR_HEIGHT = 600;
const int Player::SCR_WIDTH = 800;

// default camera values
const float Player::YAW = -90.0f;
const float Player::PITCH = 0.0f;
const float Player::SPEED = 4.0f;
const float Player::SENSITIVITY = 0.1f;
const float Player::ZOOM = 45.0f;

// player movement limitations
const float Player::BOTTOM_LIMIT_X = -World::TERRAIN_SIZE / 2;
const float Player::UPPER_LIMIT_X = World::TERRAIN_SIZE / 2;
const float Player::BOTTOM_LIMIT_Z = -World::TERRAIN_SIZE / 2;
const float Player::UPPER_LIMIT_Z = World::TERRAIN_SIZE / 2;

Player::Player(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    // set camera values
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Player::Player(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    // set camera values
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Player::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Player::setup()
{
    // set base position of gun
    gunPosition = glm::vec3(0.45f, -0.5f, -1.5f); // bottom right corner of screen

    // set angle and bools (recoil related)
    angle = 0;
    shot = false;
    startRecoil = false;
    goDown = false;

    // set matrices
    setProjectionMatrix(); // this matrix does not change while running the program
    setGunModelMatrix();

    // load model and compile shaders
    gun = Model("resources/models/handgun/Handgun_obj.obj", false);
    shader = Shader("shaders/model.vert", "shaders/model.frag");
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
        Position.x =BOTTOM_LIMIT_X;
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

void Player::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
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
        if (Pitch < -89.0f)
        {
            Pitch = -89.0f;
        }
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
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
    /* Here we make sure that the gun position always stays within a certain range,
    otherwise the gun will slowly move out of view */
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
}

void Player::drawGun()
{
    shader.use();
    shader.setMat4("view", GetViewMatrix());
    shader.setMat4("projection", projection);
    shader.setMat4("model", gunModelMatrix);
    gun.drawSpecificMesh(shader, 1);
    gun.drawSpecificMesh(shader, 3);
    gun.drawSpecificMesh(shader, 4);
    gun.drawSpecificMesh(shader, 6);
}

void Player::drawGunFire()
{
    shader.use();
    shader.setMat4("view", GetViewMatrix());
    shader.setMat4("projection", projection);
    shader.setMat4("model", gunModelMatrix);
    gun.drawSpecificMesh(shader, 5);
}

void Player::setProjectionMatrix()
{
    projection = glm::perspective(glm::radians(Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

void Player::setGunModelMatrix()
{
    gunModelMatrix = glm::mat4(1.0); 
    gunModelMatrix = glm::translate(gunModelMatrix, gunPosition); // place gun in bottom right corner
    gunModelMatrix = glm::rotate(gunModelMatrix, glm::radians(95.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate gun so it points inwards
    gunModelMatrix = glm::scale(gunModelMatrix, glm::vec3(0.6f)); // make gun a bit smaller
    gunModelMatrix = glm::inverse(GetViewMatrix()) * gunModelMatrix;
}

glm::mat4 Player::getProjectionMatrix()
{
    return projection;
}

glm::mat4 Player::getGunModelMatrix()
{
    return gunModelMatrix;
}

void Player::startRecoilAnimation()
{
    if (angle > 20.0f) // rotate up as long as angle has not surpassed this value
    {
        goDown = true;
    }

    gunModelMatrix = glm::rotate(gunModelMatrix, glm::radians(angle), glm::vec3(0.0f,  0.0f, 1.0f)); // rotate upwards
    
    angle += 6.0; // increase angle with every frame
}

void Player::endRecoilAnimation()
{
    if (angle > 0) // rotate down as long as base position is not reached
    {
        gunModelMatrix = glm::rotate(gunModelMatrix, glm::radians(angle), glm::vec3(0.0f,  0.0f, -1.0f)); // rotate downwards
    }

    if (angle <= 0) // set all values back to what they were before player shot gun
    {
        angle = 0;
        shot = false;
        goDown = false;
        startRecoil = false;
    }

    angle -= 6.0; // decrease angle with every frame
}

void Player::updateCameraVectors()
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