/* Shoot zombie and creeper heads */

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "tree.h"
#include "data.h"
#include "ground.h"
#include "glowstone.h"
#include "gun.h"
#include "vertex_data.h"
#include "texture_loading.h"
#include "skybox.h"
#include "mobs.h"
#include "box.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void setupData();

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

// terrain
const unsigned int N_GLOWSTONES = 6;  // number of glowstones
const unsigned int N_TREES = 12;      // number of trees
const unsigned int TERRAIN_SIZE = 30; // actual terrain size = TERRAIN_SIZE * TERRAIN_SIZE (it's a square)
const unsigned int HEIGHT_TREE = 5;   // height of tree (amount of blocks that make up a trunk)
const float HEIGHT_GLOWSTONES = 3.0;  // height of glow stone (counted from GROUND_Y + BLOCK_SIZE)
const float BLOCK_SIZE = 1.0f;        // all blocks are this size
const float GROUND_Y = -1.8f;         // y level of ground 

// vectors containg all the data needed for rendering the blocks
std::vector<Data> trunkVertices, leavesVertices, dirtVertices, glowStoneVertices, stoneVertices, creeperVertices, zombieVertices;

// handgun
glm::vec3 gunPosition = glm::vec3(0.45f, -0.5f, -1.5f); // (base) position for gun
const float BASE_ROTATION = 95.0f;                      // y axis rotation to make gun point slightly inwards
const float SCALE_FACTOR = 0.6f;                        // make gun smaller
const float GUN_RANGE = TERRAIN_SIZE * 2;               // how far the bullet can travel
bool shot = false;                                      // has player taken a shot? (pressed space)
bool startRecoil;                                       // start recoil animation?
bool goDown = false;                                    // needed for recoil animation --> gun needs to move down if true
float angle = 0;                                        // needed for recoil animation, this angle will be updated every frame to make the gun rotate up and then down

