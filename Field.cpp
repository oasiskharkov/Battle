#include "Field.h"
#include "Entity.h"
#include "Hero.h"
#include "Guardian.h"
#include "GameObjects.h"

Field* Field::instance(0);

Field* Field::getInstance()
{
	if(instance == 0)
	{
		instance = new Field();
	}
	return instance;
}

Field::Field() : f_xSelection(0.0f), f_ySelection(0.0f), f_xCursor(0.0f), f_yCursor(0.0f),
	f_xRightClick(0.0f), f_yRightClick(0.0f)
{
	h_floorTileTex = hge->Texture_Load("Resources\\square.png");				

	h_cursorTex = hge->Texture_Load("Resources\\cursor.png");

	h_backgroundTex = hge->Texture_Load("Resources\\background.png");

	h_towerTex = hge->Texture_Load("Resources\\tower.png");

	h_princessTex = hge->Texture_Load("Resources\\princess.png");

	h_wallLeftTex = hge->Texture_Load("Resources\\stone_wall_left.png");

	h_wallRightTex = hge->Texture_Load("Resources\\stone_wall_right.png");

	h_holeTex = hge->Texture_Load("Resources\\hole.png");

	h_cellSelectionTex = hge->Texture_Load("Resources\\green_cross.png");

	h_portraitTex = hge->Texture_Load("Resources\\knight.png");

	h_patternTex = hge->Texture_Load("Resources\\pattern.png");

	calculateCells();

	prepareSources();

	// If one of the source files is not found, free all field sources and throw an error message.
	if(!up_floorTile.get() || !up_cursor.get() || !up_background.get() || !up_tower.get() || !up_princess.get() ||
		!up_wallLeft.get() || !up_wallRight.get() || !up_hole.get() || !up_cellSelectionAni.get() ||
		!up_heroPicture.get() || !up_pattern.get() || !up_font.get())
	{		
		release();
		throw game_errors::load_field_sources;
	}		
}

Field::~Field(void)
{
	release();
}

bool Field::prepareSources()
{
	try
	{
		up_floorTile.reset(new hgeSprite(h_floorTileTex, 0.0f, 0.0f, 128.0f, 128.0f));

		up_cursor.reset(new hgeSprite(h_cursorTex, 0.0f, 0.0f, 16.0f, 16.0f));

		up_background.reset(new hgeSprite(h_backgroundTex, 0.0f, 0.0f, 32.0f, 32.0f));

		up_tower.reset(new hgeSprite(h_towerTex, 0.0f, 0.0f, 175.0f, 175.0f));

		up_princess.reset(new hgeSprite(h_princessTex, 0.0f, 0.0f, 20.0f, 36.0f));

		up_wallLeft.reset(new hgeSprite(h_wallLeftTex, 0.0f, 0.0f, 79.0f, 126.0f));

		up_wallRight.reset(new hgeSprite(h_wallRightTex, 0.0f, 0.0f, 79.0f, 126.0f));

		up_hole.reset(new hgeSprite(h_holeTex, 0.0f, 0.0f, 116.0f, 69.0f));

		up_cellSelectionAni.reset(new hgeAnimation(h_cellSelectionTex, 4, 5.0f, 0.0f, 0.0f, 32.0f, 32.0f));

		up_heroPicture.reset(new hgeSprite(h_portraitTex, 0.0f, 0.0f, 145.0f, 164.0f));

		up_pattern.reset(new hgeSprite(h_patternTex, 0.0f, 0.0f, 256.0f, 252.0f));

		up_font.reset(new hgeFont("Resources\\font1.fnt"));
	}
	catch(...)
	{
		return false;
	}
	return true;
}

void Field::release()
{
	hge->Texture_Free(h_floorTileTex);
	hge->Texture_Free(h_cursorTex);
	hge->Texture_Free(h_backgroundTex);
	hge->Texture_Free(h_towerTex);
	hge->Texture_Free(h_princessTex);
	hge->Texture_Free(h_wallLeftTex);
	hge->Texture_Free(h_wallRightTex);
	hge->Texture_Free(h_holeTex);
	hge->Texture_Free(h_cellSelectionTex);
	hge->Texture_Free(h_portraitTex);
	hge->Texture_Free(h_patternTex);

	s_holes.clear();

	instance = 0;
}

