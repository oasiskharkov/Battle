#include "Cannon.h"

Cannon::Cannon(float xPos, float yPos, float xDest, float yDest, bool isFlying) :
	f_xPos(xPos), f_yPos(yPos), f_xDest(xDest), f_yDest(yDest), f_xStart(xPos), f_yStart(yPos),
	b_isFlying(isFlying), p_cannonAnimated(0), 
	f_delta(0.0f)
{
	h_tCannon = hge->Texture_Load("Resources\\cannon.png");

	if(!prepareResources())
	{
		release();
		throw game_errors::load_cannon_sources;
	}

	f_distance = sqrt((xDest - xPos) * (xDest - xPos) + (yDest - yPos) * (yDest - yPos));
}

Cannon::~Cannon(void)
{
	release();
}

void Cannon::release()
{
	hge->Texture_Free(h_tCannon);
	delete p_cannonAnimated;
}

bool Cannon::prepareResources()
{
	try
	{
		p_cannonAnimated = new hgeAnimation(h_tCannon, 4, 10.0f, 0.0f, 0.0f, 16.0f, 16.0f);
	}
	catch(...)
	{
		return false;
	}
	return true;
}


void Cannon::render()
{
	if(b_isFlying)
	{
		fly();
	}
}

void Cannon::frame()
{
	f_delta += dt * CANNON_SPEED;

	if(f_delta > f_distance)
	{
		b_isFlying = false;
		f_delta = 0;
	}

	float lymbda = f_delta/(f_distance - f_delta);

	f_xPos = (f_xStart + lymbda * f_xDest)/(1 + lymbda);
	f_yPos = (f_yStart + lymbda * f_yDest)/(1 + lymbda);
}

void Cannon::fly()
{
	if(p_cannonAnimated == 0)
	{
		throw game_errors::null_pointer;
	}

	if(!p_cannonAnimated->IsPlaying())
	{
		p_cannonAnimated->Play();
	}
	else
	{
		p_cannonAnimated->Update(dt);
	}

	p_cannonAnimated->SetHotSpot(8.0f, 8.0f);
	p_cannonAnimated->Render(f_xPos, f_yPos);
}

