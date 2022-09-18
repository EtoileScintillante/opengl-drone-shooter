/* Shoot zombie and creeper heads */

// TODO: maybe look for another way to organize the objects
// because all of the constructors need a lot of arguments

#include "camera.h"
#include "model.h"
#include "gun.h"
#include "skybox.h"
#include "box.h"
#include "world.h"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);

// settings screen
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(15.0f, 0.0f, 15.0f)); // starting position of camera (player)
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool isWalking;

// timing
float currentFrame;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// handgun
glm::vec3 gunPosition = glm::vec3(0.45f, -0.5f, -1.5f); // (base) position for gun
bool shot = false;                                      // has player taken a shot? (pressed space)
bool startRecoil;                                       // start recoil animation?
bool goDown = false;                                    // needed for recoil animation --> gun needs to move down if true
float angle = 0;                                        // needed for recoil animation, this angle will be updated every frame to make the gun rotate up and then down

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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FPS", NULL, NULL);
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
    // build and compile shaders for handgun and skybox
    Shader handGunShader("shaders/model_loading.vert", "shaders/model_loading.frag");
    Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");
    Shader skullShader("shaders/model_loading.vert", "shaders/model_loading.frag");

    // initialize world object (includes ground, trees, glow stones and mobs)
    World world;

    // initialize skybox object
    std::vector<float> skyboxVertices = getSkyboxPositionData();
    std::vector<std::string> filenames = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"};
    std::string dirName = "resources/skybox";
    SkyBox skybox(skyboxVertices, filenames, dirName);

    // load handgun and skull model
    Model handGun("resources/models/handgun/Handgun_obj.obj");
    Model skull("resources/models/skull/skull.obj");

    // camera configuration
    camera.FPS = true;
    camera.bottomLimitX = 0.0f;
    camera.bottomLimitZ = 0.0f;
    camera.upperLimitX = World::TERRAIN_SIZE;
    camera.upperLimitZ = World::TERRAIN_SIZE;

    // set projection matrix (this does not change so we set it outside of the render loop)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        camera.currentFrame = currentFrame; // pass time to camera

        // camera movement when player is not moving (to make the player look more alive)
        camera.passiveMotion(isWalking);

        // input
        processInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw the world (ground + trees + glow stones + mobs)
        world.Draw(camera.GetViewMatrix(), projection, currentFrame);

        // view and model transformation for handgun
        glm::mat4 gunModel = getGunModelMatrix(gunPosition, camera.GetViewMatrix(), 0.6f, 95.0f); 
        handGunShader.setMat4("view", camera.GetViewMatrix());
        handGunShader.setMat4("model", gunModel);

        // draw the handgun in base position
        if (!shot)
        {
            drawhandGun(handGun, handGunShader);
        }

        // draw the gunfire (only for one frame, otherwise the gunfire is visible for too long, which just looks weird)
        if (shot && !startRecoil)
        {
            handGun.drawSpecificMesh(handGunShader, 5);
            startRecoil = true;
            world.mobs.collisionDetection(camera.Position, camera.Front, world.TERRAIN_SIZE * 2);
        }

        // start the recoil animation
        if (startRecoil)
        {
            if (!goDown)
            {
                startRecoilAnimation(handGunShader, gunModel, angle, goDown); // start moving up
            }
            else
            {
                goBackToBase(handGunShader, gunModel, angle, shot, goDown, startRecoil); // start moving down
            }
            drawhandGun(handGun, handGunShader); // render rotating handgun
        }

        // render skull when a mob has been killed
        if (world.mobs.hasDied)
        {
            world.mobs.died(skull, skullShader, camera.GetViewMatrix(), projection, deltaTime, currentFrame);
        }

        // render skybox
        glDepthFunc(GL_LEQUAL);
        skybox.Draw(skyboxShader, camera.GetViewMatrix(), projection);
        glDepthFunc(GL_LESS);

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
        isWalking = true;
        camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        isWalking = true;
        camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        isWalking = true;
        camera.ProcessKeyboard(Camera::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        isWalking = true;
        camera.ProcessKeyboard(Camera::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        shot = true;
    }
    if (isWalking)
    {
        walkingMotion(gunPosition.y, gunPosition.z, currentFrame);
    }
    isWalking = false;
}

/// glfw: whenever the window size changed (by OS or user resize) this callback function executes.
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}