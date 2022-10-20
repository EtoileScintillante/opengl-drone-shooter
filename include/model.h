/*                                  
 * model.h
 *
 * This file contains a class to load and render an obj/mtl 3D model.
 * 
 * Original author: Joey de Vries (from learnopengl)
 * Modified by EtoileScintillante.
 */

#ifndef __MODEL_H__
#define __MODEL_H__

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"
#include "texture_loading.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model 
{
public:
    // model data
    std::vector<Texture> textures_loaded; // stores all the textures loaded so far, to make sure textures aren't loaded more than once
    std::vector<Mesh>    meshes;          // stores all the meshes of the model
    std::string directory;                // model directory
    bool gammaCorrection;                 // apply gamma correction?
    bool flipVertically;                  // flip image vertically on load? 
    
    /// Default constructor.
    Model();
    
    /**
     * @brief Constructs a new Model object.
     * 
     * @param path filepath to the model.
     * @param flipVertically flip the textures vertically on load or not?
     * @param gamma apply gamma correction? Default is false.
     */
    Model(std::string const &path, bool flipVertically, bool gamma = false);

    /// Draws the model, and thus all its meshes.
    void Draw(Shader &shader);

    /// Only draws mesh at given index.
    void drawSpecificMesh(Shader &shader, int index);
    
private:
    /// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const &path);

    /**
     * @brief Processes a node in a recursive fashion: 
     * processes each individual mesh located at the node and repeats this process on its children nodes (if any).
     * 
     * @param node aiNode* node.
     * @param scene aiScene* scene.
     */
    void processNode(aiNode *node, const aiScene *scene);

    /**
     * @brief Processes the data of a mesh (vertices, indices and textures).
     * 
     * @param mesh aiMesh* mesh.
     * @param scene aiScene* scene.
     * @return Mesh 
     */
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    /**
     * @brief Checks all material textures of a given type and loads the textures if they're not loaded yet. 
     * The required info is returned as a Texture struct.
     * 
     * @param mat materials (aiMaterial*).
     * @param type type of texture (aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_HEIGHT, aiTextureType_AMBIENT).
     * @param typeName name of texture (how texture is defined in shader).
     * @return std::vector<Texture>
     */
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif /*__MODEL__*/