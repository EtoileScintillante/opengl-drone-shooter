#include "hud.h"

void startingScreen(TextRenderer &tr, Player &player, World &world)
{
    // update blink time
    tr.blink += tr.deltaTime + 0.02; // add small offset to make text blink faster

    // render skybox
    world.view = player.GetViewMatrix();
    world.projection = player.getProjectionMatrix();
    world.drawSkyBox(false);

    // set projection matrix and render title screen with instructions on how to play
    tr.projection = player.getOrthoProjectionMatrix();

    // calculate scaling factors (all x-pos and y-pos in RenderText are based on a screen with height = 600 and width = 800)
    float xScale = static_cast<float>(player.SCR_WIDTH) / 800.0f;
    float yScale = static_cast<float>(player.SCR_HEIGHT) / 600.0f;

    // also calculate text scale factor
    float textScale = std::min(xScale, yScale);

    // title
    tr.RenderText("Drone  Shooter", 75.0f * xScale, 400.0f * yScale, 1.8f * textScale, glm::vec3(0.0f, 0.0f, 0.0f));

    // add blinking effect
    if (static_cast<int>(tr.blink) % 2 == 0)
    {
        tr.RenderText("Press  ENTER  to  start", 253.0f * xScale, 303.0f * yScale, 0.55f * textScale, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    // instructions (it looks weird but this way the colons are aligned)
    float textSize = 0.4f * textScale;
    glm::vec3 textColor = glm::vec3(1.0f, 1.0f, 1.0f);
    tr.RenderText("Space  :  take shot", 310.0f * xScale, 180.0f * yScale, textSize, textColor);
    tr.RenderText("W              :   forwards", 310.0f * xScale, 155.0f * yScale, textSize, textColor);
    tr.RenderText("S                :   backwards", 310.0f * xScale, 135.0f * yScale, textSize, textColor);
    tr.RenderText("D                :   right", 310.0f * xScale, 115.0f * yScale, textSize, textColor);
    tr.RenderText("A               :   left", 310.0f * xScale, 95.0f * yScale, textSize, textColor);
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

void endingScreen(TextRenderer &tr, Player &player, World &world)
{
    // update blink time
    tr.blink += tr.deltaTime + 0.02; // add small offset to make text blink faster

    // render skybox in grayscale
    world.view = player.GetViewMatrix();
    world.projection = player.getProjectionMatrix();
    world.drawSkyBox(true);

    // set projection matrix and render game over screen
    tr.projection = player.getOrthoProjectionMatrix();

    // calculate scaling factors (all x-pos and y-pos in RenderText are based on a screen with height = 600 and width = 800)
    float xScale = static_cast<float>(player.SCR_WIDTH) / 800.0f;
    float yScale = static_cast<float>(player.SCR_HEIGHT) / 600.0f;

    // also calculate text scale factor
    float textScale = std::min(xScale, yScale);

    // game over
    tr.RenderText("Game  Over", 183.0f * xScale, 400.0f * yScale, 1.6f * textScale, glm::vec3(0.7f, 0.0f, 0.2f));

    // add blinking effect
    if (static_cast<int>(tr.blink) % 2 == 0)
    {
        tr.RenderText("Press  ENTER  to  play  again  or  ESC  to  quit", 115.0f * xScale, 210.0f * yScale, 0.55f * textScale, glm::vec3(0.870f, 0.592f, 0.0348f));
    }

    // kill count
    std::string kill = "Kills:  " + std::to_string(player.getKills());
    tr.RenderText(kill, 335.0f * xScale, 300.0f * yScale, 0.7f * textScale, glm::vec3(1.0f, 1.0f, 1.0f));
}