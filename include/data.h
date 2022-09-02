/*                                  
 * data.h
 *
 * Created by: EtoileScintillante
 * This struct represents vertex data.
 */

#ifndef DATA_H
#define DATA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Data { 
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
};

#endif