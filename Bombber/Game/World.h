#pragma once

#include <box2d/box2d.h>

class Engine;

class World
{
public:
	constexpr static float PIXEL_PER_METERS = 0.01f;
	constexpr static float METER_PER_PIXELS = 1 / PIXEL_PER_METERS;

public:

	b2Vec2 m_gravity = { 0.0f,9.8f };
	b2World m_world = b2World(m_gravity);

public:
	World();
	~World();

public:
	void Update(Engine* engine);

	void Render(Engine* engine);

	inline auto& B2World() { return m_world; };

};