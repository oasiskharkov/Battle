
#ifndef _HERO_H_
#define _HERO_H_

#include "Entity.h"

class Hero : public Entity
{
private:
	// Map with hero path
	std::vector<std::pair<float,float>> path; 

	// Calculates hero path using breadth-first search
	bool calculatePath(std::pair<float, float> dest);

	// Checks if the cells are neighboring
	bool Hero::isCellsConnected(std::pair<float, float> cellFrom, std::pair<float,float> cellTo, 
		const std::set<std::pair<float, float>>& holes) const;
	// Defines cell neighbors
	void fillNeighbors(std::vector<std::pair<float, float>>& neighbors, std::pair<float, float> point);

	// Defines is the point inside the field
	bool isPointInsideField(std::pair<float, float>);

protected:
	// Handles next hero step
	void calculateNextStep();

	// Shows died hero
	void die() override;

	// Shows fighting hero
	void fight() override;

	// Shows stopped hero
	void stop() override;
public:
	// Constructor
	Hero(float xPos, float yPos, direction dir, bool isMoving = false);

	// Destructor
	~Hero();
	 
	// Prepares hero movement
	void prepareMovement(std::pair<float, float> destination);

	// Prepares hero die
	void prepareDie();

	// Gets container with path cells
	const std::vector<std::pair<float,float>>& getHeroPath() const; 
};

inline const std::vector<std::pair<float,float>>& Hero::getHeroPath() const
{
	return path;
}

#endif