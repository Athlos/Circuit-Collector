#ifndef __MINE_H__
#define __MINE_H__

#include "entity.h"

class Mine : public Entity
{
public:
	Mine();
	~Mine();
	void Process(float deltaTime);
	void SetPosition(float x, float y);
	bool IsOutOfBounds();
	int GetDamage();
	void SetDamage(int damage);
protected:
private:

public:
protected:
	int m_damage;
private:

};
#endif //__MINE_H__