void Field::renderBackground()
{
	float tile_x = 0, tile_y = 0;
	for(size_t i = 0; i < GAME_WIDTH/X_STEP; i++)
	{
		for(size_t j = 0; j < GAME_HEIGHT/Y_STEP; j++)
		{
			up_background->RenderStretch(tile_x, tile_y, tile_x + X_STEP, tile_y + X_STEP);
			tile_x += X_STEP;
		}
		tile_y += X_STEP;
		tile_x = 0.0f;
	}
}

void Field::renderPattern()
{
	// left top corner
	up_pattern->SetFlip(false, false);
	up_pattern->SetHotSpot(0.0f, 0.0f);
	up_pattern->Render(0.0f, 0.0f);
	
	// right top corner
	up_pattern->SetFlip(true, false);
	up_pattern->SetHotSpot(256.0f, 0.0f);
	up_pattern->Render((float)GAME_WIDTH, 0.0f);
	
	// left bottom corner
	up_pattern->SetFlip(false, true);
	up_pattern->SetHotSpot(0.0f, 252.0f);
	up_pattern->Render(0.0f, (float)GAME_HEIGHT);

	// right bottom corner
	up_pattern->SetFlip(true, true);
	up_pattern->SetHotSpot(256.0f, 252.0f);
	up_pattern->Render((float)GAME_WIDTH, (float)GAME_HEIGHT);
}
	
void Field::renderTower()
{
	up_tower->SetHotSpot(87.0f, 87.0f);
	up_tower->RenderEx(GAME_WIDTH/2.0f, 100.0f, 0.0f, 1.3f, 1.3f); 
}

void Field::renderPrincess()
{
	up_princess->SetHotSpot(10.0f, 18.0f);
	up_princess->Render(GAME_WIDTH/2.0f + 4.0f, 30.0f);
}
	
void Field::renderWalls()
{
	up_wallLeft->SetHotSpot(44.0f, 103.0f);
	up_wallRight->SetHotSpot(34.0f, 103.0f);

	float x_wall_left = 395.0f, x_wall_right = 629.0f, y_wall = 218.0f;
	for(int i = 0; i < 7; i++)
	{
		up_wallLeft->Render(x_wall_left, y_wall);
		up_wallRight->Render(x_wall_right, y_wall);

		x_wall_left -= 61.0f;
		x_wall_right += 61.0f;
		y_wall += 30.5f;
	}
}

void Field::renderField()
{
	float rowstartx = GAME_WIDTH/2.0f - X_STEP;
	float rowstarty = Y_FIELD_START;
	for (int maprow = 0; maprow < MAXROWCOL; maprow++)
	{
		float drawx = rowstartx;
		float drawy = rowstarty;
		for (int mapcol = 0; mapcol < MAXROWCOL; mapcol++)
		{
			up_floorTile->Render4V(drawx + X_STEP, drawy, drawx + 2 * X_STEP, drawy + Y_STEP, drawx + X_STEP, drawy + X_STEP, drawx, drawy + Y_STEP);
			drawx += X_STEP; 
			drawy += Y_STEP;	
		}
		rowstartx -= X_STEP; 
		rowstarty += Y_STEP;
	}
}

void Field::renderHoles()
{
	up_hole->SetHotSpot(58.0f, 35.0f);
	
	typedef std::set<std::pair<float,float>>::const_iterator CI;
	for(CI i = s_holes.begin(); i != s_holes.end(); i++)
	{
		up_hole->RenderEx((*i).first, (*i).second, 0.0f, 0.48f);
	}
}

void Field::renderCellSelection()
{
	if(gameObjects->hero().get() != 0)
	{
		if(gameObjects->hero()->isMoving())
		{
			if(!up_cellSelectionAni->IsPlaying())
			{
				up_cellSelectionAni->Play();
			}
			else 
			{
				up_cellSelectionAni->Update(dt);
			}

			up_cellSelectionAni->Render4V(f_xSelection, f_ySelection - Y_STEP, f_xSelection + X_STEP, f_ySelection,
				f_xSelection, f_ySelection + Y_STEP, f_xSelection - X_STEP, f_ySelection);
		}
	}
}

void Field::renderPortrait()
{
	up_heroPicture->SetHotSpot(145.0f, 0);
	up_heroPicture->Render(GAME_WIDTH - 40.0f, 40.0f);
}

