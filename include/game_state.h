/**
 * game_state.h
 *
 * Defines the top-level game states managed by main.
 *
 * Created by EtoileScintillante.
 */

#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

enum class GameState
{
    START,     // title / start screen
    PLAYING,   // active gameplay
    GAME_OVER  // ending screen
};

#endif /*__GAME_STATE__*/
