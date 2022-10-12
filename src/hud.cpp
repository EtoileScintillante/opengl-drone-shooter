#include "hud.h"

void startingScreen(TextRenderer &tr, Player &player, World &world)
{
    // render skybox
    world.view = player.GetViewMatrix();
    world.projection = player.getProjectionMatrix();
    world.drawSkyBox();

    // set projection matrix and render title screen
    tr.projection = player.getOrthoProjectionMatrix();
    tr.RenderText("Drone  Shooter", 75.0f, 400.0f, 1.8f, glm::vec3(0.0f, 0.0f, 0.0f));
    tr.RenderText("Press  ENTER  to  start", 253.0f, 303.0f, 0.55f, glm::vec3(1.0f, 1.0f, 1.0f));
    tr.RenderText("by  Esma", 352.0f, 30.0f, 0.4f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void inGameScreen(TextRenderer &tr, Player &player)
{
    // set projection matrix and render kills and health
    tr.projection = player.getOrthoProjectionMatrix();
    tr.RenderText(player.getHealthString(), 25.0f, 25.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
    tr.RenderText(player.getKillsString(), 25.0f, 50.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
}

void endingScreen(TextRenderer &tr, Player &player, World &world, EnemyManager &em)
{
    /* in case player took a shot while dying, let the recoil animation finish before
    going to the ending screen and resetting the values if player presses enter, 
    otherwise the gun will spawn in a wrongly rotated way after restarting the game */
    if (player.shot)
    {
        player.controlGunRendering();
    }
    else
    {
        // reset enemies in case player wants to restart the game
        em.reset();

        // render skybox
        world.view = player.GetViewMatrix();
        world.projection = player.getProjectionMatrix();
        world.drawSkyBox();

        // set projection matrix and render game over screen
        tr.projection = player.getOrthoProjectionMatrix();
        tr.RenderText("Game  Over", 183.0f, 400.0f, 1.6f, glm::vec3(1.0f, 0.0f, 0.0f));
        tr.RenderText(player.getKillsString(), 335.0f, 300.0f, 0.6f, glm::vec3(1.0f, 1.0f, 1.0f));
        tr.RenderText("Press  ENTER  to  play  again  or  ESC  to  quit", 115.0f, 210.0f, 0.55f, glm::vec3(1.0f, 1.0f, 1.0f));
    }
    
}