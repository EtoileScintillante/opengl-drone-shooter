/// === Shoot drones! === ///

#include "player.h"
#include "enemy.h"
#include "hud.h"
#include "glfw_setup.h"
#include "enemy_manager.h"
#include "collision_detection.h"
#include "text_renderer.h"

int main()
{
    // ask user for the type of environment
    std::cout << "Choose the environment you want to play in (desert, forest, snow, night): ";
    std::string envType;
    std::cin >> envType;

    // initialize and configure glwf, load OpenGL function pointers and create window
    GLFWwindow *window = setup("Drone Shooter", Player::SCR_HEIGHT, Player::SCR_WIDTH);

    // prepare game related objects
    Player player; 
    World world(envType); // will choose env. type randomly if input is invalid
    EnemyManager manager;
    CollisionDetector detector;
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
        if (player.hasStarted && player.getLifeState())
        {
            // pass time variables to player and enemy manager
            player.currentFrame = currentFrame; 
            manager.currentTime = currentFrame;
            manager.deltaTime = deltaTime;

            // keyboard and mouse input
            player.processKeyboardMouse(window, deltaTime);

            // draw the world objects, the gun and the enemies
            world.Draw(player.GetViewMatrix(), player.getProjectionMatrix());
            player.controlPlayerRendering();
            manager.manage(player.Position, player.GetViewMatrix(), player.getProjectionMatrix());

            // handle collisions
            detector.Detect(player, manager);

            // HUD
            inGameScreen(text, player);
        }

        // ending screen
        if (player.hasStarted && (!player.getLifeState()))
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