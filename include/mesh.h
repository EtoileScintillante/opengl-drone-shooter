/*
 * mesh.h
 *
 * This file contains a Mesh class.
 * 
 * Original author: Joey de Vries (from learnopengl)
 * Modified by EtoileScintillante.
 */

#ifndef __MESH_H__
#define __MESH_H__

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

// store each of the required vertex attributes in a struct called Vertex.
struct Vertex
{
    glm::vec3 Position;                  // position
    glm::vec3 Normal;                    // normal
    glm::vec2 TexCoords;                 // texture coordinates
    glm::vec3 Tangent;                   // tangent
    glm::vec3 Bitangent;                 // bitangent
    int m_BoneIDs[MAX_BONE_INFLUENCE];   // bone indexes which will influence this vertex
    float m_Weights[MAX_BONE_INFLUENCE]; // weights from each bone
};

// store texture data in a texture struct.
struct Texture
{
    unsigned int id;  // texture ID
    std::string type; // type of texture: diffuse, specular, etc.
    std::string path; // path to texture
};

class Mesh
{
public:
    // mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;

    /// Constructor (here we give the mesh all the necessary data).
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    /**
     * @brief Renders the mesh. Here we give a shader to the Draw function; by passing the shader to the mesh
     * we can set several uniforms before drawing (like linking samplers to texture units).
     * 
     * @param shader shader.
     */
    void Draw(Shader &shader);

private:
    // render data
    unsigned int VBO, EBO;

    /// Initializes all the buffer objects/arrays.
    void setupMesh();
};
#endif /*__MESH__*/