
#ifndef _GUARDIAN_H_
#define _GUARDIAN_H_

#include "Entity.h"

class Guardian : public Entity
{
private:
	// The time thar guard doesn't go
	float stopTimer;

	// Checks is there a collision with other guardian
	bool avoidCollision(std::pair<float, float> dest);

protected:
	// Handles next hero step
	void calculateNextStep();

	// Shows fighting guardian
	void fight() override;

	// Shows dying guardian
	void die() override;

	// Shows stopped guardian
	void stop() override;
public:
	// Constructor
	Guardian(float xPos, float yPos, direction dir, bool isMoving = true);
	// Destructor
	~Guardian();
};

#endif

