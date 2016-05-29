#include "Guardian.h"
#include "Field.h"
#include "GameObjects.h"

Guardian::Guardian(float xPos, float yPos, direction dir, bool isMoving) : 
	Entity(xPos, yPos, dir, isMoving, "Resources\\guardian_north_west.png", "Resources\\guardian_north_east.png", 
	"Resources\\guardian_south_west.png", "Resources\\guardian_south_east.png"), stopTimer(0.0f)
{
	calculateNextStep();
}

Guardian::~Guardian()
{

}

void Guardian::stop()
{
	setupStopped();
	hgeSprite* spr = getStopped();
	if(spr == 0)
	{
		throw game_errors::null_pointer;
	}
	auto currentPosition = getCurrentPosition();

	spr->SetHotSpot(36.0f, 36.0f);
	spr->Render(currentPosition.first, currentPosition.second);

	stopTimer += dt;
	if(stopTimer > 3.0f)
	{
		stopTimer = 0.0f;
		setEntityMovement(true);
		calculateNextStep();
	}
}

void Guardian::fight()
{
	hgeAnimation* ani = getAnimation();
	if(ani == 0)
	{
		throw game_errors::null_pointer;
	}
	auto currentPosition = getCurrentPosition();

	fightTimer += dt;
	if(fightTimer < 3)
	{
		if(!ani->IsPlaying())
		{
			ani->Play();
		}
		else
		{
			ani->Update(dt);
		}

		ani->SetHotSpot(36.0f, 36.0f);
		ani->Render(currentPosition.first, currentPosition.second);
	}
	else
	{
		fightTimer = 0.0f;
		setEntityFight(false);
		calculateNextStep();
		setEntityMovement(true);
		setupMovementAnimation();
	}
}

void Guardian::die()
{
	hgeAnimation* ani = getAnimation();
	if(ani == 0)
	{
		throw game_errors::null_pointer;
	}
	auto currentPosition = getCurrentPosition();

	dieTimer += dt;
	if(dieTimer < 0.6)
	{
		if(!ani->IsPlaying())
		{
			ani->Play();
		}
		else
		{
			ani->Update(dt);
		}

		ani->SetHotSpot(36.0f, 36.0f);
		ani->Render(currentPosition.first, currentPosition.second);
	}
	else
	{
		dieTimer = 0.0f;
		setEntityDie(false);
		setEntityDead(true);
	}
}

void Guardian::calculateNextStep()
{
	int count = 0;
	std::pair<float, float> dest;
	direction oldDirection = getDirection();
	do
	{
		if(count > 10) 
		{
			setEntityMovement(false);
			setDirection(oldDirection);
			return;
		}
		int dir = hge->Random_Int(0, 3);
		setDirection((direction)dir);
		setupMovementAnimation();
		auto p = getCurrentPosition();

		switch(getDirection())
		{
		case direction::sw:
			dest.first = p.first - X_STEP;
			dest.second = p.second + Y_STEP;
			break;
		case direction::se:
			dest.first = p.first + X_STEP;
			dest.second = p.second + Y_STEP;
			break;
		case direction::nw:
			dest.first = p.first - X_STEP;
			dest.second = p.second - Y_STEP;
			break;
		case direction::ne:
			dest.first = p.first + X_STEP;
			dest.second = p.second - Y_STEP;
			break;	
		}
		count++;
	} while(!avoidCollision(dest));
	setDestination(dest);
}

bool Guardian::avoidCollision(std::pair<float, float> dest)
{
	if(!isNextStepValid(dest)) return false;

	std::vector<std::unique_ptr<Entity>>& guards = gameObjects->guards();

	// guard doesn't go to the cell where another guard goes or where another guard is
	for(size_t i = 0; i < guards.size(); i++)
	{
		if(guards.at(i) != 0 && this != guards.at(i).get())
		{
			auto currentPosition = guards.at(i)->getCurrentPosition();
			auto centerCell = field->getSelectedCellCenter(currentPosition.first, currentPosition.second);
			auto d = guards.at(i)->getDestination();
			if(d == dest || centerCell == dest) 
			{
				return false;
			}
		}
	}

	// guard doesn't go to the cell where the hero fights with another guard or hero dies
	if(gameObjects->hero().get() != 0 && (gameObjects->hero()->isFighting() || gameObjects->hero()->isDying()))
	{
		auto heroCurrentPosition = gameObjects->hero()->getCurrentPosition();
		auto heroCell = field->getSelectedCellCenter(heroCurrentPosition.first, heroCurrentPosition.second); 
	
		if(heroCell == dest) return false;
	}
	
	// guard doesn't go to the cell where the hole is
	auto holes = field->getHoles();
	auto hole = holes.find(dest);
	if(hole != holes.end()) return false;

	return true;
}