// mobs
const float MIN_HEIGHT = 2.0f; // minimum floating height
const float MAX_HEIGHT = 6.0f; // maximum floating height

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

    // build and compile shaders
    // -------------------------
    Shader blockShader("shaders/blocks.vert", "shaders/blocks.frag");
    Shader leaveShader("shaders/blocks.vert", "shaders/transparent.frag");
    Shader handGunShader("shaders/model_loading.vert", "shaders/model_loading.frag");
    Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");

    // initialize block objects
    // ------------------------
    setupData();

    std::string texturePath = "resources/textures/";
    std::string pathTrunkDirtTex = texturePath + "blocks.JPG";
    std::string pathLeavesTex = texturePath + "leaves.png";
    std::string pathGlowStoneTex = texturePath + "glowstone.jpg";
    std::string pathStoneTex = texturePath + "stone.jpg";
    std::string pathMobsTex = texturePath + "mobs.JPG";

    Tree trees(trunkVertices, leavesVertices, pathTrunkDirtTex, pathLeavesTex, N_TREES, HEIGHT_TREE, TERRAIN_SIZE, GROUND_Y, BLOCK_SIZE);
    Ground ground(dirtVertices, stoneVertices, pathTrunkDirtTex, pathStoneTex, TERRAIN_SIZE, GROUND_Y);
    GlowStone glowstones(glowStoneVertices, pathGlowStoneTex, trees.treePositions, N_GLOWSTONES, HEIGHT_GLOWSTONES);
    Mobs mobs(zombieVertices, creeperVertices, trees.treePositions, pathMobsTex, TERRAIN_SIZE, GROUND_Y, BLOCK_SIZE, MIN_HEIGHT, MAX_HEIGHT);

    // initialize skybox object
    // ------------------------
    std::vector<float> skyboxVertices = getSkyboxPositionData();
    std::vector<std::string> filenames = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"};
    std::string dirName = "resources/skybox";
    SkyBox skybox(skyboxVertices, filenames, dirName);

    // load handgun model
    // ------------------
    Model handGun("resources/models/handgun/Handgun_obj.obj");

    // camera configuration
    // --------------------
    camera.FPS = true;
    camera.bottomLimitX = 0.0f;
    camera.bottomLimitZ = 0.0f;
    camera.upperLimitX = TERRAIN_SIZE;
    camera.upperLimitZ = TERRAIN_SIZE;

    // set projection matrix
    // ---------------------
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

        // render ground
        ground.Draw(blockShader, camera.GetViewMatrix(), projection);

        // render glow stones
        glowstones.Draw(blockShader, camera.GetViewMatrix(), projection);

        // render trees
        trees.Draw(blockShader, leaveShader, camera.GetViewMatrix(), projection);

        // render a mob
        mobs.Spawn(blockShader, currentFrame, camera.GetViewMatrix(), projection);

        // view and model transformation for handGunShader
        glm::mat4 gunModel = getGunModelMatrix(gunPosition, camera.GetViewMatrix(), SCALE_FACTOR, BASE_ROTATION); // initialize gun model matrix
        handGunShader.setMat4("view", camera.GetViewMatrix());
        handGunShader.setMat4("model", gunModel);

        // render gun in base position
        if (!shot)
        {
            drawhandGun(handGun, handGunShader);
        }

        // draw the gunfire (only for one frame, otherwise the gunfire is visible for too long, which just looks weird)
        if (shot == true && startRecoil == false)
        {
            handGun.drawSpecificMesh(handGunShader, 5);
            startRecoil = true;
            mobs.collisionDetection(camera.Position, camera.Front, GUN_RANGE);
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
                goBackToBase(handGunShader, gunModel, angle, shot, goDown, startRecoil);  // start moving down
            }                                   
            drawhandGun(handGun, handGunShader); // render upwards or downwards rotating gun
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        isWalking = true;
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        isWalking = true;
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        isWalking = true;
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        isWalking = true;
        camera.ProcessKeyboard(RIGHT, deltaTime);
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// setup vertex data for all the blocks
// ------------------------------------
void setupData()
{
    std::vector<float> positionData = getPositionData();       // position data for blocks
    std::vector<float> textureCoords = getTextureCoordsData(); // texture coordinates for blocks

    int indexPos = 0;
    int indexTex = 0;
    for (unsigned i = 0; i < getPositionData().size(); i++) // 36 lines of position data (all objects, except model(s) are just blocks)
    {
        Data vertexTrunk, vertexLeaves, vertexDirt, vertexCreeper, vertexZombie;
        // all blocks have the same positon data (leaves, glow stone and stone also have same texture coords data)
        vertexTrunk.Position = vertexLeaves.Position = vertexDirt.Position = vertexCreeper.Position = vertexZombie.Position = glm::vec3(positionData[indexPos], positionData[indexPos + 1], positionData[indexPos + 2]);
        // a line of texture coords data in textureCoords[] looks like: trunk.x, trunk.y, dirt.x, dirt.y, leaves.x, leaves.y (0, 1, 2, 3, 4, 5)
        vertexTrunk.TexCoords = glm::vec2(textureCoords[indexTex], textureCoords[indexTex + 1]);
        vertexLeaves.TexCoords = glm::vec2(textureCoords[indexTex + 4], textureCoords[indexTex + 5]);
        vertexDirt.TexCoords = glm::vec2(textureCoords[indexTex + 2], textureCoords[indexTex + 3]);
        vertexCreeper.TexCoords = glm::vec2(textureCoords[indexTex + 6], textureCoords[indexTex + 7]);
        // zombie tex coords are almost the same as creeper tex coords, the only difference is that each y in zombie tex coords is 0.5f lower than y in creeper tex coords
        vertexZombie.TexCoords = glm::vec2(textureCoords[indexTex + 6], textureCoords[indexTex + 7] - 0.5f); // subtract 0.5f from y value
        // push data into vectors
        trunkVertices.push_back(vertexTrunk);
        leavesVertices.push_back(vertexLeaves);
        dirtVertices.push_back(vertexDirt);
        glowStoneVertices.push_back(vertexLeaves);
        stoneVertices.push_back(vertexLeaves);
        creeperVertices.push_back(vertexCreeper);
        zombieVertices.push_back(vertexZombie);
        // move to the next line in the arrays
        indexPos += 3;
        indexTex += 8;
    }
}