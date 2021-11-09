#pragma once

#include <Component/SpriteSheet.h>

class Game;

class Camera
{
public:
	int m_preMousePosX = 0;
	int m_preMousePosY = 0;

	int m_windowPosX = 0;
	int m_windowPosY = 0;

	int m_windowWidth = 0;
	int m_windowHeight = 0;

	bool m_lockMouse = 0;

	int m_currentX = 0;
	int m_currentY = 0;

	Sprite m_sprite;

public:
	inline Camera() {};
	Camera(Game* game);

public:
	void Update(Game* game);
	void Render(Game* game);

public:
	void UpdateWhileLock(Game* game);
	void ClampMouse(int* outX, int* outY, int* inX, int* inY);
	void ClampCamera(Rect2D& camRect);

public:
	void HandleMouse(Game* game);
	void HandleCamera(Game* game);

};