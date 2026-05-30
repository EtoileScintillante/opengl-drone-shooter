#include "hud.h"
#include "screen_renderer.h"
#include "texture_loading.h"

#include <algorithm>

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
    std::string_view title = "DRONE  SHOOTER";
    float titleScale = 1.52f * textScale;
    float titleX = (static_cast<float>(player.SCR_WIDTH) - tr.MeasureText(title, titleScale)) * 0.5f;
    tr.RenderTextBordered(title, titleX, 475.0f * yScale, titleScale,
                           glm::vec3(0.392f, 0.263f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    // add blinking effect
    if (static_cast<int>(tr.blink) % 2 == 0)
    {
        std::string_view startPrompt = "Press  ENTER  to  start";
        float startPromptScale = 0.55f * textScale;
        float startPromptX = (static_cast<float>(player.SCR_WIDTH) - tr.MeasureText(startPrompt, startPromptScale)) * 0.5f;
        tr.RenderText(startPrompt, startPromptX, 315.0f * yScale, startPromptScale, glm::vec3(0.82f, 0.82f, 0.82f));
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
    float selectedSelectorTextSize = selectorTextSize + 0.05f;
    float nameCenterX = static_cast<float>(player.SCR_WIDTH) * 0.5f;
    float arrowX = nameCenterX - 72.0f * xScale;
    float startY  = 215.0f;
    float stepY   = 33.0f;

    for (int i = 0; i < 4; i++)
    {
        float y = (startY - i * stepY) * yScale;
        if (i == selectedEnv)
        {
            float nameX = nameCenterX - tr.MeasureText(envNames[i], selectedSelectorTextSize) * 0.5f;
            tr.RenderTextBordered(">", arrowX, y, selectorTextSize, glm::vec3(0.392f, 0.263f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
            tr.RenderTextBordered(envNames[i], nameX, y, selectedSelectorTextSize, glm::vec3(0.392f, 0.263f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        }
        else
        {
            float nameX = nameCenterX - tr.MeasureText(envNames[i], selectorTextSize) * 0.5f;
            tr.RenderText(envNames[i], nameX, y, selectorTextSize, glm::vec3(0.392f, 0.263f, 1.0f));
        }
    }
}

void inGameScreen(TextRenderer &tr, Player &player)
{
    // set projection matrix and render in-game HUD
    tr.projection = player.getOrthoProjectionMatrix();
    int playerHealth = static_cast<int>(player.getHealth());

    // calculate scaling factors (all x-pos and y-pos in RenderText are based on a screen with height = 600 and width = 800)
    float xScale = static_cast<float>(player.SCR_WIDTH) / 800.0f;
    float yScale = static_cast<float>(player.SCR_HEIGHT) / 600.0f;

    // also calculate text scale factor
    float textScale = std::min(xScale, yScale);

    const glm::vec3 black(0.0f, 0.0f, 0.0f);
    const glm::vec3 white(1.0f, 1.0f, 1.0f);

    int visibleHealth = playerHealth;
    if (visibleHealth < 0)
    {
        visibleHealth = 0;
    }
    else if (visibleHealth > 100)
    {
        visibleHealth = 100;
    }

    int filledBars = visibleHealth / 10;
    std::string healthBar = "[" + std::string(filledBars, '#') + std::string(10 - filledBars, '-') + "]  " + std::to_string(visibleHealth);

    glm::vec3 healthColor(0.0f, 0.82f, 0.22f);
    if (visibleHealth < 30)
    {
        healthColor = glm::vec3(0.86f, 0.0f, 0.12f);
    }
    else if (visibleHealth <= 60)
    {
        healthColor = glm::vec3(1.0f, 0.46f, 0.0f);
    }

    float leftMargin = 20.0f * xScale;
    tr.RenderTextBordered("HEALTH", leftMargin, 565.0f * yScale, 0.48f * textScale, black, white);
    tr.RenderTextBordered(healthBar, leftMargin, 525.0f * yScale, 0.45f * textScale, healthColor, white);

    std::string killsLabel = "KILLS";
    std::string killCount = std::to_string(player.getKills());
    float killsLabelScale = 0.48f * textScale;
    float killCountScale = 0.68f * textScale;
    float rightMargin = 24.0f * xScale;
    float blockRight = static_cast<float>(player.SCR_WIDTH) - rightMargin;
    float killsBlockWidth = std::max(tr.MeasureText(killsLabel, killsLabelScale), tr.MeasureText(killCount, killCountScale));
    float killsCenter = blockRight - killsBlockWidth * 0.5f;

    tr.RenderTextBordered(killsLabel, killsCenter - tr.MeasureText(killsLabel, killsLabelScale) * 0.5f,
                          565.0f * yScale, killsLabelScale, black, white);
    tr.RenderTextBordered(killCount, killsCenter - tr.MeasureText(killCount, killCountScale) * 0.5f,
                          522.0f * yScale, killCountScale, black, white);

    // cross hairs
    float textSize = 0.3f * textScale;
    glm::vec3 textColor = glm::vec3(1.0f, 1.0f, 1.0f);
    float centerX = static_cast<float>(player.SCR_WIDTH) * 0.5f;
    float centerY = static_cast<float>(player.SCR_HEIGHT) * 0.5f;
    float horizontalGap = 42.0f * textScale;
    float verticalGap = 38.0f * textScale;
    float dashWidth = tr.MeasureText("-", textSize);
    float barWidth = tr.MeasureText("|", textSize);

    tr.RenderText("-", centerX - horizontalGap - dashWidth * 0.5f, centerY, textSize, textColor);
    tr.RenderText("-", centerX + horizontalGap - dashWidth * 0.5f, centerY, textSize, textColor);
    tr.RenderText("|", centerX - barWidth * 0.5f, centerY + verticalGap, textSize, textColor);
    tr.RenderText("|", centerX - barWidth * 0.5f, centerY - verticalGap, textSize, textColor);
    
}

void endingScreen(TextRenderer &tr, Player &player)
{
    // update blink time
    tr.blink += tr.deltaTime + 0.02;

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

    const glm::vec3 red(0.86f, 0.0f, 0.12f);
    const glm::vec3 white(1.0f, 1.0f, 1.0f);

    auto centeredX = [&](std::string_view text, float scale) {
        return (static_cast<float>(player.SCR_WIDTH) - tr.MeasureText(text, scale)) * 0.5f;
    };

    auto renderCenteredBordered = [&](std::string_view text, float y, float scale,
                                      glm::vec3 fillColor, glm::vec3 borderColor) {
        tr.RenderTextBordered(text, centeredX(text, scale), y * yScale, scale, fillColor, borderColor);
    };

    // The laser in the background sits slightly above center, so the title stays above it
    // while the stats and actions form a centered stack below it.
    if (static_cast<int>(tr.blink) % 2 == 0)
    {
        renderCenteredBordered("GAME OVER", 430.0f, 1.75f * textScale, red, white);
    }
    renderCenteredBordered("KILLS", 292.0f, 0.62f * textScale, white, red);
    renderCenteredBordered(std::to_string(player.getKills()), 215.0f, 1.22f * textScale, white, red);
    renderCenteredBordered("[ENTER]    -    RESTART", 158.0f, 0.50f * textScale, white, red);
    renderCenteredBordered("[ESC]    -    QUIT", 118.0f, 0.50f * textScale, white, red);
}
