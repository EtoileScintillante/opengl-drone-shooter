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

/**
 * @brief Renders start/title screen with a fullscreen background image and environment selector.
 *
 * @param tr TextRenderer object.
 * @param player Player object.
 * @param selectedEnv index of currently highlighted environment (0=desert, 1=forest, 2=snow, 3=night).
 */
void startingScreen(TextRenderer &tr, Player &player, int selectedEnv);

/**
 * @brief Renders text that should be visible while playing: player's kills and health.
 *
 * @param tr TextRenderer object.
 * @param player Player object.
 */
void inGameScreen(TextRenderer &tr, Player &player);

/**
 * @brief Renders ending screen with a fullscreen background image.
 *
 * @param tr TextRenderer object.
 * @param player Player object.
 */
void endingScreen(TextRenderer &tr, Player &player);

#endif /*__HUD__*/