#pragma once

#include "IPlayer.h"

#include <Component/AnimationSpriteSheet.h>

class DefaultPlayer : public IPlayer
{
public:
	Animator2DSingleTexture m_animator;
	Vec2 m_spriteDims;
	Vec2 m_originLookupDir;
	int m_flip = 0;

	float m_movSpeed = 250.0f;

public:
	inline DefaultPlayer() : IPlayer(0) {};
	DefaultPlayer(Game* game);

private:
	void InitBox2dBody();

public:
	void Update(Game* game) override;
	void Render(Game* game) override;

public:
	// Inherited via IPlayer
	virtual void OnUp() override;

	virtual void OnDown() override;

	virtual void OnLeft() override;

	virtual void OnRight() override;

	virtual void OnLockCamera() override;

	virtual void OnSwitchWeapon() override;

	virtual void OnSwitchSkill() override;

	virtual void OnReleaseMouseHold() override;

	virtual void OnFire() override;

};