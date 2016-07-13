
#include "mine.h";

#include "entity.h";
#include "sprite.h"


Mine::Mine()
{
	Entity::Entity();
}

Mine::~Mine() 
{

}

int Mine::GetDamage() 
{
	return m_damage;
}

void Mine::SetDamage(int damage) 
{
	m_damage = damage;
}

void Mine::Process(float deltaTime)
{
	Entity::Process(deltaTime);

}

void Mine::SetPosition(float x, float y)
{
	m_x = x;
	m_y = y;
	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));
}

bool Mine::IsOutOfBounds() 
{
	if (m_y < 0) 
	{
		return true;
	}

	return false;
}