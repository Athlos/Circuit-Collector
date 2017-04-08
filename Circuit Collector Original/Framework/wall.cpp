
#include "wall.h"
#include "backbuffer.h"
#include "animatedsprite.h"
#include <cassert>

Wall::Wall()
{
	Entity::Entity();
}

Wall::~Wall()
{

}

void Wall::SetMaxHealth(int health)
{
	m_maxHealth = health;
}

int Wall::GetMaxHealth()
{
	return m_maxHealth;
}

int Wall::GetCurrentHealth()
{
	return m_currentHealth;
}

void Wall::SetCurrentHealth(int health)
{
	m_currentHealth = health;
}

void Wall::TakeDamage(int amount)
{
	m_currentHealth -= amount;
	if (m_currentHealth <= 0)
	{
		m_currentHealth = 0;
		m_dead = true;
	}
}

bool Wall::IsCollidingWith(Entity& e)
{
	int x1 = e.GetPositionX() + 8;
	int y1 = e.GetPositionY() + 8;
	int radius1 = 16;
	int radius2 = 16;

	//compare the distance to combined radii
	if (sqrt((x1 - m_x) * (x1 - m_x) + (y1 - m_y) * (y1 - m_y)) < 64)
	{
		return true;
	}

	return (false);
}