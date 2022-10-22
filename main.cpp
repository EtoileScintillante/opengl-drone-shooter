/// === Shoot drones! === ///

#include "player.h"
#include "enemy.h"
#include "hud.h"
#include "glfw_setup.h"
#include "enemy_manager.h"
#include "text_renderer.h"

int main()
{
    // initialize and configure glwf, load OpenGL function pointers and create window
    GLFWwindow *window = setup("Drone Shooter", Player::SCR_HEIGHT, Player::SCR_WIDTH);

    // prepare game related objects
    Player player; 
    World world;
    EnemyManager manager;
    TextRenderer text("resources/font/theboldfont.ttf", "shaders/text.vert", "shaders/text.frag");

    // timing
    float currentFrame;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // aim assist
    bool aimAssist = true;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // per-frame time logic
        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        text.deltaTime = deltaTime;

        // title/start screen
        if (!player.hasStarted)
        {
            startingScreen(text, player, world);
            player.processKeyboardMouse(window, deltaTime); // pressing enter starts the game
        }
    
        // game started
        if (player.hasStarted && player.isAlive)
        {
            // pass time variables to player and enemy manager
            player.currentFrame = currentFrame; 
            manager.currentTime = currentFrame;
            manager.deltaTime = deltaTime;

            // keyboard and mouse input
            player.processKeyboardMouse(window, deltaTime);

            // draw the world objects, the gun and the enemies
            world.Draw(player.GetViewMatrix(), player.getProjectionMatrix());
            player.controlGunRendering();
            manager.manage(player, World::TERRAIN_SIZE * 2);

            // HUD
            inGameScreen(text, player, aimAssist);
        }

        // ending screen
        if (player.hasStarted && (!player.isAlive))
        {
            manager.reset(); // reset enemies in case player restarts the game
            endingScreen(text, player, world);
            player.processKeyboardMouse(window, deltaTime); // pressing enter restarts the game
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}