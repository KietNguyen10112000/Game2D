#pragma once

#include <Engine/Engine.h>

#include <Math/Math.h>

#include <Math/Collision.h>

#include "World.h"

#include <Engine/Renderer2D.h>

#include "Random.h"

class Game : public Engine
{
public:
	Rect2D m_camRect;

	World m_world;

public:
	Game();
	~Game();

public:
	void Update() override;
	void Render();

public:
	//render to screen with
	//position	: world position
	//angle	: rotation angle
	template <typename T>
	inline void Render(T* target, const Vec2& position, const float angle, const Vec2& scale, int flip);

public:
	inline auto& World() { return m_world; };
	inline auto& CameraRect() { return m_camRect; };

};

template <typename T>
inline void Game::Render(T* target, const Vec2& position, const float angle, const Vec2& scale, int flip)
{
	auto texture = target->GetRenderTexture();
	auto rect = target->GetRenderRect();

	Rect2D screenRect = *rect;

	auto w = screenRect.Width() * scale.x;
	auto h = screenRect.Height() * scale.y;
	auto pivot = position - m_camRect.m_point;

	screenRect = Rect2D(pivot.x, pivot.y, w, h);

	if (angle != 0)
	{
		screenRect.Rotate(angle);
	}

	if (screenRect.CanOverlap(m_camRect))
	{
		screenRect = Rect2D(pivot.x, pivot.y, w, h);
		Renderer()->Draw(texture, 1, angle, screenRect.Center(), flip, screenRect, *rect);
	}
}
