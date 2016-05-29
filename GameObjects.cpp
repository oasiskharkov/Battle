#include "GameObjects.h"
#include "Entity.h"
#include "Mage.h"
#include "Cannon.h"
#include "Guardian.h"
#include "Hero.h"
#include "Field.h"

GameObjects* GameObjects::instance(0);

GameObjects* GameObjects::getInstance()
{
	if(instance == 0)
	{
		instance = new GameObjects();
	}
	return instance;
}

GameObjects::GameObjects(void) : f_xLeftClick(0.0f), f_yLeftClick(0.0f) 
{
	up_guards.resize(8);
}

void GameObjects::release()
{
	instance = 0;
}

GameObjects::~GameObjects(void)
{
	release();
}

void GameObjects::prepareGameObjects()
{
	try
	{
		up_hero.reset(new Hero(cellCenters[MAXROWCOL - 1][MAXROWCOL - 1].first, cellCenters[MAXROWCOL - 1][MAXROWCOL - 1].second, direction::nw, false));

		up_guards.at(0).reset(new Guardian(cellCenters[0][MAXROWCOL - 1].first, cellCenters[0][MAXROWCOL - 1].second, direction::se));
		up_guards.at(1).reset(new Guardian(cellCenters[0][0].first, cellCenters[0][0].second, direction::se));
		up_guards.at(2).reset(new Guardian(cellCenters[MAXROWCOL - 1][0].first, cellCenters[MAXROWCOL - 1][0].second, direction::se));
		up_guards.at(3).reset(new Guardian(cellCenters[MAXROWCOL/2][MAXROWCOL/2].first, cellCenters[MAXROWCOL/2][MAXROWCOL/2].second, direction::se));
		up_guards.at(4).reset(new Guardian(cellCenters[MAXROWCOL/2][0].first, cellCenters[MAXROWCOL/2][0].second, direction::se));
		up_guards.at(5).reset(new Guardian(cellCenters[0][MAXROWCOL/2].first, cellCenters[0][MAXROWCOL/2].second, direction::se));
		up_guards.at(6).reset(new Guardian(cellCenters[MAXROWCOL/2][MAXROWCOL - 1].first, cellCenters[MAXROWCOL/2][MAXROWCOL - 1].second, direction::se));
		up_guards.at(7).reset(new Guardian(cellCenters[MAXROWCOL - 1][MAXROWCOL/2].first, cellCenters[MAXROWCOL - 1][MAXROWCOL/2].second, direction::se));

		up_leftMage.reset(new Mage(MAGE_SHIFT, 300.0f));
		up_rightMage.reset(new Mage(GAME_WIDTH - MAGE_SHIFT, 300.0f, false));	
	}
	catch(game_errors& error)
	{
		release();
		throw error;
	}
	catch(...)
	{
		release();
		throw game_errors::unkown_error;
	}
}

void GameObjects::renderMages()
{
	up_leftMage->render();
	up_rightMage->render();
}

void GameObjects::renderGuards()
{
	for(size_t i = 0; i < up_guards.size(); i++)
	{
		up_guards.at(i)->render();
	}
}

void GameObjects::renderCannons()
{
	for(size_t i = 0; i < up_cannons.size(); i++)
	{
		up_cannons.at(i)->render();
	}
}

void GameObjects::renderHero()
{
	up_hero->render();
}

void GameObjects::renderGameObjects()
{
	renderMages();
	renderGuards();
	renderCannons();
	renderHero();
}

void GameObjects::frameGuards()
{
	for(size_t i = 0; i < up_guards.size(); i++)
	{
		up_guards.at(i)->frame();
	}
}

void GameObjects::frameHero()
{
	if(!up_hero->isDead())
	{
		up_hero->frame();	
	}
	else
	{
		up_hero.reset(nullptr);
		up_cannons.erase(up_cannons.begin(), up_cannons.end());
	}
}

void GameObjects::frameCannons()
{
	auto can = up_cannons.begin();
	while(can != up_cannons.end())
	{
		if(!(*can)->isFlying())
		{
			(*can).reset(nullptr);
			can = up_cannons.erase(can);
			continue;
		}
		(*can)->frame();
		can++;
	}
}

bool GameObjects::frameGameObjects()
{
	frameGuards();
	frameHero();
	
	if(up_hero.get() == 0)
	{
		return false;
	}

	frameCannons();

	processHeroAndGuardCollision();
	processHeroAndCannonCollision();

	return true;
}

void GameObjects::processHeroAndGuardCollision()
{
	auto heroCurrentPosition = up_hero->getCurrentPosition();
	for(size_t i = 0; i < up_guards.size(); i++)
	{
		if(!up_hero->isDying() && !up_hero->isFighting() && !up_guards.at(i)->isFighting())
		{
			auto guardianCurrentPosition = up_guards.at(i)->getCurrentPosition();
			if(fabs(heroCurrentPosition.first - guardianCurrentPosition.first) < X_STEP
				&& fabs(heroCurrentPosition.second - guardianCurrentPosition.second) < Y_STEP)
			{
				setEntitiesFightPositions(up_guards.at(i).get());
				up_hero->prepareFight();
				up_guards.at(i)->prepareFight();
				return;
			}
		}
	}
}

void GameObjects::processHeroAndCannonCollision()
{
	auto heroPosition = up_hero->getCurrentPosition();
	for(size_t i = 0; i < up_cannons.size(); i++)
	{
		auto cannonPosition = up_cannons.at(i)->getCurrentPosition();
		float distance = sqrt((cannonPosition.first - heroPosition.first) * (cannonPosition.first - heroPosition.first) + 
			(cannonPosition.second - heroPosition.second) * (cannonPosition.second - heroPosition.second));
		if(distance < CANNON_HERO_COLLISION)
		{
			dynamic_cast<Hero*>(up_hero.get())->prepareDie();
			return;
		}
	}
}

void GameObjects::prepareHeroMovementToSelectedCell()
{
	hge->Input_GetMousePos(&f_xLeftClick, &f_yLeftClick);
	auto center = field->getSelectedCellCenter(f_xLeftClick, f_yLeftClick);
	auto holes = field->getHoles();
	if(center.first != -1.0f && center.second != -1.0f && holes.find(center) == holes.end()
		&& !up_hero->isMoving() && !up_hero->isFighting() && !up_hero->isDying())
	{
		dynamic_cast<Hero*>(up_hero.get())->prepareMovement(center);
		field->setSelection(center.first, center.second);
	}
}

void GameObjects::setEntitiesFightPositions(Entity* guard)
{
	auto heroCurrentPosition = up_hero->getCurrentPosition();
	auto guardCurrentPosition = guard->getCurrentPosition();

	up_hero->setCurrentPosition(field->getSelectedCellCenter(heroCurrentPosition.first, heroCurrentPosition.second));
	guard->setCurrentPosition(field->getSelectedCellCenter(guardCurrentPosition.first, guardCurrentPosition.second));

	heroCurrentPosition = up_hero->getCurrentPosition();
	guardCurrentPosition = guard->getCurrentPosition();

	if((fabs(guardCurrentPosition.first - heroCurrentPosition.first) != X_STEP 
		&& fabs(guardCurrentPosition.second - heroCurrentPosition.second) != Y_STEP) || 
		(fabs(guardCurrentPosition.first - heroCurrentPosition.first) == 0.0f &&
		fabs(guardCurrentPosition.second - heroCurrentPosition.second) == 0.0f))
	{
		guard->setCurrentPosition(guard->getDestination());
	}

	up_hero->determineDirection(guard->getCurrentPosition());
	guard->determineDirection(up_hero->getCurrentPosition());
}




	
	
