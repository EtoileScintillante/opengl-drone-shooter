/**
 * hud.h 
 *
 * Created by EtoileScintillante.
 * This file contains functions to render the HUD (heads up display) and other text.
 */

#ifndef __HUD_H__
#define __HUD_H__

/*
TODO:
- starting HUD (black text in middle of screen, render only skybox)
  "Press SPACE to start"
  "Movement:
  "[W] = forward"
  "[S] = backward"
  "[D] = right"
  "[A] = left"
  "[SPACE] = shoot"
- in game HUD (black text, bottom left corner; see implementation in main loop)
- game over (player died) HUD (black text in middle of screen, render only skybox)
  "Game Over!"
  "Kills: [player.kills]"
  "Press [SPACE] to play again"
  "Press [ESC] to quit"

Notes: to control starting/ending of game, use a global variable in main "started" and use player.isAlive. 
Also if after game over the player presses space, we need to reset the player's values, 
this can easily be achieved by adding a "reset" method to the player class
*/

#endif /*__HUD__*/