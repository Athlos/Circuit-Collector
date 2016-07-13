#include "alienexplosionparticle.h"
#include "sprite.h"
#include "entity.h"

#include <vector>
#include <cmath>
#include <ctime>

AlienExplosionParticle::AlienExplosionParticle() 
{
	Particle::Particle();
	m_type = ALIENEXPLOSION;
}

AlienExplosionParticle::~AlienExplosionParticle() 
{

}

bool AlienExplosionParticle::Initialise(Sprite* sprite) 
{
	return (Particle::Initialise(sprite));
}

void AlienExplosionParticle::Process(float deltaTime)
{
	Particle::Process(deltaTime);
}
void AlienExplosionParticle::Draw(BackBuffer& backbuffer)
{
	Particle::Draw(backbuffer);
}

void AlienExplosionParticle::SpawnParticle(int x, int y, BackBuffer* backbuffer, ParticleType t) 
{
	if (t == BRICKPARTICLE) 
	{
		m_pSprite = backbuffer->CreateSprite("assets\\brickfragment.png");
	}
	else 
	{
		m_pSprite = backbuffer->CreateSprite("assets\\alienfragment.png");
	}
	
	m_x = x;
	m_y = y;
	m_age = 2.0f;
	m_velocityX = (rand() % 150) - 75;
	m_velocityY = (rand() % 150) - 75;

	//OBJECTS MUST DE-ACCELERATE, MAKE ACCELERATION OPOSITE TYPE TO VELOCITY
	if (m_velocityX > 0) 
	{
		m_accelerationX = (rand() % 50) - 50;
	}
	else
	{
		m_accelerationX = rand() % 50;
	}

	if (m_velocityY > 0)
	{
		m_accelerationY = (rand() % 50) - 50;
	}
	else 
	{
		m_accelerationY = rand() % 50;
	}
}