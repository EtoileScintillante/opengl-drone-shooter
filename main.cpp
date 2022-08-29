/* Work in progress: basic FPS game */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"

#include <iostream>
#include <filesystem>
#include <random>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void updateGunMovement();
unsigned int loadTexture(const char *path);

// settings screen
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool isWalking;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// terrain
const unsigned int NUM_GLOWSTONES = 4; // must be equal to NR_POINT_LIGHTS in multiple_lights.fs (glow stones are just textured point lights)
const unsigned int NUM_TREES = 15;
int terrainSize = 25; // the for loop in which we generate the terrain goes from -terrainSize to terrainSize
int heightTree = 5;
float dirtY = -1.8; // y level of dirt blocks 

// gun 
glm::vec3 gunPosition = glm::vec3(0.45f,-0.5f,-1.5f); // initialize gun position here so we can access it in function outside of main

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
    glfwSetScrollCallback(window, scroll_callback);

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
    Shader lightingShader("shaders/lighting_map.vs", "shaders/multiple_lights.fs"); 
    Shader glowStoneShader("shaders/light_cube_textured.vs", "shaders/light_cube_textured.fs"); 
    Shader handGunShader("shaders/model_loading.vs", "shaders/model_loading.fs");

    // load model
    // ----------
    Model handGun(std::filesystem::path("resources/models/handgun/Handgun_obj.obj"));

    // set up vertex data 
    // ------------------
    float vertices[] = {
    // position            // normals
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f, 
     0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,  1.0f, 
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, 
    -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  0.0f, 
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,   0.0f,-1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,   0.0f,-1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,   0.0f,-1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,   0.0f,-1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f,-1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f,-1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  0.0f
    };

    float textureCoords[] = {
     // wood        // dirt        // leaves     
     0.5f, 0.5f,    0.0f, 0.75f,   0.0f, 0.0f,  // side    
     1.0f, 0.5f,    0.5f, 0.75f,   1.0f, 0.0f,   
     1.0f, 0.72f,   0.5f, 1.0f,    1.0f, 1.0f,   
     1.0f, 0.72f,   0.5f, 1.0f,    1.0f, 1.0f,   
     0.5f, 0.72f,   0.0f, 1.0f,    0.0f, 1.0f,   
     0.5f, 0.5f,    0.0f, 0.75f,   0.0f, 0.0f,   

     0.5f, 0.5f,    0.0f, 0.75f,   0.0f, 0.0f,  // side
     1.0f, 0.5f,    0.5f, 0.75f,   1.0f, 0.0f,   
     1.0f, 0.72f,   0.5f, 1.0f,    1.0f, 1.0f,   
     1.0f, 0.72f,   0.5f, 1.0f,    1.0f, 1.0f,   
     0.5f, 0.72f,   0.0f, 1.0f,    0.0f, 1.0f,  
     0.5f, 0.5f,    0.0f, 0.75f,   0.0f, 0.0f,   

     0.5f, 0.72f,   0.0f, 0.75f,   1.0f, 0.0f,  // side
     1.0f, 0.72f,   0.5f, 0.75f,   1.0f, 1.0f,   
     1.0f, 0.5f,    0.5f, 1.0f,    0.0f, 1.0f,   
     1.0f, 0.5f,    0.5f, 1.0f,    0.0f, 1.0f,   
     0.5f, 0.5f,    0.0f, 1.0f,    0.0f, 0.0f,   
     0.5f, 0.72f,   0.0f, 0.75f,   1.0f, 0.0f,   

     0.5f, 0.72f,   0.0f, 0.75f,   1.0f, 0.0f,  // side 
     1.0f, 0.72f,   0.5f, 0.75f,   1.0f, 1.0f,   
     1.0f, 0.5f,    0.5f, 1.0f,    0.0f, 1.0f,   
     1.0f, 0.5f,    0.5f, 1.0f,    0.0f, 1.0f,   
     0.5f, 0.5f,    0.0f, 1.0f,    0.0f, 0.0f,   
     0.5f, 0.72f,   0.0f, 0.75f,   1.0f, 0.0f,   

     0.0f, 0.5f,    0.0f, 0.75f,   0.0f, 1.0f,  // bottom
     0.5f, 0.5f,    0.5f, 0.75f,   1.0f, 1.0f,   
     0.5f, 0.745f,  0.5f, 0.90f,   1.0f, 0.0f,   
     0.5f, 0.745f,  0.5f, 0.90f,   1.0f, 0.0f,   
     0.0f, 0.745f,  0.0f, 0.90f,   0.0f, 0.0f,   
     0.0f, 0.5f,    0.0f, 0.75f,   0.0f, 1.0f,   

     0.0f, 0.5f,    0.52f, 0.75f,  0.0f, 1.0f,  // top
     0.5f, 0.5f,    1.0f,  0.75f,  1.0f, 1.0f,   
     0.5f, 0.745f,  1.0f,  1.0f,   1.0f, 0.0f,   
     0.5f, 0.745f,  1.0f,  1.0f,   1.0f, 0.0f,   
     0.0f, 0.745f,  0.52f, 1.0f,   0.0f, 0.0f,   
     0.0f, 0.5f,    0.52f, 0.75f,  0.0f, 1.0f  
    };

    // set up position data for objects
    // --------------------------------
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    // create random positions for the tree trunks (made out of the wooden blocks)
    glm::vec3 trunkPositions[NUM_TREES];
    std::uniform_int_distribution<> xzPlane(-20, 20); // define the range for x and z axis (make sure lower and upper range are not higher tha terrainSize)
    for (unsigned int i = 0; i < 15; i++)
    {
        float posX = xzPlane(gen); 
        float posZ = xzPlane(gen);
        glm::vec3 vec = {posX, dirtY + 1.0f, posZ}; // we'll render the lowest block of tree trunk at posY + 1.0 so that it is placed on the ground (1.0 is blocksize)
        trunkPositions[i] = vec;
    }

    // positions of the glow stones (sticking to a tree trunk) 
    std::uniform_int_distribution<> lights(0, NUM_TREES); // generate random number n between 0 and NUM_TREES correspondng the nth tree trunk intrunkPositions[]
    int treeIndexes[NUM_GLOWSTONES]; // the trees with their position index in this array will be getting a glow stone attached to them
    for (unsigned int i = 0; i < 4; i++)
    {
        int n = lights(gen); 
        treeIndexes[i] = n;
    }
    glm::vec3 glowStonePositions[NUM_GLOWSTONES]; 
    for (int i = 0; i < NUM_GLOWSTONES; i++)
    {
        // to position the glow stone to make it look like it sticks to the side of the tree, we lower or increase x by 0.5 and we add 2.0 to y
        glm::vec3 vec = {trunkPositions[i].x - 0.5f, trunkPositions[i].y + 2.0f, trunkPositions[i].z};
        if (i % 3 == 0) {vec = {trunkPositions[i].x + 0.5f, trunkPositions[i].y + 2.0f, trunkPositions[i].z};} // to add some variation in the direction the glow stones are pointing to
        glowStonePositions[i] = vec;
    }

    // positions of leaves blocks (every tree has 5 leaves blocks attached to it)
    glm::vec3 leavesPositions[NUM_TREES*5];
    int indexCount = 0; // 5 leaves per tree, so every iteration we need to move 5 spots in the array to get every leaves block in
    for (unsigned int i = 0; i < NUM_TREES; i++)
    {
        glm::vec3 leavesBlock1 = {trunkPositions[i].x - 1.0f, dirtY + heightTree, trunkPositions[i].z};
        leavesPositions[indexCount] = leavesBlock1;
        glm::vec3 leavesBlock2 = {trunkPositions[i].x + 1.0f, dirtY + heightTree,trunkPositions[i].z};
        leavesPositions[indexCount + 1] = leavesBlock2;
        glm::vec3 leavesBlock3 = {trunkPositions[i].x, dirtY + heightTree,trunkPositions[i].z - 1.0f};
        leavesPositions[indexCount + 2] = leavesBlock3;
        glm::vec3 leavesBlock4 = {trunkPositions[i].x, dirtY + heightTree,trunkPositions[i].z + 1.0f};
        leavesPositions[indexCount + 3] = leavesBlock4;
        glm::vec3 leavesBlock5 = {trunkPositions[i].x, dirtY + heightTree + 1.0,trunkPositions[i].z};
        leavesPositions[indexCount + 4] = leavesBlock5;
        indexCount += 5;
    }

    // buffers
    // -------
    unsigned int posNormVBO, texCoordVBO; 
    unsigned int dirtVAO, woodVAO, leavesVAO, glowStoneVAO;

    // configure dirt block VAO 
    // ------------------------
    glGenVertexArrays(1, &dirtVAO);
    glBindVertexArray(dirtVAO);
    glGenBuffers(1, &posNormVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posNormVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // texture coord
    glGenBuffers(1, &texCoordVBO); 
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float))); 
    glEnableVertexAttribArray(1);

    // configure wood block VAO
    // ------------------------
    glGenVertexArrays(1, &woodVAO);
    glBindVertexArray(woodVAO);
    glBindBuffer(GL_ARRAY_BUFFER, posNormVBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // texture coord
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(1);

    // configure leaves block VAO
    // --------------------------
    glGenVertexArrays(1, &leavesVAO);
    glBindVertexArray(leavesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, posNormVBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // texture coord
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float))); 
    glEnableVertexAttribArray(1);

    // configure the glow stones VAO 
    // ------------------------------
    glGenVertexArrays(1, &glowStoneVAO);
    glBindVertexArray(glowStoneVAO);
    glBindBuffer(GL_ARRAY_BUFFER, posNormVBO);
    // positions 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord (same coordinates as leaves block)
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float))); 
    glEnableVertexAttribArray(1);

    // load textures 
    // -------------
    unsigned int diffuseMapDW = loadTexture(std::filesystem::path("resources/textures/blocks.JPG").c_str()); // dirt blocks and wooden blocks
    unsigned int diffuseMapL = loadTexture(std::filesystem::path("resources/textures/leaves.png").c_str()); // leaves
    unsigned int diffuseMapGS = loadTexture(std::filesystem::path("resources/textures/glowstone.jpg").c_str()); // glow stone (lamp texture)

    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0); // there is no specular or emission texture

    // camera configuration
    // --------------------
    camera.nonFlying = true; // FPS
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
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
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader before setting uniforms and drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);  

        // directional light (from above, basically like the sun)
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.03f, 0.03f, 0.03f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        // glow stones (they only differ in position)
        for (int i = 0; i < NUM_GLOWSTONES; i++)
        {
            std::string number = std::to_string(i);
            lightingShader.setVec3("pointLights[" + number +"].position", glowStonePositions[i]);
            lightingShader.setVec3("pointLights[" + number +"].ambient", 0.05f, 0.05f, 0.05f);
            lightingShader.setVec3("pointLights[" + number +"].diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader.setVec3("pointLights[" + number +"].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("pointLights[" + number +"].constant", 1.0f);
            lightingShader.setFloat("pointLights[" + number +"].linear", 0.09f);
            lightingShader.setFloat("pointLights[" + number +"].quadratic", 0.032f);
        } 
        
        // view/projection transformations
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); 
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection); 

        // bind diffuse map for wooden blocks and dirt blocks
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMapDW);

        // render dirt blocks
        glBindVertexArray(dirtVAO);
        for (int i = -terrainSize; i < terrainSize; i++)
        {
           for (int j = -terrainSize; j < terrainSize; j++)
           {
                glm::mat4 modelDirt = glm::mat4(1.0f);
                modelDirt = glm::translate(modelDirt, glm::vec3(i, dirtY, j));
                lightingShader.setMat4("model", modelDirt);
                glDrawArrays(GL_TRIANGLES, 0, 36);
           }
        }

        // render tree trunks
        glBindVertexArray(woodVAO);
        for (unsigned int i = 0; i < NUM_TREES; i++)
        {
            glm::vec3 trunk =trunkPositions[i];
            for (unsigned int j = 0; j < heightTree; j++) 
            {
                glm::mat4 modelTrunk = glm::mat4(1.0f);
                modelTrunk = glm::translate(modelTrunk, trunk);
                lightingShader.setMat4("model", modelTrunk);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                trunk.y++; // increase y with 1 to place next wooden block exactly on top of the one before that
            }
        }

        // bind diffuse map for leaves blocks
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMapL);

        // render leaves
        glBindVertexArray(leavesVAO);
        for (unsigned int i = 0; i < 75; i++)
        {
            glm::mat4 modelLeaves = glm::mat4(1.0f);
            modelLeaves = glm::translate(modelLeaves, leavesPositions[i]);
            lightingShader.setMat4("model", modelLeaves);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // bind diffuse map for glow stones
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMapGS);

        // render glow stones
        glowStoneShader.use();
        glowStoneShader.setMat4("view", view);
        glowStoneShader.setMat4("projection", projection);
    
        glBindVertexArray(glowStoneVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            glm::mat4 modelGS = glm::mat4(1.0f);
            modelGS = glm::translate(modelGS, glowStonePositions[i]);
            modelGS = glm::scale(modelGS, glm::vec3(0.5f)); // make it a smaller cube
            glowStoneShader.setMat4("model", modelGS);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // render gun (made up of 4 meshes)
        glm::mat4 gunModel = glm::mat4(1.0); 
        gunModel = glm::translate(gunModel, gunPosition); // place gun in bottom right corner
        gunModel = glm::rotate(gunModel, 29.8f, glm::vec3(0.0f, -1.0f, 0.0f)); // rotate gun so it points inwards
        gunModel = glm::scale(gunModel, glm::vec3(0.6f, 0.55f, 0.6f)); // make gun a bit smaller
        handGunShader.setMat4("view", camera.GetViewMatrix()); 
        handGunShader.setMat4("model", glm::inverse(camera.GetViewMatrix()) * gunModel); 
        handGun.DrawSpecificMesh(handGunShader, 1);
        handGun.DrawSpecificMesh(handGunShader, 3);
        handGun.DrawSpecificMesh(handGunShader, 4);
        handGun.DrawSpecificMesh(handGunShader, 6);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &dirtVAO);
    glDeleteVertexArrays(1, &woodVAO);
    glDeleteVertexArrays(1, &glowStoneVAO);
    glDeleteBuffers(1, &posNormVBO);
    glDeleteBuffers(1, &texCoordVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

// adds some extra movement to the gun while the 'player' is walking 
// -----------------------------------------------------------------
void updateGunMovement()
{
    gunPosition.z += sin(glfwGetTime()*10) * 0.004f;
    gunPosition.y += sin(glfwGetTime()*10) * 0.004f;
}