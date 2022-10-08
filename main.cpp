/// === Shoot drones! === ///

// TODO:
// Make the drones dangerous; they should be able to kill the player in some way (maybe they can shoot bullets too?)
// Add scoring system
// Maybe add small random offset to the bullet direction when player shoots gun

#include "player.h"
#include "enemy.h"
#include "glfw_setup.h"
#include "enemy_manager.h"
#include "text_renderer.h"

int main()
{
    // initialize and configure glwf, load OpenGL function pointers and create window
    GLFWwindow *window = setup("Drone shooter", Player::SCR_HEIGHT, Player::SCR_WIDTH);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

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
        // per-frame time logic
        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // pass time variables to player and enemy manager
        player.currentFrame = currentFrame; 
        manager.currentTime = currentFrame;
        manager.deltaTime = deltaTime;

        // input
        player.processInput(window, deltaTime);
        glfwGetCursorPos(window, &player.xPosIn, &player.yPosIn);
        player.ProcessMouseMovement();

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw world objects (ground, trees, flowers and skybox)
        world.view = player.GetViewMatrix();
        world.projection = player.getProjectionMatrix();
        world.Draw();

        // draw gun and handle gun recoil movement
        player.controlGunRendering();
        
        // manage enemy objects
        manager.manage(player, World::TERRAIN_SIZE * 2);
        
        // render text
        text.RenderText("Drone Shooter", 25.0f, 25.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}