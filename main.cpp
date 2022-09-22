/// === Shoot drones! === ///

// TODO:
// fix walking motion of player (need new method, old one does not work)
// fix bounding box of enemy (right now it is not fitting/it is way too small)
// fix floating movement of enemy. Now it looks weird; as if the drone is bouncing
// fix dying animation of enemy: it should not rely on current time! Need new method.

#include "player.h"
#include "enemy.h"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);

// player
Player player; 
float lastX = Player::SCR_WIDTH / 2.0f;
float lastY = Player::SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float currentFrame;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(Player::SCR_WIDTH, Player::SCR_HEIGHT, "FPS", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // set callback functions
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

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
    Enemy drone(world.getTreePositions());

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
        processInput(window);

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

        // draw enemy
        drone.spawn();

        // draw gun and handle gun recoil movement
        player.controlGunRendering();

        // check for collisions
        if (player.shot)
        {
            drone.collisionDetection(player.Position, player.Front, World::TERRAIN_SIZE * 2);
        }

        // if enemy got hit, make it explode
        if (drone.isDead)
        {
            drone.dyingAnimation();
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

/// processes all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly.
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        player.isWalking = true;
        player.ProcessKeyboard(Player::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        player.isWalking = true;
        player.ProcessKeyboard(Player::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        player.isWalking = true;
        player.ProcessKeyboard(Player::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        player.isWalking = true;
        player.ProcessKeyboard(Player::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        player.shot = true;
    }
    if (player.isWalking)
    {
       // add walking motion (find new way to do this)
    }
    player.isWalking = false;
}

/// glfw: whenever the window size changed (by OS or user resize) this callback function executes.
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top.

    lastX = xpos;
    lastY = ypos;

    player.ProcessMouseMovement(xoffset, yoffset);
}