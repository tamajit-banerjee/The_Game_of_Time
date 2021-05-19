#ifndef CONSTANS_H
#define CONSTANS_H

#include "stdbool.h"

#define CELL_SIZE 64
#define MAZECOLS 10
#define MAZEROWS 10

#define SCREEN_WIDTH (MAZECOLS * CELL_SIZE)
#define SCREEN_HEIGHT (MAZECOLS * CELL_SIZE)

#define SPEED 2
#define LEVELS 4

#define CLIENT_FRAME_TIME 15
#define SERVER_FRAME_TIME 15

#define COIN_SIZE (CELL_SIZE / 2)
#define COIN_SCORE 10

#define TIME_SIZE (COIN_SIZE)
#define TIME_INCREASE 100

#define PLAYER_WIDTH_SRC 48
#define PLAYER_HEIGHT_SRC 72
#define RENDER_PLAYER_DELAY 10
#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 48

#define MONSTERS 4
#define MONSTERS_DIR_CHANGE 30
#define MONSTER_SIZE 131
#define MONSTER_DELAY 8
#define FREEZE_LIMIT 20

#define SEPARATOR '|'
#define WALL_RATIO 20

#define BULLET_WIDTH 8
#define BULLET_HEIGHT 8

#define LEVEL_TIME 2000
#define TIME_STEP 10
#define CHASE_TIME 10
#define NON_CHASE_TIME 24

#define WALL_TIME 100

#define SCORE_DISPLAY_HEIGHT 100
#define SLEEP_UNIT 100

#define COIN_DIST 2

#define PERISCOPE 1

#endif
