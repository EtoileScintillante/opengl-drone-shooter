#include "hud.h"
#include "screen_renderer.h"
#include "texture_loading.h"

void startingScreen(TextRenderer &tr, Player &player, int selectedEnv)
{
    // update blink time
    tr.blink += tr.deltaTime + 0.02; // add small offset to make text blink faster

    // render background image (lazy-initialized on first call)
    static ScreenRenderer renderer("shaders/screen.vert", "shaders/screen.frag");
    static unsigned int bgTexture = TextureFromFile("game_start.png", "resources/screens", true);
    renderer.draw(bgTexture);

    // set projection matrix and render title screen with instructions on how to play
    tr.projection = player.getOrthoProjectionMatrix();

    // calculate scaling factors (all x-pos and y-pos in RenderText are based on a screen with height = 600 and width = 800)
    float xScale = static_cast<float>(player.SCR_WIDTH) / 800.0f;
    float yScale = static_cast<float>(player.SCR_HEIGHT) / 600.0f;

    // also calculate text scale factor
    float textScale = std::min(xScale, yScale);

    // title
    tr.RenderTextBordered("Drone  Shooter", 180.0f * xScale, 400.0f * yScale, 1.2f * textScale,
                           glm::vec3(0.392f, 0.263f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    // add blinking effect
    if (static_cast<int>(tr.blink) % 2 == 0)
    {
        tr.RenderText("Press  ENTER  to  start", 253.0f * xScale, 303.0f * yScale, 0.55f * textScale, glm::vec3(0.82f, 0.82f, 0.82f));
    }

    float textSize = 0.35f * textScale;
    glm::vec3 textColor = glm::vec3(0.82f, 0.82f, 0.82f);
    tr.RenderText("How  to  play :", 20.0f * xScale, 173.0f * yScale, textSize, textColor);
    tr.RenderText("Mouse   -   look  around", 20.0f * xScale, 148.0f * yScale, textSize, textColor);
    tr.RenderText("Space   -   shoot", 20.0f * xScale, 123.0f * yScale, textSize, textColor);
    tr.RenderText("WASD / arrows   -   walk", 20.0f * xScale, 98.0f * yScale, textSize, textColor);

    // environment selector
    const std::string envNames[4] = {"Desert", "Forest", "Snow", "Night"};
    float selectorTextSize = 0.55f * textScale;
    float nameX   = 370.0f * xScale; // env name column
    float arrowX  = 338.0f * xScale; // ">" column, slightly to the left
    float startY  = 195.0f;
    float stepY   = 33.0f;

    for (int i = 0; i < 4; i++)
    {
        float y = (startY - i * stepY) * yScale;
        if (i == selectedEnv)
        {
            tr.RenderTextBordered(">", arrowX, y, selectorTextSize, glm::vec3(0.392f, 0.263f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
            tr.RenderTextBordered(envNames[i], nameX, y, selectorTextSize + 0.05, glm::vec3(0.392f, 0.263f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        }
        else
        {
            tr.RenderText(envNames[i], nameX, y, selectorTextSize, glm::vec3(0.392f, 0.263f, 1.0f));
        }
    }
}

void inGameScreen(TextRenderer &tr, Player &player)
{
    // update blink time
    tr.blink += tr.deltaTime + 0.03; // add small offset to make text blink faster

    // set projection matrix and render in-game HUD
    tr.projection = player.getOrthoProjectionMatrix();
    int playerHealth = static_cast<int>(player.getHealth());
    std::string health = "Health:  " + std::to_string(playerHealth);
    std::string kill = "Kills:  " + std::to_string(player.getKills());

    // calculate scaling factors (all x-pos and y-pos in RenderText are based on a screen with height = 600 and width = 800)
    float xScale = static_cast<float>(player.SCR_WIDTH) / 800.0f;
    float yScale = static_cast<float>(player.SCR_HEIGHT) / 600.0f;

    // also calculate text scale factor
    float textScale = std::min(xScale, yScale);

    // kill count
    tr.RenderText(kill, 365.0f * xScale, 565.0f * yScale, 0.5f * textScale, glm::vec3(0.0f, 0.0f, 0.0f));

    // player's health: add blinking effect and red color if it gets low
    if (playerHealth <= 20)
    {
        if (static_cast<int>(tr.blink) % 2 == 0)
        {
            tr.RenderText(health, 348.0f * xScale, 540.0f * yScale, 0.5f * textScale, glm::vec3(0.7f, 0.0f, 0.1f));
        }
    }
    else
    {
        tr.RenderText(health, 348.0f * xScale, 540.0f * yScale, 0.5f * textScale, glm::vec3(0.0f, 0.0f, 0.0f));
    }

    // cross hairs
    float textSize = 0.3f * textScale;
    glm::vec3 textColor = glm::vec3(1.0f, 1.0f, 1.0f);
    tr.RenderText("-", 356.0f * xScale, 300.0f * yScale, textSize, textColor);
    tr.RenderText("-", 429.0f * xScale, 300.0f * yScale, textSize, textColor);
    tr.RenderText("|", 395.0f * xScale, 337.0f * yScale, textSize, textColor);
    tr.RenderText("|", 395.0f * xScale, 267.0f * yScale, textSize, textColor);
    
}

void endingScreen(TextRenderer &tr, Player &player)
{
    // update blink time
    tr.blink += tr.deltaTime + 0.02; // add small offset to make text blink faster

    // render background image (lazy-initialized on first call)
    static ScreenRenderer renderer("shaders/screen.vert", "shaders/screen.frag");
    static unsigned int bgTexture = TextureFromFile("game_over.png", "resources/screens", true);
    renderer.draw(bgTexture);

    // set projection matrix and render game over screen
    tr.projection = player.getOrthoProjectionMatrix();

    // calculate scaling factors (all x-pos and y-pos in RenderText are based on a screen with height = 600 and width = 800)
    float xScale = static_cast<float>(player.SCR_WIDTH) / 800.0f;
    float yScale = static_cast<float>(player.SCR_HEIGHT) / 600.0f;

    // also calculate text scale factor
    float textScale = std::min(xScale, yScale);

    // game over with white border (FT_Stroker-generated outline)
    tr.RenderTextBordered("Game  Over", 183.0f * xScale, 400.0f * yScale, 1.6f * textScale,
                           glm::vec3(0.7f, 0.0f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f));

    // add blinking effect
    if (static_cast<int>(tr.blink) % 2 == 0)
    {
        tr.RenderTextBordered("Press  ENTER  to  play  again  or  ESC  to  quit",
                               115.0f * xScale, 210.0f * yScale, 0.55f * textScale,
                               glm::vec3(0.870f, 0.592f, 0.0348f), glm::vec3(1.0f, 1.0f, 1.0f));
    }

    // kill count
    std::string kill = "Kills:  " + std::to_string(player.getKills());
    tr.RenderText(kill, 335.0f * xScale, 300.0f * yScale, 0.7f * textScale, glm::vec3(1.0f, 1.0f, 1.0f));
}