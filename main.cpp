/// === Shoot drones! === ///

#include "game_state.h"
#include "player.h"
#include "world.h"
#include "enemy.h"
#include "hud.h"
#include "glfw_setup.h"
#include "enemy_manager.h"
#include "collision_detection.h"
#include "text_renderer.h"

int main()
{
    // initialize and configure glfw, load OpenGL function pointers and create window
    GLFWwindow *window = setup("Drone Shooter", Player::SCR_HEIGHT, Player::SCR_WIDTH);

    // prepare game related objects
    Player player;
    World world; // loaded later, after player selects environment on start screen
    EnemyManager manager;
    CollisionDetector detector;
    TextRenderer text("resources/font/theboldfont.ttf", "shaders/text.vert", "shaders/text.frag");

    // game state
    GameState state = GameState::START;

    // environment selection
    const std::string envNames[4] = {"desert", "forest", "snow", "night"};
    int selectedEnv = 0;
    bool wWasPressed = false;
    bool sWasPressed = false;
    bool enterWasPressed = false;

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

        // ESC always closes the window
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        bool enterNow = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;

        switch (state)
        {
            case GameState::START:
            {
                // navigate env selector with edge detection to avoid skipping entries
                bool wNow = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
                bool sNow = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
                if (wNow && !wWasPressed) selectedEnv = (selectedEnv - 1 + 4) % 4;
                if (sNow && !sWasPressed) selectedEnv = (selectedEnv + 1) % 4;
                wWasPressed = wNow;
                sWasPressed = sNow;

                startingScreen(text, player, selectedEnv);

                if (enterNow && !enterWasPressed)
                {
                    world.load(envNames[selectedEnv]);
                    state = GameState::PLAYING;
                }
                break;
            }

            case GameState::PLAYING:
                // pass timing to objects that need it
                player.currentFrame = currentFrame;
                manager.currentTime = currentFrame;
                manager.deltaTime   = deltaTime;

                // input, world, player, enemies, collisions, HUD
                player.processKeyboardMouse(window, deltaTime);
                world.Draw(player.GetViewMatrix(), player.getProjectionMatrix());
                player.controlPlayerRendering();
                manager.manage(player.Position, player.GetViewMatrix(), player.getProjectionMatrix());
                detector.Detect(player, manager);
                inGameScreen(text, player);

                // transition when player dies
                if (!player.getLifeState())
                {
                    manager.reset();
                    state = GameState::GAME_OVER;
                }
                break;

            case GameState::GAME_OVER:
                endingScreen(text, player);
                if (enterNow && !enterWasPressed)
                {
                    player.resetAll();
                    state = GameState::START;
                }
                break;
        }

        enterWasPressed = enterNow;

        // glfw: swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
