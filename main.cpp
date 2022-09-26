/// === Shoot drones! === ///

// TODO:
// Fix bounding box of enemy; right now it does not fit well
// Make the drones dangerous; they should be able to kill the player in some way (maybe they can shoot bullets too?)
// Add sound effects
// Add scoring system
// Maybe add small random offset to the bullet direction when player shoots gun

#include "player.h"
#include "enemy.h"
#include "glfw_setup.h"

int main()
{
    // initialize and configure glwf and create window
    // -----------------------------------------------
    GLFWwindow *window = setup("Drone shooter", Player::SCR_HEIGHT, Player::SCR_WIDTH);

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
    Player player; 
    player.setup();
    World world;
    Enemy drone;

    // timing
    // ------
    float currentFrame;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

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
        player.processInput(window, deltaTime);
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

        // draw world objects (ground, trees, flowers and skybox)
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