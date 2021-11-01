#pragma once

#include <box2d/box2d.h>

#include <Component/SpriteSheet.h>
#include <Component/AnimationSpriteSheet.h>

#include "Global.h"

#include "Game.h"
#include <Game/World.h>

class Game;

class Object2D
{
public:
	b2Body* m_body = 0;
	//b2PolygonShape m_shape;

	Vec2 m_position;
	float m_angle = 0;

	enum PHYSIC_TYPE
	{
		STATIC = b2BodyType::b2_staticBody,
		DYNAMIC = b2BodyType::b2_dynamicBody,
		KINEMATIC = b2BodyType::b2_kinematicBody
	};

public:
	inline virtual ~Object2D() {};

public:
	inline void _Update(Game* game)
	{
		m_position = *(Vec2*)&m_body->GetPosition();
		m_angle = m_body->GetAngle();

		m_position = m_position / World::PIXEL_PER_METERS;

		Update(game);
	}

public:
	virtual void Update(Game* game) = 0;
	virtual void Render(Game* game) = 0;

public:
	inline virtual void SetTransform(const Vec2& position, float angle)
	{
		m_position = position * World::PIXEL_PER_METERS;
		m_angle = angle;
		m_body->SetTransform(*(b2Vec2*)&m_position, m_angle);
		m_body->SetAwake(true);
	}

	inline virtual void SetPhysicType(PHYSIC_TYPE type)
	{
		m_body->SetType((b2BodyType)type);
	}

};

class Object2DNonAnim : public Object2D
{
public:
	Sprite m_sprite;

public:
	inline virtual ~Object2DNonAnim() {};

public:
	inline virtual void Render(Game* game) override
	{
		game->Render(&m_sprite, m_position, m_angle, { 1,1 }, 0);
	};

public:
	inline auto& GetSprite() { return m_sprite; };

};

class Object2DAnim : public Object2D
{
public:
	Animator2DSingleTexture m_animator;

public:
	inline virtual ~Object2DAnim() {};

public:
	inline virtual void Render(Game* game) override
	{
		game->Render(&m_animator, m_position, m_angle, { 1,1 }, 0);
	};

public:
	inline auto& GetAnimator() { return m_animator; };

};