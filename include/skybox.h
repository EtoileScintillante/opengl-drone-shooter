/*                                  
 * skybox.h
 *
 * Created by: EtoileScintillante
 */

#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <random>
#include <vector>

#include "shader.h"
#include "data.h"
#include "texture_loading.h"

class SkyBox {
public:
    std::vector< float > skyboxVertices;
    std::vector< std::string > filenames;
    std::string dirName;

    /**
     * @brief constructs a new Sky Box object and configures it automatically.
     * 
     * @param skyboxVertices position data for skybox.
     * @param filenames vector containing the filenames of the 6 faces of the skybox. 
     * Filenames must be in this order: +X (right), -X (left), +Y (top), -Y (bottom), +Z (front), -Z (back).
     * @param dirName name of directory which contains the faces.
     */
    SkyBox(std::vector< float > skyboxVertices, std::vector< std::string > filenames, std::string dirName);

    /**
     * @brief draws the skybox
     * 
     * @param shader shader for skybox.
     * @param cameraView camera view matrix.
     * @param projection projection matrix.
     */
    void Draw(Shader &shader, glm::mat4 cameraView, glm::mat4 projection);

private:
    unsigned int skyboxTexture = 8;
    unsigned int VBO, VAO;

    /// sets up buffers 
    void configureSkybox();
};

#endif /*__SKYBOX__*/