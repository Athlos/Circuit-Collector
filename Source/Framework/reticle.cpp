#include "reticle.h"
#include "sprite.h"
#include "backbuffer.h"

#include <SDL.h>


Reticle::Reticle()
{
	Entity::Entity();
}
Reticle::~Reticle() 
{

}

bool Reticle::IsFiring()
{
	return m_damageAnim->IsAnimating();
}

void Reticle::SetFiring()
{
	if (m_damageAnim->IsAnimating())
	{
		return;
	}
	m_damageAnim->StartAnimating();
}

void Reticle::Draw(BackBuffer& backBuffer, bool hit)
{
	if (m_x > 760 || m_x < 5 || m_y > 560 || m_y < 5)
	{
		//backBuffer.SetMouse(m_x + 16, m_y + 16);
	}
	
	assert(m_pSprite);

	if (hit && m_damageAnim->IsAnimating())
	{
		m_damageAnim->Draw(backBuffer);
	}
	m_pSprite->Draw(backBuffer);
}

void Reticle::SetAnimation(AnimatedSprite* anim)
{
	m_damageAnim = anim;
	m_damageAnim->SetFrameWidth(64);
	m_damageAnim->AddFrame(64);
	m_damageAnim->AddFrame(128);
	m_damageAnim->AddFrame(192);
	m_damageAnim->AddFrame(256);
	m_damageAnim->AddFrame(320);
	m_damageAnim->SetFrameSpeed(0.02f);
	m_damageAnim->StopAnimating();
}

void Reticle::SetDamage(int amount) 
{
	m_damage = amount;
}

int Reticle::GetDamage() 
{
	return m_damage;
}

void Reticle::Process(float deltaTime) 
{
	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));
	m_damageAnim->SetX(static_cast<int>(m_x));
	m_damageAnim->SetY(static_cast<int>(m_y));

	int x, y;
	m_damageAnim->Process(deltaTime);
	SDL_GetRelativeMouseState(&x, &y);
	
	m_x += x;
	m_y += y;

	if (m_x >= 800 - m_pSprite->GetWidth())
	{
		m_x = (800 - m_pSprite->GetWidth());
		m_velocityX = 0;
	}

	if (m_x <= 0)
	{
		m_x = 0;
		m_velocityX = 0;
	}

	if (m_y >= 600 - m_pSprite->GetHeight()) 
	{
		m_y = (600 - m_pSprite->GetHeight());
	}

	if (m_y <= 0) 
	{
		m_y = 0;
	}
}

void Reticle::Draw(BackBuffer& backBuffer)
{
	Entity::Draw(backBuffer);
}