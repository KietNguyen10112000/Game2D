#include "Example.h"

#include "Game.h"

RectObject::RectObject(float x, float y, float w, float h)
{
	auto game = GameGlobal::game;
	auto& world = game->World();
	auto& b2world = world.B2World();

	b2BodyDef bodyDef;
	bodyDef.userData.pointer = (uintptr_t)this;
	bodyDef.type = b2BodyType::b2_dynamicBody;
	bodyDef.position.Set(x * World::PIXEL_PER_METERS, y * World::PIXEL_PER_METERS);

	m_body = b2world.CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	//SetAsBox take half w,h
	dynamicBox.SetAsBox(w / (2.0f) * World::PIXEL_PER_METERS, h / (2.0f) * World::PIXEL_PER_METERS);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1000.0f;
	fixtureDef.friction = 0.3f;

	m_body->CreateFixture(&fixtureDef);

	new(&m_sprite) Sprite(L"../../../../Bombber/Assets/foo.png", { 0,0,10,10 });
	//we get rect 10x10 pixels from image, so need (w/10, h/10) to up scale to match w,h in screen
	m_scaleX = w / 10.0f;
	m_scaleY = h / 10.0f;

	SetTransform({ x, y }, 0);

	m_offset = { w / 2.0f, h / 2.0f };

	new(&m_pivot) Sprite(L"../../../../Bombber/Assets/green.png", { 0,0,5,5 });
}

RectObject::~RectObject()
{
	auto game = GameGlobal::game;
	auto& world = game->World();
	auto& b2world = world.B2World();

	b2world.DestroyBody(m_body);

	m_body = 0;
}

void RectObject::Update(Game* game)
{
	//logic process
	if (game->Input()->GetPressKey(SPACE))
	{
		SetTransform({ 512, 20 }, 0);
	}

	if (game->Input()->GetPressKey(RIGHT_ARROW))
	{
		m_body->ApplyForce({ 0,0.3f }, *(b2Vec2*)&(m_position / World::PIXEL_PER_METERS), true);
	}
}

void RectObject::Render(Game* game)
{
	game->Render(&m_sprite, m_position - m_offset, m_angle, { m_scaleX,m_scaleY }, 0);

	game->Render(&m_pivot, m_position, m_angle, { 1,1 }, 0);
}
