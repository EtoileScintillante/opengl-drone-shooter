/// === Shoot drones! === ///

// TODO:
// fix bounding box of enemy (right now it is not fitting/it is way too small)
// check gun walking movement: sometimes it does not look very smooth
// see if glfw keyboard input handler can be a method in the Player class (just like the mouse input),
// now keyboard input is handled with a function from window.h,
// though this is not a real problem, it may be better organized if both the mouse and keyboard input handlers
// are part of the player class so that window.h is only about initializing glfw setting up the window.

#include "player.h"
#include "enemy.h"
#include "window.h"

// player
Player player; 

// timing
float currentFrame;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // initialize and configure glwf and create window
    // -----------------------------------------------
    GLFWwindow *window = setup();

    // set callback functions
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // prepare game related objects
    // ----------------------------
    player.setup();
    World world;
    Enemy drone;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        player.currentFrame = currentFrame; // pass time to player object
        drone.currentFrame = currentFrame;  // also pass time variables to enemy object
        drone.deltaTime = deltaTime; 

        // movement when player is not moving (to make the player look more alive)
        player.passiveMotion();

        // input
        processInput(window, player, deltaTime);
        glfwGetCursorPos(window, &player.xPosIn, &player.yPosIn);
        player.ProcessMouseMovement();

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // view and projection for world objects and enemy object
        world.projection = player.getProjectionMatrix();
        world.view = player.GetViewMatrix();
        drone.projection = player.getProjectionMatrix();
        drone.view = player.GetViewMatrix();

        // draw world objects (ground, trees and skybox)
        world.Draw();

        // draw gun and handle gun recoil movement
        player.controlGunRendering();
        
        // control life of drone (spawning and dying)
        drone.controlEnemyLife(player.shot, player.Position, player.Front, World::TERRAIN_SIZE * 2);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}