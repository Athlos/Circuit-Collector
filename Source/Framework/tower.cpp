#include "entity.h"
#include "tower.h"
#include "backbuffer.h"
#include "animatedsprite.h"
#include <cassert>


Tower::Tower()
{
	Entity::Entity();
	m_timePassed = 0.0f;
	m_level = 0;
}

Tower::~Tower()
{

}

void Tower::SetSpawning()
{
	if (m_spawnAnim->IsAnimating())
	{
		return;
	}
	m_spawnAnim->StartAnimating();
}

void
Tower::Draw(BackBuffer& backBuffer, bool spawn)
{
	assert(m_pSprite);

	if (spawn && m_spawnAnim->IsAnimating()) 
	{
		m_spawnAnim->Draw(backBuffer);
	}
	else
	{
		m_pSprite->Draw(backBuffer);
	}
}

void Tower::SetAnimation(AnimatedSprite* anim)
{
	m_spawnAnim = anim;
	m_spawnAnim->SetFrameWidth(64);
	m_spawnAnim->AddFrame(0);
	m_spawnAnim->AddFrame(64);
	m_spawnAnim->AddFrame(128);
	m_spawnAnim->AddFrame(192);
	m_spawnAnim->AddFrame(256);
	m_spawnAnim->SetFrameSpeed(0.1f);
	m_spawnAnim->StopAnimating();
	m_spawnAnim->SetX(static_cast<int>(m_x));
	m_spawnAnim->SetY(static_cast<int>(m_y));
}

int Tower::GetLevel()
{
	return m_level;
}

void Tower::SetLevel(int level)
{
	m_level = level;
}

void Tower::Process(float deltaTime)
{
	Entity::Process(deltaTime);
	m_spawnAnim->Process(deltaTime);
	m_timePassed += deltaTime;
	m_spawnAnim->SetX(static_cast<int>(m_x));
	m_spawnAnim->SetY(static_cast<int>(m_y));
}

void Tower::SetDamage(int amount)
{
	m_damage = amount;
}

int Tower::GetDamage() 
{
	return m_damage;
}

bool Tower::ReadyToFire()
{
	if (m_timePassed > m_firingSpeed) 
	{
		m_timePassed = 0;
		return true;
	}
	return false;
}

void Tower::SetType(TowerType t)
{
	m_type = t;
}

TowerType Tower::GetType()
{
	return m_type;
}

void Tower::SetFiringSpeed(float speed)
{
	m_firingSpeed = speed;
}

float Tower::GetFiringSpeed()
{
	return m_firingSpeed;
}
