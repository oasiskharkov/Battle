#ifndef _SERVICE_H_
#define _SERVICE_H_

#include <hge.h>
#include <hgesprite.h>
#include <hgeanim.h>
#include <hgefont.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <memory>
#include <string>
#include <iterator>

const float X_STEP = 32.0f;
const float Y_STEP= 16.0f;
const float ENTITY_SPEED = 50.0f; 
const float CANNON_SPEED = 100.0f;
const float CANNON_SHIFT = 20.0f;
const float MAGE_SHIFT = 20.0f;
const float CANNON_HERO_COLLISION = 20.0f;
const float Y_FIELD_START = 170.0f; 
const int MAXROWCOL = 16;
const int GAME_WIDTH = 1024;
const int GAME_HEIGHT = 768;

class GameObjects;
class Field;

enum direction {nw, ne, sw, se };
enum game_errors {load_field_sources, load_entity_sources, load_mage_sources, load_cannon_sources, null_pointer, unkown_error};

extern HGE* hge;

extern Field* field;
extern GameObjects* gameObjects;

extern unsigned short lives;
extern float dt;
extern std::pair<float,float> cellCenters[MAXROWCOL][MAXROWCOL];

#endif