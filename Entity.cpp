#include "Entity.h"
#include "Field.h"

Entity::Entity(float xPos, float yPos, direction dir, bool isMoving, const char* entity_nw_file, const char* entity_ne_file, 
					const char* entity_sw_file, const char* entity_se_file) : f_xPos(xPos), f_yPos(yPos), 
					f_xPosDest(0.0f), f_yPosDest(0.0f), p_entityAnimated(0), p_entityStopped(0), e_dir(dir), b_isMoving (isMoving), 
					b_isFighting(false), b_isDying(false), b_isDead(false), fightTimer(0.0f), dieTimer(0.0f)
{
	entity_north_west = hge->Texture_Load(entity_nw_file);
	entity_north_east = hge->Texture_Load(entity_ne_file);
	entity_south_west = hge->Texture_Load(entity_sw_file);
	entity_south_east = hge->Texture_Load(entity_se_file);
	
	if(!prepareResources())
	{
		release();
		throw game_errors::load_entity_sources;
	}
}

Entity::~Entity()
{
	release();
}

void Entity::release()
{
	hge->Texture_Free(entity_north_west);
	hge->Texture_Free(entity_north_east);
	hge->Texture_Free(entity_south_west);
	hge->Texture_Free(entity_south_east);

	for(auto entityMove : movementAnimations)
	{
		delete entityMove.second;
	}

	for(auto entityStop : stoppedSprites)
	{
		delete entityStop.second;
	}

	for(auto entityFight : fightAnimations)
	{
		delete entityFight.second;
	}

	for(auto entityDying : dyingAnimations)
	{
		delete entityDying.second;
	}
}

bool Entity::prepareResources()
{
	hgeAnimation* entityAnimated;
	hgeSprite* entityStopped;
	
	try
	{
		// Movement
		entityAnimated = new hgeAnimation(entity_south_west, 5, 8.0f, 0.0f, 0.0f, 72.0f, 72.0f);
		movementAnimations[direction::sw] = entityAnimated;

		entityAnimated = new hgeAnimation(entity_south_east, 5, 8.0f, 0.0f, 0.0f, 72.0f, 72.0f); 
		movementAnimations[direction::se] = entityAnimated;

		entityAnimated = new hgeAnimation(entity_north_west, 5, 8.0f, 0.0f, 0.0f, 72.0f, 72.0f); 
		movementAnimations[direction::nw] = entityAnimated;

		entityAnimated = new hgeAnimation(entity_north_east, 5, 8.0f, 0.0f, 0.0f, 72.0f, 72.0f); 
		movementAnimations[direction::ne] = entityAnimated;

		// Fight
		entityAnimated = new hgeAnimation(entity_south_west, 4, 6.0f, 360.0f, 0.0f, 72.0f, 72.0f);
		fightAnimations[direction::sw] = entityAnimated;

		entityAnimated = new hgeAnimation(entity_south_east, 4, 6.0f, 360.0f, 0.0f, 72.0f, 72.0f); 
		fightAnimations[direction::se] = entityAnimated;

		entityAnimated = new hgeAnimation(entity_north_west, 4, 6.0f, 360.0f, 0.0f, 72.0f, 72.0f); 
		fightAnimations[direction::nw] = entityAnimated;

		entityAnimated = new hgeAnimation(entity_north_east, 4, 6.0f, 360.0f, 0.0f, 72.0f, 72.0f); 
		fightAnimations[direction::ne] = entityAnimated;

		// Dying
		entityAnimated = new hgeAnimation(entity_south_west, 3, 5.0f, 648.0f, 0.0f, 72.0f, 72.0f);
		dyingAnimations[direction::sw] = entityAnimated;

		entityAnimated = new hgeAnimation(entity_south_east, 3, 5.0f, 648.0f, 0.0f, 72.0f, 72.0f); 
		dyingAnimations[direction::se] = entityAnimated;

		entityAnimated = new hgeAnimation(entity_north_west, 3, 5.0f, 648.0f, 0.0f, 72.0f, 72.0f); 
		dyingAnimations[direction::nw] = entityAnimated;

		entityAnimated = new hgeAnimation(entity_north_east, 3, 5.0f, 648.0f, 0.0f, 72.0f, 72.0f); 
		dyingAnimations[direction::ne] = entityAnimated;

		// Stopped
		entityStopped = new hgeSprite(entity_south_west, 0.0f, 0.0f, 72.0f, 72.0f);
		stoppedSprites[direction::sw] = entityStopped;

		entityStopped = new hgeSprite(entity_south_east, 0.0f, 0.0f, 72.0f, 72.0f); 
		stoppedSprites[direction::se] = entityStopped;

		entityStopped = new hgeSprite(entity_north_west, 0.0f, 0.0f, 72.0f, 72.0f); 
		stoppedSprites[direction::nw] = entityStopped;

		entityStopped = new hgeSprite(entity_north_east, 0.0f, 0.0f, 72.0f, 72.0f); 
		stoppedSprites[direction::ne] = entityStopped;
	}
	catch(...)
	{
		return false;
	}	
	return true;
}

