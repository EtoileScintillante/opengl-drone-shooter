/**
 * player.h
 *
 * Created by EtoileScintillante.
 * Based on the camera class made by Joey de Vries (from learnopengl).
 */

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "world.h"

class Player
{
public:
    // Defines several possible options for player movement
    enum Player_Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };
    // player movement limits
    static const float BOTTOM_LIMIT_X; // bottom limit x plane
    static const float UPPER_LIMIT_X;  // upper limit x plane
    static const float BOTTOM_LIMIT_Z; // bottom limit z ppane
    static const float UPPER_LIMIT_Z;  // upper limit z plane
    // default camera values
    static const float YAW;
    static const float PITCH;
    static const float SPEED;
    static const float SENSITIVITY;
    static const float ZOOM;
    // screen dimensions (used to set the projection matrix)
    static const int SCR_HEIGHT; 
    static const int SCR_WIDTH; 
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    // gun related
    Model gun;                // gun model
    Shader shader;            // gun shader
    glm::vec3 gunPosition;    // (base) position for gun
    glm::mat4 gunModelMatrix; // model matrix for gun
    bool shot;                // has player taken a shot?
    // time
    float currentFrame;
    // other
    bool isWalking;       // is player walking?

    /// constructor with vectors.
    Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    /// constructor with scalar values.
    Player(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    /// setup related objects (like the gun).
    void setup();

    /// returns the view matrix calculated using Euler Angles and the LookAt Matrix.
    glm::mat4 GetViewMatrix();

    /**
     * @brief Processes keyboard input.
     *
     * @param direction player movement direction (ENUM).
     * @param deltaTime delta time (time passed between two frames).
     */
    void ProcessKeyboard(Player_Movement direction, float deltaTime);

    /**
     * @brief Processes input received from a mouse input system.
     *
     * @param xoffset offset x position of mouse.
     * @param yoffset offset y position of mouse.
     * @param constrainPitch limit pitch to a maximum of 90 degrees (up an down)?
     */
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    /// when player is not moving create slow up and down movement to make player seem alive.
    void passiveMotion();

    /// when player is walking, make gun move up and down.
    void walkingMotion();

    /// render gun.
    void drawGun();

    /// render gunfire.
    void drawGunFire();

    /// get projection matrix.
    glm::mat4 getProjectionMatrix();

    /// get gun model matrix.
    glm::mat4 getGunModelMatrix();

    /// renders gun and controls the recoil movements.
    void controlGunRendering();

private:
    float angle;               // recoil animation: this angle will be updated every frame to make the gun rotate up or down
    bool startRecoil;          // start recoil animation?
    bool goDown;               // recoil animation: gun needs to move down if true
    glm::mat4 projection;      // projection matrix
    glm::mat4 viewLocalMat;	   // view matrix with positional information removed (needed for rendering the gun)

    /// set projection matrix.
    void setProjectionMatrix();

    /// set gun model matrix.
    void setGunModelMatrix(); 

    /// starts recoil animation for gun (gun starts moving down).
    void startRecoilAnimation();

    /// starts second part of recoil animation: gun moves back up to base position.
    void endRecoilAnimation();

    /// calculates the front vector from the Camera's (updated) Euler Angles.
    void updateCameraVectors();
};

#endif