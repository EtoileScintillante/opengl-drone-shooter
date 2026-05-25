/**
 * screen_renderer.h
 *
 * Renders a fullscreen textured quad. Used for start and ending screens.
 *
 * Created by EtoileScintillante.
 */

#ifndef __SCREEN_RENDERER_H__
#define __SCREEN_RENDERER_H__

#include <glad/glad.h>
#include <string>
#include "shader.h"

class ScreenRenderer
{
public:
    /**
     * @brief Constructs a ScreenRenderer and uploads a fullscreen quad to the GPU.
     *
     * @param vertPath path to vertex shader.
     * @param fragPath path to fragment shader.
     */
    ScreenRenderer(const std::string& vertPath, const std::string& fragPath);

    /**
     * @brief Draws the given texture as a fullscreen background.
     * Disables depth testing so 2D text drawn afterwards renders correctly on top.
     *
     * @param textureID OpenGL texture ID.
     */
    void draw(unsigned int textureID);

private:
    Shader shader;
    unsigned int VAO, VBO;

    void setupQuad();
};

#endif /*__SCREEN_RENDERER__*/
