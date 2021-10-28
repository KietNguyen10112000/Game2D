#pragma once

#include "Object.h"

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