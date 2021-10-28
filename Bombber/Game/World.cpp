#include "World.h"

#include <Engine/Engine.h>
#include <Engine/Common.h>

#include <iostream>

World::World()
{
	m_world.SetGravity({ 0,10 });
}

World::~World()
{
}

void World::Update(Engine* engine)
{
	float timeStep = engine->FDeltaTime();
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	m_world.Step(timeStep, velocityIterations, positionIterations);
}

void World::Render(Engine* engine)
{

}
