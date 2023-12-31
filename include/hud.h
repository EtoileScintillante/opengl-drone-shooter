/**
 * hud.h 
 *
 * This file contains functions to render the title screen, ending screen and the HUD (heads up display).
 * Current implementations of the functions are based on the font "theboldfont.ttf".
 * Text positions and sizes are adjusted according to the screen dimension (player::SCR_HEIGHT and player::SCR_WIDTH)
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
 */
void inGameScreen(TextRenderer &tr, Player &player);

/**
 * @brief Renders simple ending screen (with option to play again).
 * 
 * @param tr TextRenderer object.
 * @param player Player object.
 * @param world World object (for drawing the skybox as background).
 */
void endingScreen(TextRenderer &tr, Player &player, World &world);

#endif /*__HUD__*/