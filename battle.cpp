#include "service.h"
#include "Hero.h"
#include "Guardian.h"
#include "Mage.h"
#include "Cannon.h"
#include "Field.h"
#include "GameObjects.h"

HGE* hge = 0;

Field* field = 0;
GameObjects* gameObjects = 0;

bool oldLeftKeyState = true;
bool oldRightKeyState = true;
bool toExit = true;

float dt;

unsigned short lives = 20;

std::pair<float,float> cellCenters[MAXROWCOL][MAXROWCOL];

void ShowErrorMessageIfAnyAndSafeExit(const std::string& error = "");
void ReleaseGameSources();
 
bool FrameFunc()
{
	// Get timer delta
	dt = hge->Timer_GetDelta();

	// Press ESC to exit
	if (hge->Input_GetKeyState(HGEK_ESCAPE))
	{
		return true;
	}

	// Press Enter to continue
	if(hge->Input_GetKeyState(HGEK_ENTER))
	{
		if(lives > 0 && gameObjects->hero() == 0)
		{
			toExit = false;
			return true;
		}
	}

	// If hero is dead or cell destination was riched
	if(gameObjects->hero() == 0 || gameObjects->hero()->getCurrentPosition() == cellCenters[0][0])
	{
		return false;
	}

	// Frames game objects
	if(!(gameObjects->frameGameObjects()))
	{
		return false;
	}
	
	// Handles left button mouse click
	if (oldLeftKeyState == false && hge->Input_GetKeyState(HGEK_LBUTTON))
	{
		gameObjects->prepareHeroMovementToSelectedCell();
	}
	oldLeftKeyState = hge->Input_GetKeyState(HGEK_LBUTTON);
	
	// Handles right button mouse click
	if (oldRightKeyState == false && hge->Input_GetKeyState(HGEK_RBUTTON))
	{
		field->insertHole();
	}
	oldRightKeyState = hge->Input_GetKeyState(HGEK_RBUTTON);

	return false;
}
 
bool RenderFunc()
{
	hge->Gfx_BeginScene();
	
	// Renders scene
	field->renderScene();

	// Player loses
	if(gameObjects->hero() == 0)
	{
		if(lives == 0)
		{
			field->renderFont(255, 255, 0, 0, 3.0f, GAME_WIDTH / 2.0f, GAME_HEIGHT / 2.0f, HGETEXT_CENTER, "GAME OVER"); 
			field->renderFont(255, 255, 0, 0, 1.0f, GAME_WIDTH / 2.0f, GAME_HEIGHT / 2.0f + 100.0f, HGETEXT_CENTER, "Press Esc to exit"); 
		}
		else
		{
			field->renderFont(255, 255, 0, 0, 3.0f, GAME_WIDTH / 2.0f, GAME_HEIGHT / 2.0f, HGETEXT_CENTER, "LOSE");
			field->renderFont(255, 255, 0, 0, 1.0f, GAME_WIDTH / 2.0f, GAME_HEIGHT / 2.0f + 100.0f, HGETEXT_CENTER, "Press Enter to try again"); 
		}
		hge->Gfx_EndScene();
		return false;
	}

	// Player wins
	if(gameObjects->hero()->getCurrentPosition() == cellCenters[0][0])
	{
		field->renderFont(255, 0, 255, 0, 3.0f, GAME_WIDTH / 2.0f, GAME_HEIGHT / 2.0f, HGETEXT_CENTER, "WIN"); 
		field->renderFont(255, 0, 255, 0, 1.0f, GAME_WIDTH / 2.0f, GAME_HEIGHT / 2.0f + 100.0f, HGETEXT_CENTER, "Press Esc to exit"); 
		hge->Gfx_EndScene();
		return false;
	}

	// Renders game objects
	gameObjects->renderGameObjects();
		
	// Renders mouse cursor
	field->renderCursor();

	hge->Gfx_EndScene();
	
	return false;
}
 
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Checks for memory leak if debug build
	#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	
	#endif

	hge = hgeCreate(HGE_VERSION);
		
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, GAME_WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT, GAME_HEIGHT);
	hge->System_SetState(HGE_SCREENBPP, 32);
	hge->System_SetState(HGE_TITLE, "Battle");
	
	if(hge->System_Initiate())
	{		
		while(lives > 0)
		{
			try
			{
				field = Field::getInstance();
				gameObjects = GameObjects::getInstance();
				gameObjects->prepareGameObjects();
			}
			catch(game_errors& gerr)
			{
				switch(gerr)
				{
				case game_errors::load_field_sources:
					ShowErrorMessageIfAnyAndSafeExit("Can't load field sources.");
					return 0;
				case game_errors::load_cannon_sources:
					ShowErrorMessageIfAnyAndSafeExit("Can't load cannon sources.");
					return 0;
				case game_errors::load_entity_sources:
					ShowErrorMessageIfAnyAndSafeExit("Can't load entity sources.");
					return 0;
				case game_errors::load_mage_sources:
					ShowErrorMessageIfAnyAndSafeExit("Can't load mage sources.");
					return 0;
				}
			}
			catch(...)
			{
				ShowErrorMessageIfAnyAndSafeExit("Something goes wrong.");
				return 0;
			}	

			hge->System_Start();

			if(toExit)
			{
				break;
			}
			
			toExit = true;
			ReleaseGameSources();
		}	
	}

	ShowErrorMessageIfAnyAndSafeExit();
	return 0;
}

void ReleaseGameSources()
{
	delete gameObjects;
	delete field;
}

void ShowErrorMessageIfAnyAndSafeExit(const std::string& error)
{
	if(!error.empty())
	{
		MessageBox(NULL, error.c_str(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}
	ReleaseGameSources();
	hge->System_Shutdown();
	hge->Release();
}



