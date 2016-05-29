#ifndef _UTIL_H_
#define _UTIL_H_

#include <vector>
#include <set>
#include <hge.h>
#include <hgesprite.h>
#include <hgeanim.h>
#include <hgegui.h>
#include <utility>
#include <cstdlib>
#include <ctime>

#define MAXROWCOL 10

enum direction {nw, ne, sw, se };

HGE *hge = 0;
hgeSprite* heroStopped = 0;
hgeAnimation *ani = 0;
hgeAnimation *selAni = 0;
hgeSprite *floorTile;
//hgeSprite *hero;
hgeSprite *tower;
hgeSprite *cursor;
hgeSprite *wallLeft;
hgeSprite *wallRight;

HTEXTURE floorTex;
HTEXTURE heroText_south_west;
HTEXTURE heroText_south_east;
HTEXTURE heroText_north_west;
HTEXTURE heroText_north_east;
HTEXTURE towerTex;
HTEXTURE cursorTex;
HTEXTURE cellSelectionTex;
HTEXTURE wallLeftTex;
HTEXTURE wallRightTex;

const float speed = 50;
std::set<std::pair<float,float>> towers;
std::pair<float,float> cellCenters[MAXROWCOL][MAXROWCOL];
bool oldLeftKeyState = true;
bool oldRightKeyState = true;
float xl_click = 400.0f, yl_click = 480.0f;
float xr_click = 0.0f, yr_click = 0.0f;
float x_cursor = 0.0f, y_cursor = 0.0f;
float x = 400.0f;
float y = 480.0f;
float dt;
bool isMoving = false;
direction dir = direction::nw;

typedef std::set<std::pair<float,float>>::const_iterator CI;

#endif