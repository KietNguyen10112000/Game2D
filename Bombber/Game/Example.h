#pragma once

#include "Object.h"

#include <Engine/Polygon.h>

class RectObject : public Object2DNonAnim
{
public:
	float m_scaleX = 0;
	float m_scaleY = 0;

	Vec2 m_offset;

	//to display m_position
	Sprite m_pivot;

public:
	RectObject(float x, float y, float w, float h);
	~RectObject();

public:
	// Inherited via Object2DNonAnim
	virtual void Update(Game* game) override;
	virtual void Render(Game* game) override;

};

class PolygonObject : public Object2D
{
public:
	class Polygon* m_polygon = 0;

public:
	PolygonObject();
	~PolygonObject();

public:
	// Inherited via Object2D
	virtual void Update(Game* game) override;
	virtual void Render(Game* game) override;
};


class CircleObject : public Object2D
{
public:
	class Polygon* m_polygon = 0;

public:
	CircleObject(float x, float y, float r);
	~CircleObject();

public:
	// Inherited via Object2DNonAnim
	virtual void Update(Game* game) override;
	virtual void Render(Game* game) override;

};