void Entity::render()
{
	if(isMoving())
	{
		move();
	}
	else if(isFighting())
	{
		fight();
	}
	else if (isDying())
	{
		die();
	}
	else
	{
		stop();
	}
}

void Entity::frame()
{
	if(isMoving())
	{
		float x = f_xPos;
		float y = f_yPos;

		switch(e_dir)
		{
		case direction::nw:
			x -= dt * ENTITY_SPEED;
			y -= dt * ENTITY_SPEED / 2;
			break;
		case direction::ne:
			x += dt * ENTITY_SPEED;
			y -= dt * ENTITY_SPEED / 2;
			break;
		case direction::se:
			x += dt * ENTITY_SPEED;
			y += dt * ENTITY_SPEED / 2;
			break;
		case direction::sw:
			x -= dt * ENTITY_SPEED;
			y += dt * ENTITY_SPEED / 2;	
			break;
		}
		
		f_xPos = x;
		f_yPos = y;

		if((x < f_xPosDest && y < f_yPosDest && e_dir == direction::nw) || (x > f_xPosDest && y < f_yPosDest && e_dir == direction::ne) 
			|| (x < f_xPosDest && y > f_yPosDest && e_dir == direction::sw) || (x > f_xPosDest && y > f_yPosDest && e_dir == direction::se))
		{
			f_xPos = f_xPosDest;
			f_yPos = f_yPosDest;
			calculateNextStep();
		}
	}
}

void Entity::move()
{
	if(!p_entityAnimated->IsPlaying())
	{
		p_entityAnimated->Play();
	}
	else
	{
		p_entityAnimated->Update(dt);
	}

	p_entityAnimated->SetHotSpot(36.0f, 36.0f);
	p_entityAnimated->Render(f_xPos, f_yPos);
}

void Entity::prepareFight()
{
	setEntityFight(true);
	setEntityMovement(false);
	setupFightAnimation();
}

bool Entity::isNextStepValid(std::pair<float, float> nextStep) const
{	
	for(size_t i = 0 ; i < MAXROWCOL; i++)
	{
		for(size_t j = 0; j < MAXROWCOL; j++)
		{
			if(nextStep.first == cellCenters[i][j].first && nextStep.second == cellCenters[i][j].second)
			{	
				return true;
			}
		}
	}
	return false;
}

void Entity::determineDirection(std::pair<float,float> dest)
{
	if(f_xPos > dest.first && f_yPos > dest.second)
	{
		setDirection(direction::nw);
	}
	else if(f_xPos < dest.first &&  f_yPos > dest.second)
	{
		setDirection(direction::ne);
	}
	else if(f_xPos < dest.first && f_yPos < dest.second)
	{
		setDirection(direction::se);
	}
	else if (f_xPos > dest.first && f_yPos < dest.second)
	{
		setDirection(direction::sw);
	}
}
