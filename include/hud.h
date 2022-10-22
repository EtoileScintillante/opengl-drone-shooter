/**
 * hud.h 
 *
 * This file contains functions to render the title screen, ending screen and the HUD (heads up display).
 * 
 * Created by EtoileScintillante.
 */

#ifndef __HUD_H__
#define __HUD_H__

#include "text_renderer.h"
#include "player.h"
#include "world.h"

/**
 * @brief Renders simple start/title screen.
 * 
 * @param tr TextRenderer object.
 * @param player Player object.
 * @param world World object (for drawing the skybox as background).
 */
void startingScreen(TextRenderer &tr, Player &player, World &world);

/**
 * @brief Renders text that should be visible while playing: player's kills and health.
 * 
 * @param tr TextRenderer object.
 * @param player Player object.
 * @param aimAssist if true: renders a "+" to make it more clear where the gun is aiming on (default = false).
 */
void inGameScreen(TextRenderer &tr, Player &player, bool aimAssist = false);

/**
 * @brief Renders simple ending screen (with option to play again).
 * 
 * @param tr TextRenderer object.
 * @param player Player object.
 * @param world World object (for drawing the skybox as background).
 */
void endingScreen(TextRenderer &tr, Player &player, World &world);

#endif /*__HUD__*/