#include "hud.h"

void startingScreen(TextRenderer &tr, Player &player, World &world)
{
    // render skybox
    world.view = player.GetViewMatrix();
    world.projection = player.getProjectionMatrix();
    world.drawSkyBox();

    // set projection matrix
    tr.projection = player.getOrthoProjectionMatrix();

    // render game title
    tr.RenderText("Drone Shooter", 350.0f, 550.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

    // render instructions
    tr.RenderText("Press [SPACE] to start", 350.0f, 450.0f, 0.6f, glm::vec3(0.0f, 0.0f, 0.0f));
    tr.RenderText("How to play:", 350.0f, 400.0f, 0.4f, glm::vec3(0.0f, 0.0f, 0.0f));
    tr.RenderText("[W] = forward", 350.0f, 375.0f, 0.4f, glm::vec3(0.0f, 0.0f, 0.0f));
    tr.RenderText("[S] = backward", 350.0f, 350.0f, 0.4f, glm::vec3(0.0f, 0.0f, 0.0f));
    tr.RenderText("[D] = right", 350.0f, 325.0f, 0.4f, glm::vec3(0.0f, 0.0f, 0.0f));
    tr.RenderText("[A] = left", 350.0f, 300.0f, 0.4f, glm::vec3(0.0f, 0.0f, 0.0f));
    tr.RenderText("[SPACE] = shoot", 350.0f, 275.0f, 0.4f, glm::vec3(0.0f, 0.0f, 0.0f));
}

void inGameScreen(TextRenderer &tr, Player &player)
{
    // set projection matrix and render kills and health
    tr.projection = player.getOrthoProjectionMatrix();
    tr.RenderText(player.getHealthString(), 25.0f, 25.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
    tr.RenderText(player.getKillsString(), 25.0f, 50.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
}

void endingScreen(TextRenderer &tr, Player &player, World &world)
{
    // TODO
}