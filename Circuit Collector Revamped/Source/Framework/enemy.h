#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

enum EnemyType
{
	TANK,
	MAX_TYPE
};

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();

	void Enemy::SetPosition(float x, float y);
	void Process(float deltaTime);
	void SetData(int health, int speed, int damage);
	bool IsClickedOn(int x, int y);
	void TakeDamage(int amount);
	int GetDamage();
	int GetReward();
	int GetHealth();

	void SetReward(int amount);

protected:
private:

public:
protected:
	int m_health;
	int m_damage;
	int m_reward;
private:

};
#endif //__ENEMY_H__

