/**
 * hud.h 
 *
 * Created by EtoileScintillante.
 * This file contains functions to render the HUD (heads up display) and other text.
 */

#ifndef __HUD_H__
#define __HUD_H__

#include "text_renderer.h"
#include "enemy_manager.h"
#include "player.h"
#include "world.h"

/// @brief Renders simple start/title screen.
/// @param[out] tr TextRenderer object.
/// @param[out] player Player object.
/// @param[out] world World object (for drawing skybox).
void startingScreen(TextRenderer &tr, Player &player, World &world);

/// @brief Renders text that should be visible while playing: player's kills and health.
/// @param[out] tr TextRenderer object.
/// @param[out] player Player object.
void inGameScreen(TextRenderer &tr, Player &player);

/// @brief Rneders simple ending screen (with option to play again).
/// @param[out] tr TextRenderer object.
/// @param[out] player Player object.
/// @param[out] world WWorld object (for drawing skybox).
void endingScreen(TextRenderer &tr, Player &player, World &world);

#endif /*__HUD__*/