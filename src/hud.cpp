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

    // title
    tr.RenderText("Drone  Shooter", 75.0f, 400.0f, 1.8f, glm::vec3(0.0f, 0.0f, 0.0f));

    // add blinking effect
    if (static_cast<int>(tr.blink) % 2 == 0)
    {
        tr.RenderText("Press  ENTER  to  start", 253.0f, 303.0f, 0.55f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    // instructions (it looks weird but this way the colons are aligned)
    tr.RenderText("Space  :  take shot", 310.0f, 180.0f, 0.4f, glm::vec3(1.0f, 1.0f, 1.0f));
    tr.RenderText("W              :   forwards", 310.0f, 155.0f, 0.4f, glm::vec3(1.0f, 1.0f, 1.0f));
    tr.RenderText("S                :   backwards", 310.0f, 135.0f, 0.4f, glm::vec3(1.0f, 1.0f, 1.0f));
    tr.RenderText("D                :   right", 310.0f, 115.0f, 0.4f, glm::vec3(1.0f, 1.0f, 1.0f));
    tr.RenderText("A               :   left", 310.0f, 95.0f, 0.4f, glm::vec3(1.0f, 1.0f, 1.0f));
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

    // kill count
    tr.RenderText(kill, 365.0f, 565.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

    // player's health: add blinking effect and red color if it gets low
    if (playerHealth <= 20)
    {
        if (static_cast<int>(tr.blink) % 2 == 0)
        {
            tr.RenderText(health, 348.0f, 540.0f, 0.5f, glm::vec3(0.7f, 0.0f, 0.1f));
        }
    }
    else
    {
        tr.RenderText(health, 348.0f, 540.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
    }

    // aim assist
    tr.RenderText("-", 356.0f, 300.0f, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
    tr.RenderText("-", 429.0f, 300.0f, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
    tr.RenderText("|", 395.0f, 337.0f, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
    tr.RenderText("|", 395.0f, 267.0f, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
    
}

void endingScreen(TextRenderer &tr, Player &player, World &world)
{
    // update blink time
    tr.blink += tr.deltaTime + 0.02; // add small offset to make text blink faster

    // render skybox
    world.view = player.GetViewMatrix();
    world.projection = player.getProjectionMatrix();
    world.drawSkyBox(true);

    // set projection matrix and render game over screen
    tr.projection = player.getOrthoProjectionMatrix();

    // game over
    tr.RenderText("Game  Over", 183.0f, 400.0f, 1.6f, glm::vec3(0.7f, 0.0f, 0.2f));

    // add blinking effect
    if (static_cast<int>(tr.blink) % 2 == 0)
    {
        tr.RenderText("Press  ENTER  to  play  again  or  ESC  to  quit", 115.0f, 210.0f, 0.55f, glm::vec3(0.0f, 0.0f, 0.0f));
    }

    // kill count
    std::string kill = "Kills:  " + std::to_string(player.getKills());
    tr.RenderText(kill, 335.0f, 300.0f, 0.6f, glm::vec3(0.0f, 0.0f, 0.0f));
}