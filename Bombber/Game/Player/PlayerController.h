#pragma once

class PlayerController
{
public:
	enum CONTROLER_KEY
	{
		UP,			//jump
		DOWN,
		LEFT,
		RIGHT,

		LOCK_CAMERA,

		SWITCH_WEAPON,

		SWITCH_SKILL,

		RELEASE_MOUSE_HOLD,

		COUNT
	};

public:
	uint8_t m_ctrlKeys[CONTROLER_KEY::COUNT] = {
		'W',
		'S',
		'A',
		'D',

		SPACE,

		'Q',

		'E',

		TAB
	};

public:
	inline uint8_t GetKeyCodeOf(CONTROLER_KEY key) { return m_ctrlKeys[key]; }

};