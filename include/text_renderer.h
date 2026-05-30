/**
 * text_renderer.h
 *
 * This file contains a TextRenderer class and Character struct to make text rendering easier.
 * Based on code from Joey de Vries (https://learnopengl.com/In-Practice/Text-Rendering).
 * 
 * Created by EtoileScintillante.
 */

#ifndef __TEXT_RENDERER_H__
#define __TEXT_RENDERER_H__

#include <string>
#include <string_view>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "player.h"
#include "shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_STROKER_H

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
    float deltaTime;      // time passed between two frames
    float blink;          // needed to make text blink

    /**
     * @brief Constructs a new Text Renderer object.
     * Also initializes FreeType library, loads font and compiles shaders.
     * 
     * @param pathToFont path to font (ttf).
     * @param pathVertexShader path to vertex shader.
     * @param pathFragmentShader path to fragment shader.
     */
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

    /**
     * @brief Measures a line of text using the same glyph advances as RenderText.
     *
     * @param text text to measure.
     * @param scale scale factor.
     * @return width of the rendered text in screen pixels.
     */
    float MeasureText(std::string_view text, float scale) const;

    /**
     * @brief Renders text with a solid border using FT_Stroker-generated glyph outlines.
     * Border pass renders first so the fill sits cleanly on top.
     *
     * @param text text to render.
     * @param x x position.
     * @param y y position.
     * @param scale scale factor.
     * @param fillColor color of the text fill.
     * @param borderColor color of the border.
     */
    void RenderTextBordered(std::string_view text, float x, float y, float scale, glm::vec3 fillColor, glm::vec3 borderColor);

private:
    std::string font;          // path to font
    FT_Library ft;             // FreeType Library
    FT_Face face;              // needed to load font as a face
    Character characters[95];       // fill glyphs, covers ASCII 32–126
    Character borderCharacters[95]; // stroked border glyphs, same range
    Shader shader;             // shader
    unsigned int VBO, VAO;     // data buffers

    /// Loads font as face.
    void loadFace();

    /// Configures buffers for texture quads.
    void configBuffers();
};

#endif /*__TEXT_RENDERER__*/
