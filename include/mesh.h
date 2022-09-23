/*                                  
 * mesh.h
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
struct Vertex { 
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent 
    glm::vec3 Tangent; 
    // bitangent 
    glm::vec3 Bitangent; 
	// bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	// weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

// store texture data in a texture struct.
struct Texture {
    unsigned int id;
    std::string type; // diffuse? specular? 
    std::string path;
};

class Mesh {
public:
    // mesh Data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    unsigned int VAO;

    /// constructor (here we give the mesh all the necessary data).
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    /// render the mesh (here we give a shader to the Draw function; by passing the shader to the mesh 
    /// we can set several uniforms before drawing (like linking samplers to texture units)).
    void Draw(Shader &shader);

private: 
    // render data
    unsigned int VBO, EBO;

    /// initializes all the buffer objects/arrays.
    void setupMesh();
};
#endif /*__MESH__*/