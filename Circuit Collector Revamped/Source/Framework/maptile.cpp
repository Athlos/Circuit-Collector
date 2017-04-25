#include "maptile.h"


MapTile::MapTile()
{
}


MapTile::~MapTile()
{
}

void MapTile::Process(float deltaTime)
{
	Entity::Process(deltaTime);
}

void MapTile::Draw(BackBuffer& backBuffer)
{
	Entity::Draw(backBuffer);
}