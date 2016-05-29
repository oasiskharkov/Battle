#include "Hero.h"
#include "Field.h"
#include "GameObjects.h"

Hero::Hero(float xPos, float yPos, direction dir, bool isMoving) : 
	Entity(xPos, yPos, dir, isMoving, "Resources\\hero_north_west.png", "Resources\\hero_north_east.png", 
	"Resources\\hero_south_west.png", "Resources\\hero_south_east.png")
{

}

Hero::~Hero()
{

}

void Hero::stop()
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
}

void Hero::fight()
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
		
		setupDyingAnimation();
		setEntityDie(true);
	}
}

void Hero::die()
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
		lives--;
		setEntityDie(false);
		setEntityDead(true);
	}
}

void Hero::prepareDie()
{
	if(gameObjects->hero()->isDying()) 
	{
		return;
	}
	if(gameObjects->hero()->isFighting()) 
	{
		setEntityFight(false);
	}
	if(gameObjects->hero()->isMoving())
	{
		setEntityMovement(false);
	}
	setEntityDie(true);	
	setupDyingAnimation();
}

bool Hero::calculatePath(std::pair<float, float> dest)
{
	auto currentPosition = getCurrentPosition();
	auto destination = dest;

	// declares dictionary with visited cells of the field
	std::map<std::pair<float, float>, bool> visited;

	// sets flags for all cells as false
	for(size_t i = 0; i < MAXROWCOL; i++)
	{
		for(size_t j = 0; j < MAXROWCOL; j++)
		{
			visited[cellCenters[i][j]] = false;
		}
	}

	// marks current position as visited
	visited[currentPosition] = true;

	// declares queue to store analyzing cells
	std::queue<std::pair<float,float>> analyzing;

	// enqueues the current position
	analyzing.push(currentPosition);

	// shows if destination was found
	bool found = false;

	// shows from which cell we got to current
	std::map<std::pair<float,float>, std::pair<float,float>> cameFrom;

	// All holes
	auto holes = field->getHoles();

	// operates until all nodes are not analyzed
	while(analyzing.size())
	{
		// gets front cell
		std::pair<float,float> frontOfQueue = analyzing.front();
		
		// dequeues front element
		analyzing.pop();

		// valid cells - neighbors
		std::vector<std::pair<float, float>> neighbors;
		
		// fills neighbors
		fillNeighbors(neighbors, frontOfQueue);

		// Processes all neighbor cells
		for(auto neighbor : neighbors)
		{
			// checks is analyzing cell connected with other and other cell is not visited
			if(isCellsConnected(frontOfQueue, neighbor, holes) && !visited[neighbor])
			{
				// marks the current cell as visited
				visited[neighbor] = true;
				// saves the link to the cell from which we came to current
				cameFrom[neighbor] = frontOfQueue;
				// if the destination was found sets the flag as true and breaks loop
				if(neighbor == destination)
				{
					found = true;		
					break;
				}
				// enqueue analyzing cell
				analyzing.push(neighbor);
			}
		}	
		
		// if destination cell was found breaks main loop
		if(found)
		{
			break;
		}
	}
	
	// fills in the path from the current position to the destination
	if(found)
	{
		// begins from last cell
		std::pair<float, float> lastCell = destination;

		// starts endless cycle
		while(true)
		{
			// adds last cell
			path.push_back(lastCell);
			// breaks the loop if the destination was reached
			if(lastCell == currentPosition)
			{
				break;
			}
			// gets the cell from which reached the last
			lastCell = cameFrom[lastCell];
		}
		
		// removes current position
		path.pop_back();
	}
	
	return found;
}

void Hero::fillNeighbors(std::vector<std::pair<float, float>>& neighbors, std::pair<float, float> point)
{
	auto p1 = std::pair<float,float>(point.first + X_STEP, point.second + Y_STEP);
	if(isPointInsideField(p1))
	{
		neighbors.push_back(p1);
	}

	auto p2 = std::pair<float,float>(point.first - X_STEP, point.second + Y_STEP);
	if(isPointInsideField(p2))
	{
		neighbors.push_back(p2);
	}

	auto p3 = std::pair<float,float>(point.first + X_STEP, point.second - Y_STEP);
	if(isPointInsideField(p3))
	{
		neighbors.push_back(p3);
	}

	auto p4 = std::pair<float,float>(point.first - X_STEP, point.second - Y_STEP);
	if(isPointInsideField(p4))
	{
		neighbors.push_back(p4);
	}
}

bool Hero::isPointInsideField(std::pair<float, float> point)
{
	float x = point.first;
	float y = point.second;

	float x1 = GAME_WIDTH / 2.0f;
	float y1 = Y_FIELD_START;

	float x2 = GAME_WIDTH;
	float y2 = Y_FIELD_START + ( Y_STEP * MAXROWCOL);

	float x3 = GAME_WIDTH / 2.0f;
	float y3 = Y_FIELD_START + (2 * Y_STEP * MAXROWCOL);

	float x4 = 0.0f;
	float y4 = Y_FIELD_START + ( Y_STEP * MAXROWCOL);

	if((y > ((x - x1) * (y2 - y1)/(x2 - x1)) + y1) && (y < ((x - x2) * (y3 - y2)/(x3 - x2)) + y2)  
				&& (y > ((x - x1) * (y4 - y1)/(x4 - x1)) + y1) && (y < ((x - x4)*(y3 - y4)/(x3 - x4)) + y4))
	{
		return true;
	}
	return false;
}

bool Hero::isCellsConnected(std::pair<float, float> cellFrom, std::pair<float,float> cellTo, const std::set<std::pair<float, float>>& holes) const
{
	return holes.find(cellTo) == holes.end();
}

void Hero::calculateNextStep()
{
	if(!path.empty())
	{
		path.pop_back();
		if(!path.size())
		{
			setEntityMovement(false);
			return;
		}  
		auto nextCell = path.back();
		auto holes = field->getHoles();
		if(holes.find(nextCell) != holes.end())
		{
			auto destination = path.front();
			path.clear();
			prepareMovement(destination);
		}
		else
		{
			setDestination(nextCell);
			determineDirection(getDestination());
			setupMovementAnimation();
			setEntityMovement(true);	
		}
	}
}

void Hero::prepareMovement(std::pair<float, float> destination)
{
	if(calculatePath(destination))
	{
		auto firstCell = path.back();
		setDestination(firstCell);
		determineDirection(getDestination());
		setupMovementAnimation();
		setEntityMovement(true);
	}
	else
	{
		setEntityMovement(false);
	}
}

