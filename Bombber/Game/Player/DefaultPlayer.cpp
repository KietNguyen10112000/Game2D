#include "DefaultPlayer.h"

DefaultPlayer::DefaultPlayer(Game* game) : IPlayer(game)
{
	//idle animation
	m_animator.AddAnimation({
		L"../../../../Bombber/Assets/char_red.png",
		{
			{0,512,128,128},
			{128,512,128,128},
			{256,512,128,128},
			{384,512,128,128},
			{512,512,128,128},
			{640,512,128,128},
			{768,512,128,128},
			{896,512,128,128},
			{1024 - 5,512,128,128},
			{1152 - 5,512,128,128},
			{1280 - 5,512,128,128},
			{1408 - 5,512,128,128},
		}
		});
	m_animator.SetAnimation(0);
	m_animator.SetDuration(1.5);

	//run animation
	m_animator.AddAnimation({
		L"../../../../Bombber/Assets/char_red.png",
		{
			{128,0,128,128},
			{256,0,128,128},
			{384,0,128,128},
			{512,0,128,128},
			{640,0,128,128},
			{768,0,128,128},
			{896,0,128,128},
			{1024,0,128,128},
		}
		});
	m_animator.SetAnimation(1);
	m_animator.SetDuration(1);

	m_spriteDims = { m_animator.GetRenderRect()->Width(), m_animator.GetRenderRect()->Height() };
	m_originLookupDir = { 1,0 };

	InitBox2dBody();

	SetTransform({ 512, 0 }, PI / 2);
}

void DefaultPlayer::InitBox2dBody()
{
	auto game = GameGlobal::game;
	auto& world = game->World();
	auto& b2world = world.B2World();

	b2BodyDef bodyDef;
	bodyDef.userData.pointer = (uintptr_t)this;
	bodyDef.type = b2BodyType::b2_dynamicBody;
	bodyDef.position.Set(0, 0);

	m_body = b2world.CreateBody(&bodyDef);

	Vec2 boundingBox = { m_animator.GetRenderRect()->Width() / 2.0f, m_animator.GetRenderRect()->Height() * 0.7f };
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(
		boundingBox.x / (2.0f) * World::PIXEL_PER_METERS, boundingBox.y / (2.0f) * World::PIXEL_PER_METERS,
		{0, boundingBox.y * 0.1f * World::PIXEL_PER_METERS }, 0
	);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 10.0f;
	fixtureDef.friction = 1.f;
	fixtureDef.restitution = 0.1f;

	m_body->CreateFixture(&fixtureDef);

	b2CircleShape circleLeg; //:))
	circleLeg.m_radius = (boundingBox.x / 2.0f) * World::PIXEL_PER_METERS;
	circleLeg.m_p = { 0,0 };

	fixtureDef.shape = &circleLeg;
	m_body->CreateFixture(&fixtureDef);
}

void DefaultPlayer::Update(Game* game)
{
	m_animator.Play(game);

	if (DotProduct(m_originLookupDir, m_lookupDir) < 0)
	{
		m_flip = 1;
	}
	else
	{
		m_flip = 0;
	}

	if (m_stateFlags == 0)
	{
		m_animator.SetAnimation(0);
		
		if (m_angle != 0)
		{
			m_angle = m_angle * 0.9f;
			m_body->SetTransform(*(b2Vec2*)&(m_position * World::PIXEL_PER_METERS), m_angle);
			if (std::abs(m_angle) < 0.001f) m_angle = 0;
		}
	}
}

void DefaultPlayer::Render(Game* game)
{
	m_camCtrl.Render(game);

	auto texture = m_animator.GetRenderTexture();
	auto rect = m_animator.GetRenderRect();

	game->Render(&m_animator, m_position - m_spriteDims / 2.0f, m_angle, { 1,1 }, m_flip);

}

void DefaultPlayer::OnUp()
{
	m_animator.SetAnimation(1);
	
}

void DefaultPlayer::OnDown()
{
	m_animator.SetAnimation(1);
}

void DefaultPlayer::OnLeft()
{
	m_animator.SetAnimation(1);
	m_body->SetLinearVelocity({ -m_movSpeed * World::PIXEL_PER_METERS,0.f });
	m_body->SetTransform(m_body->GetPosition(), 0);
}

void DefaultPlayer::OnRight()
{
	m_animator.SetAnimation(1);
	
	m_body->SetLinearVelocity({ m_movSpeed * World::PIXEL_PER_METERS,0.f });
	//m_body->SetAngularVelocity(0);
	SetTransform(m_position, 0);
}

void DefaultPlayer::OnLockCamera()
{
	if (m_camCtrl.GetFollowPosition() == 0)
	{
		m_camCtrl.FollowType() = Camera::FOLLOW_TYPE::LOCK_POSITION;
		m_camCtrl.Follow(&m_position);
	}
	else
	{
		m_camCtrl.Follow(0);
	}
}

void DefaultPlayer::OnSwitchWeapon()
{
}

void DefaultPlayer::OnSwitchSkill()
{
}

void DefaultPlayer::OnReleaseMouseHold()
{
}

void DefaultPlayer::OnFire()
{
}
