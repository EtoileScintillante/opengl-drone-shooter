#include "text_renderer.h"

TextRenderer::TextRenderer(std::string pathToFont, std::string pathVertexShader, std::string pathFragmentShader)
{
    this->font = pathToFont;
    blink = 0;

    // compile shader
    shader = Shader(pathVertexShader.c_str(), pathFragmentShader.c_str());

    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    // find path to font
    if (font.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    // load font as face
    loadFace();

    // configure VAO and VBO
    configBuffers();
}

void TextRenderer::RenderText(std::string_view text, float x, float y, float scale, glm::vec3 color)
{
    // set OpenGL state
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // activate corresponding render state
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string_view::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        const Character ch = characters[*c - 32];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}};
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    // reset
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}

float TextRenderer::MeasureText(std::string_view text, float scale) const
{
    float width = 0.0f;
    for (char c : text)
    {
        if (c < 32 || c > 126)
        {
            continue;
        }
        width += static_cast<float>(characters[c - 32].Advance >> 6) * scale;
    }
    return width;
}

// Border stroke width in FreeType's 26.6 fixed-point format
// (64 units = 1 pixel). FreeType uses this representation to
// achieve sub-pixel precision for smoother text rendering.
// A stroke radius of 2 pixels therefore becomes 2 * 64 = 128 units.
static constexpr int BORDER_STROKE_RADIUS = 2 * 64; 

static void uploadGlyphTexture(unsigned int &texID, const FT_Bitmap &bmp)
{
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                 bmp.width, bmp.rows, 0,
                 GL_RED, GL_UNSIGNED_BYTE, bmp.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void TextRenderer::loadFace()
{
    if (FT_New_Face(ft, font.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font as face" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // set up stroker for border glyphs
    FT_Stroker stroker;
    FT_Stroker_New(ft, &stroker);
    FT_Stroker_Set(stroker, BORDER_STROKE_RADIUS,
                   FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

    for (unsigned char c = 32; c < 127; c++)
    {
        // load as vector outline so the stroker can work with it
        if (FT_Load_Char(face, c, FT_LOAD_NO_BITMAP))
        {
            std::cout << "ERROR::FREETYPE: Failed to load glyph" << std::endl;
            continue;
        }

        unsigned int fillAdvance = static_cast<unsigned int>(face->glyph->advance.x);

        // save a copy of the outline before rendering destroys it
        FT_Glyph outlineGlyph;
        FT_Get_Glyph(face->glyph, &outlineGlyph);

        // fill glyph
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        unsigned int fillTex;
        uploadGlyphTexture(fillTex, face->glyph->bitmap);
        characters[c - 32] = {
            fillTex,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left,  face->glyph->bitmap_top),
            fillAdvance
        };

        // border glyph
        // FT_Glyph_Stroke modifies outlineGlyph in place; destroy=true frees input on success
        if (FT_Glyph_Stroke(&outlineGlyph, stroker, true) == 0)
        {
            FT_Glyph_To_Bitmap(&outlineGlyph, FT_RENDER_MODE_NORMAL, nullptr, true);
            FT_BitmapGlyph borderBmp = reinterpret_cast<FT_BitmapGlyph>(outlineGlyph);
            unsigned int borderTex;
            uploadGlyphTexture(borderTex, borderBmp->bitmap);
            borderCharacters[c - 32] = {
                borderTex,
                glm::ivec2(borderBmp->bitmap.width, borderBmp->bitmap.rows),
                glm::ivec2(borderBmp->left,          borderBmp->top),
                fillAdvance
            };
            FT_Done_Glyph(outlineGlyph);
        }
        else
        {
            // no outline to stroke (e.g. space) —> fall back to fill metrics
            FT_Done_Glyph(outlineGlyph);
            borderCharacters[c - 32] = characters[c - 32];
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Stroker_Done(stroker);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::RenderTextBordered(std::string_view text, float x, float y, float scale, glm::vec3 fillColor, glm::vec3 borderColor)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // pass 1: border (rendered first so fill sits nicely on top)
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), borderColor.x, borderColor.y, borderColor.z);
    float cx = x;
    for (auto c = text.begin(); c != text.end(); c++)
    {
        const Character &fill   = characters[*c - 32];
        const Character &border = borderCharacters[*c - 32];

        float xpos = cx + border.Bearing.x * scale;
        float ypos = y  - (border.Size.y - border.Bearing.y) * scale;
        float w = border.Size.x * scale;
        float h = border.Size.y * scale;
        
        // verts array defines the rectangle where one character texture is drawn:
        float verts[6][4] = {
            {xpos,     ypos + h, 0.0f, 0.0f},
            {xpos,     ypos,     0.0f, 1.0f},
            {xpos + w, ypos,     1.0f, 1.0f},
            {xpos,     ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos,     1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}
        };
        glBindTexture(GL_TEXTURE_2D, border.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        cx += (fill.Advance >> 6) * scale; 
    }

    // pass 2: fill
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), fillColor.x, fillColor.y, fillColor.z);
    cx = x;
    for (auto c = text.begin(); c != text.end(); c++)
    {
        const Character &ch = characters[*c - 32];

        float xpos = cx + ch.Bearing.x * scale;
        float ypos = y  - (ch.Size.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float verts[6][4] = {
            {xpos,     ypos + h, 0.0f, 0.0f},
            {xpos,     ypos,     0.0f, 1.0f},
            {xpos + w, ypos,     1.0f, 1.0f},
            {xpos,     ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos,     1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}
        };
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        cx += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}

void TextRenderer::configBuffers()
{
    // each 2D quad requires 6 vertices of 4 floats each
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
