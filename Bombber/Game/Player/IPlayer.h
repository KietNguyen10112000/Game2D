#pragma once

#include "../Object.h"

#include "../Camera.h"
#include "PlayerController.h"

class IPlayer : public Object2D
{
public:
	//replace with child classname
	std::string m_classname;

public:
	Camera m_camCtrl;
	PlayerController m_keyCtrl;

public:
	//correct to what action call
	enum STATE_FLAG
	{
		UP						= 1,			//jump
		DOWN					= 2,
		LEFT					= 4,
		RIGHT					= 8,

		LOCK_CAMERA				= 16,

		SWITCH_WEAPON			= 32,

		SWITCH_SKILL			= 64,

		RELEASE_MOUSE_HOLD		= 128,

		FIRE					= 256,

		COUNT
	};

	//info can be use by child class

	uint64_t m_stateFlags = 0;

	//dir from center of player to mouse cursor
	Vec2 m_lookupDir;
	float m_forceMagnitude = 0;

public:
	inline IPlayer() {};
	IPlayer(Game* game);
	inline virtual ~IPlayer() {};

public:
	inline bool IsContain(STATE_FLAG flag) { return (m_stateFlags & flag) != 0; };

public:
	virtual void OnUp() = 0;
	virtual void OnDown() = 0;
	virtual void OnLeft() = 0;
	virtual void OnRight() = 0;

	virtual void OnLockCamera() = 0;

	virtual void OnSwitchWeapon() = 0;
	virtual void OnSwitchSkill() = 0;

	virtual void OnReleaseMouseHold() = 0;

	virtual void OnFire() = 0;

};


class PlayerUpdater
{
public:
	void Update(Game* game, IPlayer* player);
};