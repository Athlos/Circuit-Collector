#include "enemy.h";

#include "entity.h";
#include "sprite.h"


Enemy::Enemy()
{
}

Enemy::~Enemy() 
{
	
}

void Enemy::SetReward(int amount) 
{
	m_reward = amount;
}

void Enemy::SetData(int health, int speed, int damage) 
{
	m_health = health;
	m_velocityY = speed;
	m_damage = damage;
}

bool Enemy::IsClickedOn(int x, int y) 
{
	return (Entity::IsClickedOn(x, y));
}

void Enemy::TakeDamage(int amount) 
{
	m_health -= amount;
	if (m_health <= 0) 
	{
		m_health = 0;
		m_dead = true;
	}
}

void Enemy::Process(float deltaTime) 
{
	Entity::Process(deltaTime);

	if (m_y >= 500 - m_pSprite->GetHeight())
	{
		m_y = 500 - m_pSprite->GetHeight();
		m_velocityY = 0;
	}}

void Enemy::SetPosition(float x, float y) 
{
	m_x = x;
	m_y = y;
	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));
}

int Enemy::GetDamage()
{
	return m_damage;
}

int Enemy::GetReward()
{
	return m_reward;
}

int Enemy::GetHealth()
{
	return m_health;
}