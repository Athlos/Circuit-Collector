
#include "bullet.h";
#include "sprite.h"
#include "entity.h";


Bullet::Bullet()
{
	Entity::Entity();
}

Bullet::~Bullet()
{
}

void Bullet::SetParticleType(ParticleType t) 
{
	m_particles = t;
}

ParticleType Bullet::GetParticleType() 
{
	return m_particles;
}

void Bullet::Process(float deltaTime)
{
	Entity::Process(deltaTime);
	if (IsOutOfBounds()) 
	{
		m_dead = true;
	}
}

void Bullet::SetDamage(int amount)
{
	m_damage = amount;
}

int Bullet::GetDamage() 
{
	return m_damage;
}

bool Bullet::IsOutOfBounds() 
{
	if (m_y < 0) 
	{
		return true;
	}
	return false;
}