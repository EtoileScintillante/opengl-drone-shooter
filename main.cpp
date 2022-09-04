/* Work in progress: basic FPS game */

// TODO (from most important to least important)
// ---------------------------------------------
// add mobs (they should only spawn on places where there is no tree) 
// add collision detection (needed: gun direction, fragment position and knowing if fragment belongs to mob) 
// add some spawning mechanism: only one mob per time. When mob is hit, let new one spawn in different position 
// add some small animation when mob is killed (maybe make mob explode?)
// add recoil to gun when space is pressed (space = shoot)
// add score (needed: global variable score (render top left) [text rendering]
// add instructions: movement = w,a,s,d. shoot = space (render top right) [text rendering]
// optionally: limited bullets (so adding a reload function + adding bullet counter)

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "tree.h"
#include "data.h"
#include "ground.h"
#include "glowstone.h"

#include <iostream>
#include <filesystem>
#include <ctime>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void updateGunMovement();
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);

// settings screen
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(25.0f, 0.0f, 25.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool isWalking;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// terrain
const unsigned int N_GLOWSTONES = 6; 
const unsigned int N_TREES = 15;
const unsigned int TERRAIN_SIZE = 50; // actual terrain size = TERRAIN_SIZE * TERRAIN_SIZE (it's a square)
const unsigned int HEIGHT_TREE = 5;
const float BLOCK_SIZE = 1.0f;
const float GROUND_Y = -1.8; // y level of terrain

// handgun 
glm::vec3 gunPosition = glm::vec3(0.45f,-0.5f,-1.5f); // initialize gun position here so we can access it in function outside of main
bool shot;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FPS", NULL, NULL);
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
    Shader blockShader("shaders/blocks.vs", "shaders/blocks.fs");  
    Shader leaveShader("shaders/blocks.vs", "shaders/transparent.fs");
    Shader handGunShader("shaders/model_loading.vs", "shaders/model_loading.fs");
    Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");

    // set up vertex data 
    // ------------------
    float vertices[] = {
    // position           
    -0.5f, -0.5f, -0.5f,    
     0.5f, -0.5f, -0.5f,   
     0.5f,  0.5f, -0.5f,   
     0.5f,  0.5f, -0.5f,   
    -0.5f,  0.5f, -0.5f,   
    -0.5f, -0.5f, -0.5f,   

    -0.5f, -0.5f,  0.5f,    
     0.5f, -0.5f,  0.5f,   
     0.5f,  0.5f,  0.5f,   
     0.5f,  0.5f,  0.5f,   
    -0.5f,  0.5f,  0.5f,   
    -0.5f, -0.5f,  0.5f,   

    -0.5f,  0.5f,  0.5f,   
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  

     0.5f,  0.5f,  0.5f,    
     0.5f,  0.5f, -0.5f,   
     0.5f, -0.5f, -0.5f,   
     0.5f, -0.5f, -0.5f,   
     0.5f, -0.5f,  0.5f,   
     0.5f,  0.5f,  0.5f,   

    -0.5f, -0.5f, -0.5f,   
     0.5f, -0.5f, -0.5f,   
     0.5f, -0.5f,  0.5f,   
     0.5f, -0.5f,  0.5f,   
    -0.5f, -0.5f,  0.5f,   
    -0.5f, -0.5f, -0.5f,   

    -0.5f,  0.5f, -0.5f,   
     0.5f,  0.5f, -0.5f,   
     0.5f,  0.5f,  0.5f,   
     0.5f,  0.5f,  0.5f,   
    -0.5f,  0.5f,  0.5f,   
    -0.5f,  0.5f, -0.5f
    };

    float textureCoords[] = {
    // trunk        // dirt        // leaves/stone  // creeper/zombie
     0.5f, 0.5f,    0.0f, 0.75f,   0.0f, 0.0f,      0.0f, 0.5f, // side    
     1.0f, 0.5f,    0.5f, 0.75f,   1.0f, 0.0f,      0.5f, 0.5f,
     1.0f, 0.72f,   0.5f, 1.0f,    1.0f, 1.0f,      0.5f, 1.0f,
     1.0f, 0.72f,   0.5f, 1.0f,    1.0f, 1.0f,      0.5f, 1.0f,
     0.5f, 0.72f,   0.0f, 1.0f,    0.0f, 1.0f,      0.0f, 1.0f,
     0.5f, 0.5f,    0.0f, 0.75f,   0.0f, 0.0f,      0.0f, 0.5f,

     0.5f, 0.5f,    0.0f, 0.75f,   0.0f, 0.0f,      0.5f, 0.5f, // side
     1.0f, 0.5f,    0.5f, 0.75f,   1.0f, 0.0f,      1.0f, 0.5f,
     1.0f, 0.72f,   0.5f, 1.0f,    1.0f, 1.0f,      1.0f, 1.0f,
     1.0f, 0.72f,   0.5f, 1.0f,    1.0f, 1.0f,      1.0f, 1.0f,
     0.5f, 0.72f,   0.0f, 1.0f,    0.0f, 1.0f,      0.5f, 1.0f,
     0.5f, 0.5f,    0.0f, 0.75f,   0.0f, 0.0f,      0.5f, 0.5f,

     0.5f, 0.72f,   0.0f, 0.75f,   1.0f, 0.0f,      0.5f, 1.0f, // side
     1.0f, 0.72f,   0.5f, 0.75f,   1.0f, 1.0f,      1.0f, 1.0f,
     1.0f, 0.5f,    0.5f, 1.0f,    0.0f, 1.0f,      1.0f, 0.5f,
     1.0f, 0.5f,    0.5f, 1.0f,    0.0f, 1.0f,      1.0f, 0.5f,
     0.5f, 0.5f,    0.0f, 1.0f,    0.0f, 0.0f,      0.5f, 0.5f,
     0.5f, 0.72f,   0.0f, 0.75f,   1.0f, 0.0f,      0.5f, 1.0f,

     0.5f, 0.72f,   0.0f, 0.75f,   1.0f, 0.0f,      0.5f, 1.0f, // side 
     1.0f, 0.72f,   0.5f, 0.75f,   1.0f, 1.0f,      1.0f, 1.0f,
     1.0f, 0.5f,    0.5f, 1.0f,    0.0f, 1.0f,      1.0f, 0.5f,
     1.0f, 0.5f,    0.5f, 1.0f,    0.0f, 1.0f,      1.0f, 0.5f,
     0.5f, 0.5f,    0.0f, 1.0f,    0.0f, 0.0f,      0.5f, 0.5f,
     0.5f, 0.72f,   0.0f, 0.75f,   1.0f, 0.0f,      0.5f, 1.0f,

     0.0f, 0.5f,    0.0f, 0.75f,   0.0f, 1.0f,      0.5f, 1.0f, // bottom
     0.5f, 0.5f,    0.5f, 0.75f,   1.0f, 1.0f,      1.0f, 1.0f,
     0.5f, 0.745f,  0.5f, 0.90f,   1.0f, 0.0f,      1.0f, 0.5f,
     0.5f, 0.745f,  0.5f, 0.90f,   1.0f, 0.0f,      1.0f, 0.5f,
     0.0f, 0.745f,  0.0f, 0.90f,   0.0f, 0.0f,      0.5f, 0.5f,
     0.0f, 0.5f,    0.0f, 0.75f,   0.0f, 1.0f,      0.5f, 1.0f,

     0.0f, 0.5f,    0.52f, 0.75f,  0.0f, 1.0f,      0.5f, 1.0f, // top
     0.5f, 0.5f,    1.0f,  0.75f,  1.0f, 1.0f,      1.0f, 1.0f,
     0.5f, 0.745f,  1.0f,  1.0f,   1.0f, 0.0f,      1.0f, 0.5f,
     0.5f, 0.745f,  1.0f,  1.0f,   1.0f, 0.0f,      1.0f, 0.5f,
     0.0f, 0.745f,  0.52f, 1.0f,   0.0f, 0.0f,      0.5f, 0.5f,
     0.0f, 0.5f,    0.52f, 0.75f,  0.0f, 1.0f,      0.5f, 1.0f
    };

    float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
    };

    // setup vertices for every block
    // ------------------------------
    std::vector< Data > trunkVertices, leavesVertices, dirtVertices, glowStoneVertices, stoneVertices, creeperVertices, zombieVertices;
    int indexPos = 0;
    int indexTex = 0;
    for (unsigned i = 0; i < 36; i++) // 36 lines of position data (all objects, except model(s) are just blocks)
    {   
        Data vertexTrunk, vertexLeaves, vertexDirt, vertexCreeper, vertexZombie;
        // all blocks have the same positon data (leaves, glow stone and stone also have same texture coords data)
        vertexTrunk.Position = vertexLeaves.Position = vertexDirt.Position = vertexCreeper.Position = vertexZombie.Position = glm::vec3(vertices[indexPos],vertices[indexPos+1], vertices[indexPos + 2]); 
        // a line of texture coords data in textureCoords[] looks like: trunk.x, trunk.y, dirt.x, dirt.y, leaves.x, leaves.y (0, 1, 2, 3, 4, 5)
        vertexTrunk.TexCoords = glm::vec2(textureCoords[indexTex], textureCoords[indexTex + 1]);
        vertexLeaves.TexCoords = glm::vec2(textureCoords[indexTex + 4], textureCoords[indexTex + 5]);
        vertexDirt.TexCoords = glm::vec2(textureCoords[indexTex + 2], textureCoords[indexTex + 3]);
        vertexCreeper.TexCoords = glm::vec2(textureCoords[indexTex + 6], textureCoords[indexTex + 7]);
        // zombue tex coords are almost the same as creeper tex coords, the only difference is that each y in zombie tex coords is 0.5f lower than y in creeper tex coords
        vertexZombie.TexCoords = glm::vec2(textureCoords[indexTex + 6], textureCoords[indexTex + 7] - 0.5f); // subtract 0.5f from y value
        // move to the next line in the arrays
        indexPos += 3;
        indexTex += 8; 
        // push data into vectors
        trunkVertices.push_back(vertexTrunk);
        leavesVertices.push_back(vertexLeaves);
        dirtVertices.push_back(vertexDirt);
        glowStoneVertices.push_back(vertexLeaves);
        stoneVertices.push_back(vertexLeaves);
        creeperVertices.push_back(vertexCreeper);
        zombieVertices.push_back(vertexZombie);
    }

    // buffers for skybox
    // ------------------
    unsigned int skyboxVBO; 
    unsigned int skyboxVAO;

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // block textures
    // --------------
    std::string texturePath = "resources/textures/";
    std::string pathTrunkTex = texturePath + "blocks.JPG"; // also for dirt blocks
    std::string pathLeavesTex = texturePath + "leaves.png";
    std::string pathGlowStoneTex = texturePath + "glowstone.jpg";
    std::string pathStoneTex = texturePath + "stone.jpg";

    // initialize block objects
    // ------------------------
    Tree trees(trunkVertices, leavesVertices, pathTrunkTex, pathLeavesTex, N_TREES, HEIGHT_TREE, TERRAIN_SIZE, GROUND_Y, BLOCK_SIZE);
    Ground ground(dirtVertices, stoneVertices, pathTrunkTex, pathStoneTex, TERRAIN_SIZE, GROUND_Y);
    GlowStone glowstones(glowStoneVertices, pathGlowStoneTex, trees.treePositions, N_GLOWSTONES, 3);

    // load skybox texture
    // -------------------
    std::vector<std::string> faces
    {
        std::filesystem::path("resources/skybox/right.jpg"),
        std::filesystem::path("resources/skybox/left.jpg"),
        std::filesystem::path("resources/skybox/top.jpg"),
        std::filesystem::path("resources/skybox/bottom.jpg"),
        std::filesystem::path("resources/skybox/front.jpg"),
        std::filesystem::path("resources/skybox/back.jpg")
    };
    unsigned int skyboxTexture = loadCubemap(faces);

    // skybox shader configuration
    // ---------------------------
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // load model(s)
    // -------------
    Model handGun(std::filesystem::path("resources/models/handgun/Handgun_obj.obj"));

    // camera configuration
    // --------------------
    camera.nonFlying = true; // set to FPS

    // set projection matrix (this one never changes so we do it outside the render loop)
    // ----------------------------------------------------------------------------------
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); 
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        camera.time = currentFrame; // pass time to camera

        // camera movement when standing still (to make the player look more alive)
        if (isWalking == false)
        {
            camera.Position.y = sin(currentFrame*2) * 0.02f;
        } 

        // input
        processInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // view and projection transformations for blockShader
        blockShader.use();
        glm::mat4 view = camera.GetViewMatrix();
        blockShader.setMat4("view", view);
        blockShader.setMat4("projection", projection); 

        // render ground
        ground.Draw(blockShader);

        // render glow stones
        glowstones.Draw(blockShader);

        // view and projection transformations for leaveShader
        leaveShader.use();
        leaveShader.setMat4("view", view);
        leaveShader.setMat4("projection", projection); 

        // render trees
        trees.Draw(blockShader, leaveShader);

        // view and model transformation for handGunShader
        glm::mat4 gunModel = glm::mat4(1.0); 
        gunModel = glm::translate(gunModel, gunPosition); // place gun in bottom right corner
        gunModel = glm::rotate(gunModel, 29.8f, glm::vec3(0.0f, -1.0f, 0.0f)); // rotate gun so it points inwards
        //float angle = glfwGetTime();
        //std::cout << angle << std::endl;
        //gunModel = glm::rotate(gunModel, (float) angle, glm::vec3(0.0f,  0.0f,1.0f)); // rotate gun so it points inwards
        gunModel = glm::scale(gunModel, glm::vec3(0.6f, 0.55f, 0.6f)); // make gun a bit smaller
        handGunShader.setMat4("view", camera.GetViewMatrix()); 
        handGunShader.setMat4("model", glm::inverse(camera.GetViewMatrix()) * gunModel); 
        
        // render gun 
        handGun.drawhandGun(handGunShader);

        if (shot == true)
        {
            handGun.drawSpecificMesh(handGunShader, 5);
        }
        shot = false;

        // before rendering skybox, change depth function so depth test passes when values are equal to depth buffer's content
        glDepthFunc(GL_LEQUAL);  

        // view and projection transformations for skyboxShader
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);

        // render skybox as last (for better perfomance)
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // set depth function back to default
        glDepthFunc(GL_LESS); 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        isWalking = true;
        updateGunMovement();
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        isWalking = true;   
        updateGunMovement();
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        isWalking = true;
        updateGunMovement();
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        isWalking = true;
        updateGunMovement();
        camera.ProcessKeyboard(RIGHT, deltaTime); 
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        shot = true;
    }
    isWalking = false;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    stbi_set_flip_vertically_on_load(true); 
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else {format = GL_RGBA;}

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// adds some extra movement to the gun while the player is walking 
// -----------------------------------------------------------------
void updateGunMovement()
{
    gunPosition.z += sin(glfwGetTime()*10) * 0.004f;
    gunPosition.y += sin(glfwGetTime()*10) * 0.004f;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    stbi_set_flip_vertically_on_load(false); 
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}