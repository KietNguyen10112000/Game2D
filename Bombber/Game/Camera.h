#pragma once

#include <Component/SpriteSheet.h>

class Game;

class Camera
{
public:
	enum SPEED_ACCELERATION_TYPE
	{
		LINEAR			= 0,
		EXPONENT		= 1
	};

	SPEED_ACCELERATION_TYPE m_speedAcceType = EXPONENT;

	float m_speedLowerBound = 3;
	float m_speedUpperBound = 8;

	//magic numbers :D
	Vec2 m_camLimitTopLeft = { -1280 * 1.5,-720 / 4 };
	Vec2 m_camLimitBottomRight = { 1280 * 1.5, 720 + 720 / 4 };

public:
	Rect2D* m_camRect = 0;

	int m_preMousePosX = 0;
	int m_preMousePosY = 0;

	int m_windowPosX = 0;
	int m_windowPosY = 0;

	int m_windowWidth = 0;
	int m_windowHeight = 0;

	bool m_lockMouse = 0;

	Sprite m_sprite;

public:
	enum FOLLOW_TYPE
	{
		CENTER			= 0,
		LOCK_POSITION	= 1,
	};
	FOLLOW_TYPE m_followType = CENTER;

	Vec2 m_followOffset;
	Vec2* m_targetPosition = 0;


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

protected:
	void HandleCameraLinear(Game* game);
	void HandleCameraExp(Game* game);

	void HandleFollow(Game* game);

public:
	inline auto& SpeedAccelerationType() { return m_speedAcceType; };

	inline auto& FollowType() { return m_followType; };
	//just Follow(NULL) to un-follow
	void Follow(Vec2* position);

	inline auto* GetFollowPosition() { return m_targetPosition; };

};