void Field::renderScene()
{
	renderBackground();
	renderPattern();
	renderPortrait();
	renderGameStatistics();
	renderTower();
	renderPrincess();
	renderField();
	renderWalls();
	renderHoles();
	renderCellSelection();
}

void Field::renderFont(int a, int r, int g, int b, float scale, float x, float y, int align, const std::string& str)
{
	up_font->SetColor(ARGB(a, r, g, b));
	up_font->SetScale(scale);
	up_font->Render(x, y, align, str.c_str()); 
}

void Field::renderGameStatistics()
{
	std::string str("lives: ");
	str += std::to_string(lives); 
	renderFont(255, 128, 128, 128, 1.0f, GAME_WIDTH - 150.0f, 210.0f, HGETEXT_LEFT, str);
}

void Field::renderCursor()
{
	hge->Input_GetMousePos(&f_xCursor, &f_yCursor);
	up_cursor->Render(f_xCursor, f_yCursor);
}

void Field::insertHole()
{
	hge->Input_GetMousePos(&f_xRightClick, &f_yRightClick);
	auto p = getSelectedCellCenter(f_xRightClick, f_yRightClick);
	if(validateHoleInstalation(p))
	{
		s_holes.insert(p);
	}
	else
	{
		auto findedHole = s_holes.find(p); 
		if(findedHole != s_holes.end())
		{
			s_holes.erase(findedHole);
		}
	}
}

bool Field::validateHoleInstalation(std::pair<float, float> instalationPoint)
{
	// If such center is exists
	if( instalationPoint.first == -1.0f && instalationPoint.second == -1.0f)
	{
		return false;
	}
	
	// Cannot place hole if there is a guardian on this cell
	for(size_t i = 0; i < gameObjects->guards().size(); i++)
	{
		auto guardPosition =  gameObjects->guards().at(i)->getCurrentPosition();
		if(X_STEP > fabs(instalationPoint.first - guardPosition.first) 
			&& Y_STEP > fabs(instalationPoint.second - guardPosition.second))
		{
			return false;
		}
	}

	// Cannot place hole if there is a hero on this cell
	auto heroPosition = gameObjects->hero()->getCurrentPosition();
	if(X_STEP > fabs(instalationPoint.first - heroPosition.first) &&
		Y_STEP > fabs(instalationPoint.second - heroPosition.second))
	{
		return false;
	}

	// Cannot place hole if the hole is already placed
	if(s_holes.find(instalationPoint) != s_holes.end())
	{
		return false;
	}

	return true;
}

std::pair<float,float> Field::getSelectedCellCenter(float x, float y)
{
	for(size_t i = 0; i < MAXROWCOL; i++)
	{
		for(size_t j = 0; j < MAXROWCOL; j++)
		{
			float x1, x2, y1, y2, x3, y3, x4, y4;
			x1 = cellCenters[i][j].first - X_STEP;
			y1 = cellCenters[i][j].second;
			x2 = cellCenters[i][j].first;
			y2 = cellCenters[i][j].second + Y_STEP;
			x3 = cellCenters[i][j].first + X_STEP;
			y3 = cellCenters[i][j].second;
			x4 = cellCenters[i][j].first;
			y4 = cellCenters[i][j].second - Y_STEP;

			if((y < ((x - x1) * (y2 - y1)/(x2 - x1)) + y1) && (y < ((x - x2) * (y3 - y2)/(x3 - x2)) + y2)  
				&& (y > ((x - x1) * (y4 - y1)/(x4 - x1)) + y1) && (y > ((x - x4)*(y3 - y4)/(x3 - x4)) + y4))
			{
				return std::pair<float, float>(cellCenters[i][j].first, cellCenters[i][j].second);
			}
		}		
	}
	return std::pair<float,float>(-1.0f,-1.0f);
}

void Field::calculateCells()
{
	float rowstartx = GAME_WIDTH/2 - X_STEP;
	float rowstarty = Y_FIELD_START;
	for (int maprow = 0; maprow < MAXROWCOL; maprow++)
	{
		float drawx = rowstartx;
		float drawy = rowstarty;
		for (int mapcol = 0; mapcol < MAXROWCOL; mapcol++)
		{
			cellCenters[maprow][mapcol] = std::pair<float,float>(drawx + X_STEP, drawy + Y_STEP);
			drawx += X_STEP; 
			drawy += Y_STEP;
		}
		rowstartx -= X_STEP; 
		rowstarty += Y_STEP;
	}
}