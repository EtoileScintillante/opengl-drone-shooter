/**
 * text_renderer.h
 *
 * Created by EtoileScintillante.
 * Based on code from Joey de Vries (https://learnopengl.com/In-Practice/Text-Rendering).
 */

#ifndef __TEXT_RENDERER_H__
#define __TEXT_RENDERER_H__

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "player.h"
#include "shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H

/// Holds all state information relevant to a character as loaded using FreeType.
struct Character
{
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class TextRenderer
{
public:
    glm::mat4 projection; // projection matrix 

    /// Constructs new TextRenderer object.
    /// Also initializes FreeType library, loads font and compiles shaders.
    TextRenderer(std::string pathToFont, std::string pathVertexShader, std::string pathFragmentShader);

    /**
     * @brief Renders a line of text. Bottom left corner of screen is (x,y) = (0,0).
     *
     * @param text text (string) to render.
     * @param x x position of text (of the first character).
     * @param y y position of text.
     * @param scale scale factor.
     * @param color color of text.
     */
    void RenderText(std::string_view text, float x, float y, float scale, glm::vec3 color);

private:
    std::string font;          // path to font
    FT_Library ft;             // FreeType Library
    FT_Face face;              // needed to load font as a face
    Character characters[95];  // covers ASCII values from 32 to 127
    Shader shader;             // shader
    unsigned int VBO, VAO;     // data buffers

    /// Loads font as face.
    void loadFace();

    /// Configures buffers for texture quads.
    void configBuffers();
};

#endif /*__TEXT_RENDERER__*/