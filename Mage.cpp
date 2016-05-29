#include "Mage.h"
#include "GameObjects.h"

Mage::Mage(float xPos, float yPos, bool toEast) : f_xPos(xPos), f_yPos(yPos), 
	b_standToTheEast(toEast), p_mageStopped(0), p_mageAnimated(0), b_isUsingMagic(false), f_pauseUsingMagic(0.0f), f_timeToShot(0.0f) 
{
	mage_east = hge->Texture_Load("Resources\\mage_east.png");
	f_pauseInterval = hge->Random_Float(2.0, 5.0);

	if(!prepareResources())
	{
		release();
		throw game_errors::load_mage_sources;
	}
}

Mage::~Mage(void)
{
	release();
}

void Mage::release()
{
	hge->Texture_Free(mage_east);

	delete p_mageAnimated;
	delete p_mageStopped;
}

bool Mage::prepareResources()
{
	try
	{
		p_mageStopped = new hgeSprite(mage_east, 0.0f, 0.0f, 72.0f, 72.0f);
		p_mageAnimated = new hgeAnimation(mage_east, 4, 5.0f, 0.0f, 360.0f, 72.0f, 72.0f); 
	}
	catch(...)
	{
		return false;
	}
	return true;
}

void Mage::render()
{
	if(b_isUsingMagic)
	{
		useMagic();
	}
	else
	{
		stand();
	}
}

void Mage::useMagic()
{

	if(p_mageAnimated == 0)
	{
		throw game_errors::null_pointer;
	}

	if(!b_standToTheEast)
	{
		p_mageAnimated->SetFlip(true, false);
	}
	
	if(!p_mageAnimated->IsPlaying())
	{
		p_mageAnimated->Play();
	}
	else
	{
		p_mageAnimated->Update(dt);
	}

	p_mageAnimated->SetHotSpot(36.0f, 36.0f);
	p_mageAnimated->Render(f_xPos, f_yPos);

	f_timeToShot += dt;
	
	if(f_timeToShot > 2.0f)
	{
		b_isUsingMagic = false;
		f_timeToShot = 0.0f;
		prepareCannonFlight();
		f_pauseInterval = hge->Random_Float(0.0, 5.0);
	}
}

void Mage::stand()
{
	if(p_mageStopped == 0)
	{
		throw game_errors::null_pointer;
	}

	if(!b_standToTheEast)
	{
		p_mageStopped->SetFlip(true, false);
	}
	
	p_mageStopped->SetHotSpot(36.0f, 36.0f);
	p_mageStopped->Render(f_xPos, f_yPos);

	f_pauseUsingMagic += dt;
	
	if(f_pauseUsingMagic > f_pauseInterval)
	{
		b_isUsingMagic = true;
		f_pauseUsingMagic = 0.0f;
	}
}

void Mage::prepareCannonFlight()
{
	Cannon* pCannon;

	std::pair<float, float> dest;

	if(gameObjects->hero()->isMoving())
	{
		auto path = dynamic_cast<Hero*>(gameObjects->hero().get())->getHeroPath();
		int index = hge->Random_Int(0, path.size() - 1);
		dest = path[index];
	}
	else if(gameObjects->hero()->isDying() || gameObjects->hero()->isFighting())
	{
		return;
	}
	else
	{
		dest = gameObjects->hero()->getCurrentPosition();
	}
	
	if(b_standToTheEast)
	{
		pCannon = new Cannon(f_xPos + CANNON_SHIFT, f_yPos, dest.first, dest.second, true);
	}
	else
	{
		pCannon = new Cannon(f_xPos - CANNON_SHIFT, f_yPos, dest.first, dest.second, true);
	}
	gameObjects->cannons().push_back(static_cast<std::unique_ptr<Cannon>>(pCannon));
}