#include "IPlayer.h"

#include "../Game.h"


IPlayer::IPlayer(Game* game)
{
	new(&m_camCtrl) Camera(game);
}

void PlayerUpdater::Update(Game* game, IPlayer* player)
{
	auto& stateFlags = player->m_stateFlags;
	stateFlags = 0;

	player->RecvDataFromBox2d();

	auto input = game->Input();

	auto& keyCtrl = player->m_keyCtrl;

	player->m_camCtrl.Update(game);

	auto& camRect = game->m_camRect;

	Vec2 targetInWorldSpace = { 
		camRect.m_point.x + player->m_camCtrl.m_preMousePosX, 
		camRect.m_point.y + player->m_camCtrl.m_preMousePosY 
	};

	player->m_lookupDir = (targetInWorldSpace - player->m_position).Normalize();

	if (input->GetKey(keyCtrl.GetKeyCodeOf(PlayerController::CONTROLER_KEY::UP)))
	{
		player->OnUp();
		stateFlags |= IPlayer::STATE_FLAG::UP;
	}

	if (input->GetKey(keyCtrl.GetKeyCodeOf(PlayerController::CONTROLER_KEY::DOWN)))
	{
		player->OnDown();
		stateFlags |= IPlayer::STATE_FLAG::UP;
	}

	if (input->GetKey(keyCtrl.GetKeyCodeOf(PlayerController::CONTROLER_KEY::LEFT)))
	{
		player->OnLeft();
		stateFlags |= IPlayer::STATE_FLAG::LEFT;
	}

	if (input->GetKey(keyCtrl.GetKeyCodeOf(PlayerController::CONTROLER_KEY::RIGHT)))
	{
		player->OnRight();
		stateFlags |= IPlayer::STATE_FLAG::RIGHT;
	}

	if (input->GetPressKey(keyCtrl.GetKeyCodeOf(PlayerController::CONTROLER_KEY::LOCK_CAMERA)))
	{
		player->OnLockCamera();
		stateFlags |= IPlayer::STATE_FLAG::LOCK_CAMERA;
	}

	if (input->GetPressKey(keyCtrl.GetKeyCodeOf(PlayerController::CONTROLER_KEY::RELEASE_MOUSE_HOLD)))
	{
		player->OnReleaseMouseHold();
		stateFlags |= IPlayer::STATE_FLAG::RELEASE_MOUSE_HOLD;
	}

	if (input->GetPressKey(keyCtrl.GetKeyCodeOf(PlayerController::CONTROLER_KEY::SWITCH_WEAPON)))
	{
		player->OnSwitchWeapon();
		stateFlags |= IPlayer::STATE_FLAG::SWITCH_WEAPON;
	}

	if (input->GetPressKey(keyCtrl.GetKeyCodeOf(PlayerController::CONTROLER_KEY::SWITCH_SKILL)))
	{
		player->OnSwitchSkill();
		stateFlags |= IPlayer::STATE_FLAG::SWITCH_SKILL;
	}

	if (input->GetMouseClick(LEFT))
	{
		player->OnFire();
		stateFlags |= IPlayer::STATE_FLAG::FIRE;
	}

	//player->_Update(game);
	player->Update(game);
}
