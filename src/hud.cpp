#include "hud.h"

void startingScreen(TextRenderer &tr, Player &player, World &world)
{
    // update blink time
    tr.blink += tr.deltaTime + 0.02; // add small offset to make text blink faster

    // render skybox
    world.view = player.GetViewMatrix();
    world.projection = player.getProjectionMatrix();
    world.drawSkyBox(false);

    // set projection matrix and render title screen
    tr.projection = player.getOrthoProjectionMatrix();
    tr.RenderText("Drone  Shooter", 75.0f, 400.0f, 1.8f, glm::vec3(0.0f, 0.0f, 0.0f));
    if (static_cast<int>(tr.blink) % 2 == 0)
    {
        tr.RenderText("Press  ENTER  to  start", 253.0f, 303.0f, 0.55f, glm::vec3(1.0f, 1.0f, 1.0f));
    }
    tr.RenderText("by  Esma", 352.0f, 30.0f, 0.4f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void inGameScreen(TextRenderer &tr, Player &player)
{
    // update blink time
    tr.blink += tr.deltaTime + 0.03; // add small offset to make text blink faster

    // set projection matrix and render health and kill count
    tr.projection = player.getOrthoProjectionMatrix();
    int playerHealth = static_cast<int>(player.getHealth());
    std::string health = "Health:  " + std::to_string(playerHealth);
    std::string kill = "Kills:  " + std::to_string(player.kills);

    // add blinking effect and red color to player's health if it gets low
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
    tr.RenderText(kill, 365.0f, 565.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
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
    tr.RenderText("Game  Over", 183.0f, 400.0f, 1.6f, glm::vec3(0.7f, 0.0f, 0.2f));
    std::string kill = "Kills:  " + std::to_string(player.kills);
    tr.RenderText(kill, 335.0f, 300.0f, 0.6f, glm::vec3(0.0f, 0.0f, 0.0f));
    if (static_cast<int>(tr.blink) % 2 == 0)
    {
        tr.RenderText("Press  ENTER  to  play  again  or  ESC  to  quit", 115.0f, 210.0f, 0.55f, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}