#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "entity.h"

enum ProjectileType
{
	MISSILE,
	BULLET,
	BEAM,
	ARC
};

class Projectile : public Entity
{
public:
	Projectile();
	~Projectile();
};

#endif //__PROJECTILE_H__