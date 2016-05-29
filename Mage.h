#ifndef _MAGE_H_
#define _MAGE_H_

#include "service.h"
#include "Cannon.h"
#include "Hero.h"

class Mage
{
	// Current mage's position
	float f_xPos;
	float f_yPos;
	
	// Mage's direction
	bool b_standToTheEast;

	// Mage's animation
	hgeAnimation* p_mageAnimated;

	// Mage's stopped sprite
	hgeSprite* p_mageStopped;
	
	// Is mage using magic or standing still
	bool b_isUsingMagic;

	// Pause between using magic
	float f_pauseUsingMagic;

	// Time to use magic
	float f_timeToShot;

	// Pause interval
	float f_pauseInterval;

	// mage's texture
	HTEXTURE mage_east;

	// Mage uses magic
	void useMagic();

	// Mage stands
	void stand();
	
	//Prepares mage's resources
	bool prepareResources();

	// Prepares cannon flight
	void prepareCannonFlight();

	// Private copy constructor
	Mage(const Mage&);

	// Private copy assigment operator
	Mage& operator = (const Mage&);

	// Releases allocated sources
	void release();
public:
	// Constructor
	Mage(float xPos, float yPos, bool toEast = true);

	// Destructor
	~Mage();

	// Renders mage
	void render();
};

#endif
