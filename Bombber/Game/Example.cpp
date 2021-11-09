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
	fixtureDef.density = 10.0f;
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
		m_body->SetLinearVelocity({ 0.f,0.f });
		SetTransform({ 512, 20 }, 0);
	}

	//if (game->Input()->GetKey(RIGHT_ARROW))
	//{
	//	m_body->ApplyForce({ 50.0f,0.f }, *(b2Vec2*)&(m_position * World::PIXEL_PER_METERS), true);
	//	//m_body->SetLinearVelocity({ 50.0f * World::PIXEL_PER_METERS,0.f });
	//}

	//if (game->Input()->GetKey(LEFT_ARROW))
	//{
	//	m_body->ApplyForce({ -50.0f,0.f }, *(b2Vec2*)&(m_position * World::PIXEL_PER_METERS), true);
	//	//m_body->SetLinearVelocity({ -50.0f * World::PIXEL_PER_METERS,0.f });
	//}

	
}

void RectObject::Render(Game* game)
{
	game->Render(&m_sprite, m_position - m_offset, m_angle, { m_scaleX,m_scaleY }, 0);

	game->Render(&m_pivot, m_position, m_angle, { 1,1 }, 0);
}


PolygonObject::PolygonObject()
{
	const size_t totalArc = 3;
	const size_t numSamples = 8;
	m_polygon = new class Polygon(numSamples * totalArc);
	
	const float total = PI;
	const float dx = total / (float)(numSamples - 1);
	float x = 0;

	const float y = 600;

	const float A = 100;
	const float scaleX = 150;

	b2Vec2 vs[numSamples * totalArc];
	b2ChainShape chain;

	for (size_t j = 0; j < totalArc; j++)
	{
		for (size_t i = 0; i < numSamples; i++)
		{
			//y = f(x)
			auto _y = -std::sin(x);
			auto fx = A * _y + y;
			auto _x = scaleX * x;

			if (std::abs(_y) < 0.00001f)
			{
				//std::cout << i << '\n';
				m_polygon->PushVertex({ _x, y }, { 1,0.5,0,1 }, {});
				m_polygon->SetVertexPivot(m_polygon->GetVerticesCount() - 1);
			}
			else
			{
				m_polygon->PushVertex({ _x, fx }, { 1,0.5,0,1 }, {});
			}

			//if((i % 4) == 0)
			vs[i + j * numSamples].Set(_x * World::PIXEL_PER_METERS, fx * World::PIXEL_PER_METERS);

			x += dx;
		}
	}

	m_polygon->Update();

	chain.CreateLoop(vs, numSamples * totalArc);


	auto game = GameGlobal::game;
	auto& world = game->World();
	auto& b2world = world.B2World();

	b2BodyDef bodyDef;
	bodyDef.userData.pointer = (uintptr_t)this;
	bodyDef.type = b2BodyType::b2_staticBody;
	bodyDef.position.Set(0, 0);

	m_body = b2world.CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &chain;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.f;
	fixtureDef.restitution = 0.1f;

	m_body->CreateFixture(&fixtureDef);
}

PolygonObject::~PolygonObject()
{
	delete m_polygon;

	auto game = GameGlobal::game;
	auto& world = game->World();
	auto& b2world = world.B2World();

	b2world.DestroyBody(m_body);
}

void PolygonObject::Update(Game* game)
{
}

void PolygonObject::Render(Game* game)
{
	auto& camRect = game->CameraRect();
	Mat3x3 temp;
	temp.SetPosition(m_position - camRect.m_point);
	game->Renderer()->DrawPolygon(0, 1, 0, {}, 0, m_polygon, temp);
}


CircleObject::CircleObject(float x, float y, float r)
{
	m_position = { x,y };

	const size_t numSamples = 32;
	m_polygon = new class Polygon(numSamples + 3);

	m_polygon->PushVertex({ 0,0 }, {0,0.3,0.5,1}, {});
	m_polygon->SetVertexPivot(0);

	float phi = 0;
	float dphi = 2 * PI / numSamples;
	m_polygon->PushVertex({ -r, 0 },
		{ 1, Random::Float(0, 1.0f), Random::Float(0, 1.0f), 1 }, {});
	for (size_t j = 0; j < 2; j++)
	{
		for (size_t i = 0; i < numSamples / 2; i++)
		{
			phi += dphi;
			float _x = r * std::cos(phi);
			float _y = r * std::sin(phi);
			
			m_polygon->PushVertex({ _x, _y },
				{ 1, Random::Float(0, 1.0f), Random::Float(0, 1.0f), 1 }, {});
		}
	}

	phi += dphi;
	float _x = r * std::cos(phi);
	float _y = r * std::sin(phi);
	m_polygon->PushVertex({ _x, _y },
		{ 1, Random::Float(0, 1.0f), Random::Float(0, 1.0f), 1 }, {});

	m_polygon->Update();


	auto game = GameGlobal::game;
	auto& world = game->World();
	auto& b2world = world.B2World();

	b2BodyDef bodyDef;
	bodyDef.userData.pointer = (uintptr_t)this;
	bodyDef.type = b2BodyType::b2_dynamicBody;
	bodyDef.position.Set(x * World::PIXEL_PER_METERS, y * World::PIXEL_PER_METERS);

	m_body = b2world.CreateBody(&bodyDef);

	b2CircleShape circle;
	circle.m_radius = r * World::PIXEL_PER_METERS;
	circle.m_p = { 0,0 };

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = 10.0f;
	fixtureDef.friction = 1.f;
	fixtureDef.restitution = 0.1f;

	m_body->CreateFixture(&fixtureDef);
}

CircleObject::~CircleObject()
{
}

void CircleObject::Update(Game* game)
{
	if (game->Input()->GetPressKey(SPACE))
	{
		m_body->SetLinearVelocity({ 0.f,0.f });
		SetTransform({ 512, 60 }, 0);
	}

	if (game->Input()->GetKey(RIGHT_ARROW))
	{
		m_body->ApplyForce({ 50.0f,0.f }, *(b2Vec2*)&((m_position) *World::PIXEL_PER_METERS), true);
	}

	if (game->Input()->GetKey(LEFT_ARROW))
	{
		m_body->ApplyForce({ -50.0f,0.f }, *(b2Vec2*)&(m_position * World::PIXEL_PER_METERS), true);
	}

	if (game->Input()->GetPressKey(UP_ARROW))
	{
		m_body->ApplyForce({ 0.0f,-1000.f }, *(b2Vec2*)&(m_position * World::PIXEL_PER_METERS), true);
	}
}

void CircleObject::Render(Game* game)
{
	auto& camRect = game->CameraRect();

	auto posInCam = m_position - camRect.m_point;
	Mat3x3 temp;
	temp.SetTranslation(posInCam);
	game->Renderer()->DrawPolygon(0, 1, m_angle, posInCam, 0, m_polygon, temp);
}
