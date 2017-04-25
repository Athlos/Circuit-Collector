#include "projectilespawner.h"
#include "projectile.h"
#include "tower.h"
#include "sprite.h"

ProjectileSpawner::ProjectileSpawner()
{
}


ProjectileSpawner::~ProjectileSpawner()
{
	std::vector<Projectile*>::iterator projIter = m_projectiles.begin();

	while (projIter != m_projectiles.end())
	{
		if (*projIter != 0)
		{
			delete *projIter;
			projIter = m_projectiles.erase(projIter);
		}
		else
		{
			++projIter;
		}
	}
}

void ProjectileSpawner::Process(float deltaTime)
{
	std::vector<Projectile*>::iterator projIter = m_projectiles.begin();

	while (projIter != m_projectiles.end())
	{
		Projectile* current = *projIter;
		if (current->IsDead() || current->GetPositionY() < -32)
		{
			projIter = m_projectiles.erase(projIter);
			delete current;
		}
		else
		{
			current->Process(deltaTime);
			projIter++;
		}
	}
}

void ProjectileSpawner::Draw(BackBuffer& backBuffer)
{
	for each (Projectile* projectile in m_projectiles)
	{
		projectile->Draw(backBuffer);
	}
}

void ProjectileSpawner::SpawnProjectile(Tower* tower)
{
	Projectile* firedProjectile = new Projectile();

	//TODO: Figure out the proper way to do this

	Sprite* projSprite = new Sprite();

	Texture* t = tower->GetProjectile()->GetSprite()->GetTexture();

	projSprite->Initialise(*t);

	firedProjectile->Initialise(projSprite);

	firedProjectile->SetPosition(tower->GetPositionX(), tower->GetPositionY());

	firedProjectile->SetVerticalVelocity(-200);

	m_projectiles.push_back(firedProjectile);
}

std::vector<Projectile*> ProjectileSpawner::GetProjectiles()
{
	return m_projectiles;
}