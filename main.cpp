/// === Shoot drones! === ///

// TODO:
// Make the drones dangerous; they should be able to hit the player (maybe they can shoot bullets too?)
// and when player gets hit, decrease player's health. Is health 0? Player is killed --> show ending screen 
// Add ending screen ("You died. Press [key] to play again or ESC to quit")

#include "player.h"
#include "enemy.h"
#include "hud.h"
#include "glfw_setup.h"
#include "enemy_manager.h"
#include "text_renderer.h"

int main()
{
    // initialize and configure glwf, load OpenGL function pointers and create window
    GLFWwindow *window = setup("Drone shooter", Player::SCR_HEIGHT, Player::SCR_WIDTH);

    // prepare game related objects
    Player player; 
    World world;
    EnemyManager manager;
    TextRenderer text("resources/font/theboldfont.ttf", "shaders/text.vert", "shaders/text.frag");

    // timing
    float currentFrame;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // title/start screen
        if (!player.hasStarted)
        {
            // load starting and wait for keyboard input (enter = start game)
            startingScreen(text, player, world);
            player.processInput(window, deltaTime); 
        }
    
        // game started
        if (player.hasStarted)
        {
            // per-frame time logic
            currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // pass time variables to player and enemy manager
            player.currentFrame = currentFrame; 
            manager.currentTime = currentFrame;
            manager.deltaTime = deltaTime;

            // keyboard and mouse input
            player.processInput(window, deltaTime);
            glfwGetCursorPos(window, &player.xPosIn, &player.yPosIn);
            player.ProcessMouseMovement();

            // draw world objects (ground, trees, flowers and skybox)
            world.view = player.GetViewMatrix();
            world.projection = player.getProjectionMatrix();
            world.Draw();

            // draw gun and handle gun recoil movement
            player.controlGunRendering();
            
            // manage enemy objects
            manager.manage(player, World::TERRAIN_SIZE * 2);

            // HUD
            inGameScreen(text, player);
        }

        // ending screen
        // TODO

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}