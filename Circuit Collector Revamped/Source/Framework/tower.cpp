#include "tower.h"
#include "projectile.h"

Tower::Tower()
{
	m_secondsPerShot = 0.75;
	m_secondsElapsedSinceShot = 0;
}

Tower::~Tower()
{
	Entity::~Entity();
	delete m_projectile;
	m_projectile = 0;
}

void Tower::InitialiseProjectile(Projectile* projectile)
{
	m_projectile = projectile;
}

void Tower::Process(float deltaTime)
{
	Entity::Process(deltaTime);

	m_secondsElapsedSinceShot += deltaTime;
}

void Tower::Draw(BackBuffer& backBuffer)
{
	Entity::Draw(backBuffer);
}

bool Tower::IsReadyToShoot()
{
	return (m_secondsElapsedSinceShot >= m_secondsPerShot);
}

Projectile* Tower::GetProjectile()
{
	return m_projectile;
}

void Tower::ResetCooldown()
{
	m_secondsElapsedSinceShot = 0;
}