/**
 * player.h
 *
 * Created by EtoileScintillante.
 * Based on the camera class made by Joey de Vries (from learnopengl).
 */

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "world.h"
#include "miniaudio.h"

class Player
{
public:
    // defines several possible options for player movement
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
    // default player values
    static const float YAW;         // eluer angle related
    static const float PITCH;       // euler angle related
    static const float SPEED;       // movement speed
    static const float SENSITIVITY; // mouse sensitivity
    static const float ZOOM;        // needed to set projection matrix
    // screen dimensions (used to set the projection matrix)
    static const int SCR_HEIGHT;
    static const int SCR_WIDTH;
    // other player attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // player movement options
    float MovementSpeed;
    float MouseSensitivity;
    // mouse
    double xPosIn; // mouse x position input
    double yPosIn; // mouse y position input
    // gun related
    bool shot; // has player taken a shot?
    // time
    float currentFrame;
    // game
    int health;
    bool isAlive;
    bool hasStarted;
    int kills;

    /**
     * @brief Construct a new Player object with vectors. Also initializes player related objects
     * like the gun and audio objects.
     *
     * @param position player position vector.
     * @param up player up vector.
     * @param yaw yaw value.
     * @param pitch pitch value.
     */
    Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    /**
     * @brief Construct a new Player object with scalar values. Also initializes player related objects
     * like the gun and audio objects.
     *
     * @param posX x value of position vector.
     * @param posY y value of position vector.
     * @param posZ z value of position vector.
     * @param upX x value of up vector.
     * @param upY y value of up vector.
     * @param upZ z value of up vector.
     * @param yaw yaw value.
     * @param pitch pitch value.
     */
    Player(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    /// Destructor.
    ~Player();

    /// Returns the view matrix calculated using Euler Angles and the LookAt Matrix.
    glm::mat4 GetViewMatrix() const;

    /// Get projection matrix.
    glm::mat4 getProjectionMatrix() const;

    /// Get ortho projection matrix (part of HUD)
    glm::mat4 getOrthoProjectionMatrix() const;

    /// Get gun model matrix.
    glm::mat4 getGunModelMatrix() const;

    /// Get player's kill count as string (part of HUD).
    std::string getKillsString() const;

    /// Get player's health as string (part of HUD).
    std::string getHealthString() const;

    /// Renders gun and controls the recoil movements.
    void controlGunRendering();

    /// Set default player values.
    void setDefaultValues();

    /// Resets all values in case player wants to restart the game (excluding isAlive).
    void resetAll();

    /**
     * @brief Processes all player input (keyboard + mouse).
     * 
     * @param window window pointer.
     * @param deltaTime delta time (time passed between two frames).
     */
    void processKeyboardMouse(GLFWwindow *window, float deltaTime);

private:
    // gun related
    Model gun;                // gun model
    Shader shader;            // gun shader
    glm::vec3 gunPosition;    // (base) position for gun
    glm::mat4 gunModelMatrix; // model matrix for gun
    float angle;              // recoil animation: this angle will be updated every frame to make the gun rotate up or down
    bool startRecoil;         // start recoil animation?
    bool goDown;              // recoil animation: gun needs to move down if true
    // player movement control (mouse input)
    bool isWalking;  // is player walking?
    bool firstMouse; // first time moving mouse?
    double lastX;    // last x position of mouse
    double lastY;    // last y position of mouse
    // audio
    ma_engine engine;             // miniaudio engine
    ma_sound walkingSound;        // sound object for walking soundeffect (needed to control looping of sound)
    int soundCount;               // needed to make sure that the gunshot will only be played once every shot
    std::string gunshotSoundPath; // path to gunshot wav file
    std::string walkSoundPath;    // path to walking wav file
    // matrices
    glm::mat4 projection;   // projection matrix
    glm::mat4 viewLocalMat; // view matrix with positional information removed (needed for rendering the gun)
    // for when resetting player
    glm::vec3 origPosition; // original position of player when starting game for first time

    /// Renders gun.
    void drawGun();

    /// Renders gunfire.
    void drawGunFire();

    /**
     * @brief Processes keyboard input.
     *
     * @param direction player movement direction (enum).
     * @param deltaTime time passed between two frames.
     */
    void ProcessKeyboard(Player_Movement direction, float deltaTime);

    /**
     * @brief Processes window input; controls attributes shot and isWalking and forwards the input
     * towards ProcessKeyboard method to process it further.
     *
     * @param window glfw window.
     * @param deltaTime time passed between two frames.
     */
    void processInput(GLFWwindow *window, float deltaTime);

    /**
     * @brief Processes input received from a mouse input system.
     *
     * @param constrainPitch limit pitch to a maximum of 90 degrees up and 45 degrees down?
     */
    void ProcessMouseMovement(GLboolean constrainPitch = true);

    /// When player is not moving create slow up and down movement to make player look alive.
    void passiveMotion();

    /// When player is walking, make gun move up and down.
    void walkingMotion();

    /// Sets projection matrix.
    void setProjectionMatrix();

    /// Sets gun model matrix.
    void setGunModelMatrix();

    /// Starts recoil animation for gun (gun starts rotating up).
    void startRecoilAnimation();

    /// Starts second part of recoil animation: gun rotates down to base position.
    void endRecoilAnimation();

    /// Calculates the front vector from the Player's (updated) Euler Angles.
    void updatePlayerVectors();
};

#endif /*__PLAYER__*/