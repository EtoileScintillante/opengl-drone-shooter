/*                                  
 * data.h
 *
 * Created by: EtoileScintillante
 * This struct represents vertex data.
 */

#ifndef __DATA_H__
#define __DATA_H__

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

#endif /*__DATA__*/