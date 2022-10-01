/// === Shoot drones! === ///

// TODO:
// Make the drones dangerous; they should be able to kill the player in some way (maybe they can shoot bullets too?)
// Improve sound effect of drone (the closer the player is to the drone, the louder the hovering and explosion sound)
// Add walking sound for player
// Add scoring system
// Maybe add small random offset to the bullet direction when player shoots gun

#include "player.h"
#include "enemy.h"
#include "glfw_setup.h"

int main()
{
    // initialize and configure glwf, load OpenGL function pointers and create window
    GLFWwindow *window = setup("Drone shooter", Player::SCR_HEIGHT, Player::SCR_WIDTH);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // prepare game related objects
    Player player; 
    player.setup();
    World world;
    Enemy drone;

    // timing
    float currentFrame;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // pass time variables to player and enemy
        player.currentFrame = currentFrame; 
        drone.currentFrame = currentFrame; 
        drone.deltaTime = deltaTime;

        // pass player position to enemy, needed to control enemy soundeffects
        drone.playerPosition = player.Position; 

        // input
        player.processInput(window, deltaTime);
        glfwGetCursorPos(window, &player.xPosIn, &player.yPosIn);
        player.ProcessMouseMovement();

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // view and projection for world objects and enemy
        world.projection = player.getProjectionMatrix();
        world.view = player.GetViewMatrix();
        drone.projection = player.getProjectionMatrix();
        drone.view = player.GetViewMatrix();

        // draw world objects (ground, trees, flowers and skybox)
        world.Draw();

        // draw gun and handle gun recoil movement
        player.controlGunRendering();
        
        // control life of enemy (spawning and dying)
        drone.controlEnemyLife(player.shot, player.Position, player.Front, World::TERRAIN_SIZE * 2);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}