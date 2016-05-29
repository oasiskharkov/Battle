#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "service.h"

class Entity
{
private:
	// Current entity position
	float f_xPos;
	float f_yPos;

	// Destination
	float f_xPosDest;
	float f_yPosDest;

	// Entity animation
	hgeAnimation* p_entityAnimated;

	// Entity stopped sprite
	hgeSprite* p_entityStopped;
	
	// Movement direction
	direction e_dir;

	// Is entity moving
	bool b_isMoving;

	// Is entity fighting
	bool b_isFighting;

	// Is entity dying
	bool b_isDying;

	// Is entity dead
	bool b_isDead;

	// Textures which depend on direction
	HTEXTURE entity_north_west;
	HTEXTURE entity_north_east;
	HTEXTURE entity_south_west;
	HTEXTURE entity_south_east;

	// Pointers to hgeAnimation, which depend on movement direction
	std::map<direction, hgeAnimation*> movementAnimations;

	// Pointers to hgeAnimation, which depend on fight direction
	std::map<direction, hgeAnimation*> fightAnimations;

	// Pointers to hgeAnimation, which depend on dying direction
	std::map<direction, hgeAnimation*> dyingAnimations;

	// Pointers to hgeSprite, which depend on stopped direction
	std::map<direction, hgeSprite*> stoppedSprites;
	
	// Prepeares resources
	bool prepareResources();

	// Animates entity movement
	void move();

	// Releases all allocated memory
	void release();

	// Private copy constructor
	Entity(const Entity&);

	// Private copy assignment operator
	Entity& operator = (const Entity&);

protected:
	// Handles next entity step
	virtual void calculateNextStep() = 0;

	// Animates entity die
	virtual void die() = 0;

	// Shows entity stopped sprite
	virtual void stop() = 0;

	// Animates entity fight
	virtual void fight() = 0;

	// Setup entity movement animation
	void setupMovementAnimation();
	
	// Setup stopped entity sprite
	void setupStopped();

	// Setup fight animation
	void setupFightAnimation();

	// Setup dying animation
	void setupDyingAnimation();

	// Time span for fighting
	float fightTimer;

	// Time span for dying
	float dieTimer;
public:
	// Constructor
	Entity(float xPos, float yPos, direction dir, bool isMoving, 
		const char* entity_nw_file, const char* entity_ne_file, const char* entity_sw_file, const char* entity_se_file);

	// Virtual destructor
	virtual ~Entity();

	// Prepare entity to fight
	void prepareFight();

	// Render entity
	void render();

	// Frame calculations
	void frame();
		
	// Setup entity state (moving or standing still)
	void setEntityMovement(bool isMooving);

	// Setup entity fight state
	void setEntityFight(bool isFighting);

	// Setup entity die state
	void setEntityDie(bool isDying);

	// Setup entity dead state
	void setEntityDead(bool isDead);
	
	// Setup movement direction
	void setDirection(direction dir);
	
	// Get movement direction
	direction getDirection() const;

	// Determines direction
	void determineDirection(std::pair<float,float> dest);

	// Does entity move or stand still
	bool isMoving() const;

	// Does entity fight
	bool isFighting() const;

	// Does entity die
	bool isDying() const;

	// Is entity dead
	bool isDead() const;

	// Get current entity position
	std::pair<float, float> getCurrentPosition() const;

	// Set current entity position
	void setCurrentPosition(std::pair<float, float> position);
	
	// Check if next step is valid
	bool isNextStepValid(std::pair<float, float> nextStep) const;
	
	// Set entity destination
	void setDestination(std::pair<float, float> dest);

	// Get entity destination
	std::pair<float, float> getDestination() const;

	// Get apropriate entity animation
	hgeAnimation* getAnimation();

	// Get apropriate entity stopped sprite
	hgeSprite* getStopped();	
};

inline void Entity::setEntityMovement(bool isMoving)
{
	b_isMoving = isMoving;
}

inline void Entity::setEntityFight(bool isFighting)
{
	b_isFighting = isFighting;
}

inline void Entity::setEntityDie(bool isDying)
{
	b_isDying = isDying;
}

inline void Entity::setEntityDead(bool isDead)
{
	b_isDead = isDead;
}

inline bool Entity::isMoving() const
{
	return b_isMoving;
}

inline bool Entity::isFighting() const
{
	return b_isFighting;
}

inline bool Entity::isDying() const
{
	return b_isDying;
}

inline void Entity::setDirection(direction dir)
{
	e_dir = dir;
}

inline direction Entity::getDirection() const
{
	return e_dir;
}

inline std::pair<float, float> Entity::getCurrentPosition() const
{
	return std::pair<float, float>(f_xPos, f_yPos);
}

inline void Entity::setCurrentPosition(std::pair<float, float> position)
{
	f_xPos = position.first;
	f_yPos = position.second;
}

inline void Entity::setDestination(std::pair<float, float> dest)
{
	f_xPosDest = dest.first;
	f_yPosDest = dest.second;
}

inline std::pair<float, float> Entity::getDestination() const
{
	return std::pair<float, float>(f_xPosDest, f_yPosDest);
}

inline hgeAnimation* Entity::getAnimation()
{
	return  p_entityAnimated;
}

inline hgeSprite* Entity::getStopped()
{
	return p_entityStopped;
}

inline void Entity::setupMovementAnimation()
{
	p_entityAnimated = movementAnimations[e_dir];
}

inline void Entity::setupStopped()
{
	p_entityStopped = stoppedSprites[e_dir];
}

inline void Entity::setupFightAnimation()
{
	p_entityAnimated = fightAnimations[e_dir];
}

inline void Entity::setupDyingAnimation()
{
	p_entityAnimated = dyingAnimations[e_dir];
}

inline bool Entity::isDead() const
{
	return b_isDead;
}

#endif