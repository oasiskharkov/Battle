#ifndef _GAME_OBJECTS_H_
#define _GAME_OBJECTS_H_

#include "service.h"

class Mage;
class Cannon;
class Entity;

class GameObjects
{
private:

	// X left mouse click coordinate
	float f_xLeftClick;

	// Y left mouse click coordinate
	float f_yLeftClick;

	// Unique pointer to the hero
	std::unique_ptr<Entity> up_hero;

	// Container with unique pointers to the guards
	std::vector<std::unique_ptr<Entity>> up_guards;
	
	// Unique pointer to the left mage
	std::unique_ptr<Mage> up_leftMage;

	// Unique pointer to the right mage
	std::unique_ptr<Mage> up_rightMage;

	// Container with unique pointers to the cannons
	std::vector<std::unique_ptr<Cannon>> up_cannons;

	// Renders both mages 
	void renderMages();

	// Renders all guards
	void renderGuards();

	// Renders all cannons
	void renderCannons();

	// Renders hero
	void renderHero();

	// Frames guards
	void frameGuards();

	// Frames hero
	void frameHero();

	// Frames cannons
	void frameCannons();

	// Process collision of the hero and the guardian
	void processHeroAndGuardCollision();
	
	// Process collision of the hero and the cannon
	void processHeroAndCannonCollision();
	
	// Set entities fight positions 
	void setEntitiesFightPositions(Entity* guard);

	// Private constructor
	GameObjects(void);

	// Field intstance
	static GameObjects* instance;
public:
	
	// Destructor
	~GameObjects(void);
	
	// Returns an instance of the GameObjects object
	static GameObjects* getInstance();

	// Renders all game objects
	void renderGameObjects();

	// Frames all game objects
	bool frameGameObjects();

	// Prepares game objects
	void prepareGameObjects();

	// Prepares hero movement to the selected cell
	void prepareHeroMovementToSelectedCell();

	// Gets unique pointer to the hero
	std::unique_ptr<Entity>& hero();

	// Gets container with unique pointers to the guards
	std::vector<std::unique_ptr<Entity>>& guards();

	// Gets container with unique pointers to the cannons
	std::vector<std::unique_ptr<Cannon>>& cannons();

	// Releases all allocated memory
	void release();
};

inline std::unique_ptr<Entity>& GameObjects::hero()
{
	return up_hero;
}

inline std::vector<std::unique_ptr<Entity>>& GameObjects::guards()
{
	return up_guards;
}

inline std::vector<std::unique_ptr<Cannon>>& GameObjects::cannons()
{
	return up_cannons;
}

#endif

