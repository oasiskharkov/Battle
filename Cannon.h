#ifndef _CANNON_H_
#define _CANNON_H_

#include "service.h"

class Cannon
{
	// Current cannon's position
	float f_xPos;
	float f_yPos;

	// Cannon's destination
	float f_xDest;
	float f_yDest;

	// Cannon's start position
	float f_xStart;
	float f_yStart;

	// Is cannon flying
	bool b_isFlying;
	
	// Animated cannon
	hgeAnimation* p_cannonAnimated;

	// Cannon's flight delta
	float f_delta;

	// Cannon's texture
	HTEXTURE h_tCannon;

	// Cannon's flight distance
	float f_distance;

	// Prepares cannon resources
	bool prepareResources();
	
	// Cannon flies
	void fly();

	// Releases allocated memory
	void release();

	// Private copy constructor
	Cannon(const Cannon&);

	// Private copy assignment operator
	Cannon& operator = (const Cannon&);
public:
	// Constructor
	Cannon(float xPos, float yPos, float xDest, float yDest, bool isFlying);

	// Destructor
	~Cannon();

	// Renders Cannon
	void render();

	// Calculates cannon frame
	void frame();

	// Is cannon flying
	bool isFlying() const;

	// Gets current position
	std::pair<float, float> getCurrentPosition() const;
};

inline bool Cannon::isFlying() const
{
	return b_isFlying;
}

inline std::pair<float, float>  Cannon::getCurrentPosition() const
{
	return std::pair<float, float>(f_xPos, f_yPos);
}

#endif


