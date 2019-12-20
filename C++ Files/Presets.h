#pragma once

#include "ray_utils.h"


/*
* Material Presets and colors
*/
Color RED = { 0.95, 0.15, 0.15 };
Color GREEN = { 0.1, 0.95, 0.1};
Color BLUE = { 0.1, 0.1, 0.95 };
Color YELLOW = { 0.95, 0.95, 0.1 };
Color WHITE = { 0.95, 0.95, 0.95 };
Color BLACK = { 0, 0, 0 };
Color GRAY = { 0.5, 0.5, 0.5 };
Color REALLY_DARK = { 0.1, 0.1, 0.1 };
Color CYAN = { 0.1, 0.95, 0.95 };
Color MAGENTA = { 0.95, 0.1, 0.95 };
Color LIGHT_YELLOW = { 0.7, 0.7, 0.5 };
Color DAMP_YELLOW = { 0.3, 0.3, 0.1 };
float K_D_ball = 0.5;
float K_D_wall = 0.4;


Material LIGHT_YELLOW_WALL(LIGHT_YELLOW, DIFFUSE, K_D_wall, BLACK, 0, 0, 0.5, 0, 0, 0, 0);
Material DAMP_YELLOW_WALL(DAMP_YELLOW, DIFFUSE, K_D_wall, BLACK, 0, 0, 0.5, 0, 0, 0, 0);
Material BLACK_WALL(REALLY_DARK, DIFFUSE, K_D_wall, BLACK, 0, 0, 0.5, 0, 0, 0, 0);

Material GRAY_WALL(GRAY, DIFFUSE, K_D_wall, BLACK, 0, 0, 0.5, 0, 0,0,0);
Material RED_WALL(RED,  DIFFUSE, K_D_wall, BLACK, 0, 0,0.6, 0, 0,0,0);
Material GREEN_WALL( GREEN, GLOSSY, K_D_wall, BLACK,  0, 0 ,0.1, 0, 0,0,0);
Material GREEN_GLOSSY_WALL(GREEN,GLOSSY, 0, BLACK, 1, 0.4, 0.1, 0, 0, 0, 0);

Material WHITE_WALL(WHITE, DIFFUSE, K_D_wall, BLACK,  0, 0, 0.2, 0,0,0, 0);
Material BLUE_WALL (BLUE, DIFFUSE, K_D_wall, BLACK,  0, 0, 0.2, 0, 0, 0, 0);
Material WHITE_LIGHT_WALL ( WHITE, EMISSIVE, K_D_wall, WHITE, 0, 0, 0, 3,0, 0, 0);
Material YELLOW_LIGHT_WALL(YELLOW, EMISSIVE, K_D_wall, YELLOW, 0, 0, 0, 10, 0, 0, 0);
Material GREEN_LIGHT(GREEN, EMISSIVE, K_D_wall, GREEN, 0, 0, 0, 4, 0, 0, 0);
Material MAGENTA_LIGHT(GREEN, EMISSIVE, K_D_wall, LIGHT_YELLOW, 0, 0, 0, 2, 0, 0, 0);
Material WHITE_LIGHT_SPHERE ( WHITE, EMISSIVE, K_D_wall, WHITE, 0, 0, 0, 10,0, 0, 0);
Material YELLOW_WALL(YELLOW, DIFFUSE, K_D_wall, BLACK,  0, 0, 0.1, 0, 0, 0, 0);
Material YELLOW_BALL(YELLOW, GLOSSY, K_D_ball, BLACK, 0, 0.5, 0.3, 0, 0, 0, 0);
Material PERFECT_MIRROR( WHITE, MIRROR, 0, BLACK, 4, 1, 0, 0, 1, 0, 0);
Material GLASS_BALL ( WHITE, REFRACTIVE, 0, BLACK, 5, 0.2, 0, 0,0, 0.9, 1.3);
Material YELLOW_GLASS(YELLOW, REFRACTIVE, 0.2, BLACK, 5, 0.2, 0, 0, 0, 0.9, 1.4);
Material GREEN_GLASS(YELLOW, REFRACTIVE, 0.2, BLACK, 5, 0.2, 0, 0, 0, 0.9, 1.3);
Material CYAN_GLASS(CYAN, REFRACTIVE, 0.2, BLACK, 5, 0.2, 0, 0, 0, 0.9, 1.5);
Material MAGENTA_GLASS(MAGENTA, REFRACTIVE, 0.2, BLACK, 5, 0.2, 0, 0, 0, 0.9, 1.5);
Material TRANSPARENT(WHITE, REFRACTIVE, 0, BLACK, 1, 1, 0, 0, 1, 1, 1);