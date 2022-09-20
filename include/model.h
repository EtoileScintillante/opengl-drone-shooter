/*                                  
 * model.h
 *
 * Original author: Joey de Vries (from learnopengl)
 * Modified by: EtoileScintillante
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
    std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh>    meshes;
    std::string directory;
    bool gammaCorrection;
    bool flipVertically;                    // flip image vertically on load? 
    
    /// default constructor
    Model();
    
    /// constructor, expects a filepath to a 3D model.
    Model(std::string const &path, bool flipVertically, bool gamma = false);

    /// draws the model, and thus all its meshes
    void Draw(Shader &shader);

    /// only draws mesh at given index
    void drawSpecificMesh(Shader &shader, int index);
    
private:
    /// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const &path);

    /// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene);

    /// processes the data of a mesh
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    /// checks all material textures of a given type and loads the textures if they're not loaded yet. The required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif /*__MODEL__*/