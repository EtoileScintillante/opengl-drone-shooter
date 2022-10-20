/*                                  
 * shader.h
 *
 * This file contains a shader class to make compiling and using shaders easier.
 * 
 * Original author: Joey de Vries (from learnopengl)
 * Modified by EtoileScintillante.
 */

#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    /// Default constructor.
    Shader();

    /// Constructor with paths to shaders (it generates the shaders on the fly).
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

    /// Activates the shader.
    void use() const;
    
    /**
     * @brief Sets bool uniform.
     * 
     * @param name name of uniform.
     * @param value bool.
     */
    void setBool(const std::string &name, bool value) const;

    /**
     * @brief Sets int uniform.
     * 
     * @param name name of uniform.
     * @param value int.
     */
    void setInt(const std::string &name, int value) const;
    
    /**
     * @brief Sets float uniform.
     * 
     * @param name name of uniform.
     * @param value float.
     */
    void setFloat(const std::string &name, float value) const;
    
    /**
     * @brief Sets vec2 uniform: glm::vec2.
     * 
     * @param name name of uniform.
     * @param value glm::vec2.
     */
    void setVec2(const std::string &name, const glm::vec2 &value) const;

    /**
     * @brief Sets vec2 uniform: x, y.
     * 
     * @param name name of uniform.
     * @param x x coordinate.
     * @param y y coordinate.
     */
    void setVec2(const std::string &name, float x, float y) const;
    
    /**
     * @brief Sets vec3 uniform: glm::vec3.
     * 
     * @param name name of uniform.
     * @param value glm::vec3.
     */
    void setVec3(const std::string &name, const glm::vec3 &value) const;

    /**
     * @brief Sets vec3 uniform: x, y, z.
     * 
     * @param name name of uniform.
     * @param x x coordinate.
     * @param y y coordinate.
     * @param z z coordinate.
     */
    void setVec3(const std::string &name, float x, float y, float z) const;
    
    /**
     * @brief Sets vec4 uniform: glm::vec4.
     * 
     * @param name name of uniform.
     * @param value glm::vec4.
     */
    void setVec4(const std::string &name, const glm::vec4 &value) const;

    /**
     * @brief Sets vec4 uniform: x, y, z, w.
     * 
     * @param name name of uniform.
     * @param x x coordinate.
     * @param y y coordinate.
     * @param z z coordinate.
     * @param w w coordinate.
     */
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    
    /**
     * @brief Sets mat2 uniform.
     * 
     * @param name name of uniform.
     * @param mat glm::mat2.
     */
    void setMat2(const std::string &name, const glm::mat2 &mat) const;

    /**
     * @brief Sets mat3 uniform.
     * 
     * @param name name of uniform.
     * @param mat glm::mat3.
     */
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    
    /**
     * @brief Sets mat4 uniform.
     * 
     * @param name name of uniform.
     * @param mat glm::mat4.
     */
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    
private:
    /// Utility function for checking shader compilation/linking errors.
    void checkCompileErrors(GLuint shader, std::string type);
};
#endif /*__SHADER__*/