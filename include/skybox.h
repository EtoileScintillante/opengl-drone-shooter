/*                                  
 * skybox.h
 *
 * This file contains a SkyBox class, to load and render a skybox/cubemap.
 * 
 * Created by EtoileScintillante.
 */

#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include "shader.h"
#include "texture_loading.h"

class SkyBox {
public:
    std::vector< float > skyboxVertices;
    std::vector< std::string > filenames;
    std::string dirName;

    /// Default constructor.
    SkyBox();

    /**
     * @brief Constructs a new Sky Box object and configures it automatically.
     * 
     * @param filenames vector containing the filenames of the 6 faces of the skybox. 
     * Filenames must be in this order: +X (right), -X (left), +Y (top), -Y (bottom), +Z (front), -Z (back).
     * @param dirName name of directory which contains the faces.
     */
    SkyBox(std::vector< std::string > filenames, std::string dirName);

    /**
     * @brief Draws the skybox.
     * 
     * @param shader shader for skybox.
     * @param cameraView camera view matrix.
     * @param projection projection matrix.
     */
    void Draw(Shader &shader, glm::mat4 cameraView, glm::mat4 projection);

private:
    unsigned int skyboxTexture;
    unsigned int VBO, VAO;

    /// Sets up buffers.
    void configureSkybox();

    /// Get skybox vertices.
    std::vector<float> getSkyboxVertexData();
};

#endif /*__SKYBOX__*/