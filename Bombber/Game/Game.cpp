#include "Game.h"

#include <Engine/Renderer2D.h>
#include <Engine/Resource.h>

#include <Component/AnimationSpriteSheet.h>

#include <Engine/Polygon.h>

#include "Global.h"

#include "Example.h"

Animator2DSingleTexture* animator = 0;
Animator2DMultiTexture* animator2 = 0;

class Polygon* polygon = 0;

Texture2D* texture = 0;

RectObject* obj = 0;
RectObject* ground = 0;

Game::Game() : Engine(L"Bombber", 1280, 720)
{
	GameGlobal::game = this;

	m_camRect = { 0,0,1280,720 };

	animator = new Animator2DSingleTexture();
	animator->AddAnimation({
		L"../../../../Bombber/Assets/foo.png",
		{
			{ 0,		0,		64,		205 },
			{ 64,		0,		64,		205 },
			{ 128,		0,		64,		205 },
			{ 196,		0,		64,		205 },
		}
		});
	animator->SetAnimation(0);
	animator->SetDuration(0.5);


	//init animator 2
	animator2 = new Animator2DMultiTexture();
	animator2->AddAnimation({
		{
			L"../../../../Bombber/Assets/foo.png",
			L"../../../../Bombber/Assets/foo.png",
			L"../../../../Bombber/Assets/foo.png",
			L"../../../../Bombber/Assets/foo.png",
		},
		{
			{ 0,		0,		64,		205 },
			{ 64,		0,		64,		205 },
			{ 128,		0,		64,		205 },
			{ 196,		0,		64,		205 },
		}
		});
	animator2->SetAnimation(0);
	animator2->SetDuration(0.5);


	polygon = new class Polygon();
	polygon->PushVertex({ 0,128 }, { 1,0,0,1 }, {0,0});
	polygon->PushVertex({ 0,256 }, { 0,1,1,1 }, {0,1});
	polygon->PushVertex({ 256,0 }, { 1,1,0,1 }, {1,0});
	polygon->PushVertex({ 256,256 }, { 1,0,1,1 }, {1,1});

	texture = Resource::Get<Texture2D>(L"../../../../Bombber/Assets/foo.png");

	obj = new RectObject(512, 256, 50, 50);
	obj->SetTransform({ 512,256 }, PI / 3.0f);

	ground = new RectObject(0, 600, 3000, 20);
	ground->SetPhysicType(Object2D::STATIC);
	ground->_Update(this);
	ground->GetSprite().SetTexture(L"../../../../Bombber/Assets/copper.png");

	_TIMING_;
}

Game::~Game()
{
}

void Game::Update()
{
	_TIMING_;

	////do update
	//if (Input()->GetPressKey('W'))
	//{
	//	std::cout << animator->GetDuration() << "\n";
	//}

	animator->Play(this);
	animator2->Play(this);

	m_world.Update(this);

	obj->_Update(this);

	//render
	Render();
}

void Game::Render()
{
	const float rgba[] = { 0,0.5,0.5,1 };
	Renderer()->ClearFrame(rgba);

	m_world.Render(this);

	auto texture = animator->GetRenderTexture();
	auto rect = animator->GetRenderRect();
	Renderer()->Draw(texture, 1, 0, {}, 0, { 0,0,rect->Width(),rect->Height() }, *rect);

	texture = animator2->GetRenderTexture();
	rect = animator2->GetRenderRect();

	static float a = 0;
	a += (PI / 10) * FDeltaTime();

	//Rect2D screenRect = { 256,0,rect->Width(),rect->Height() };
	//Renderer()->Draw(texture, 1, a, screenRect.Center(), 0, screenRect, *rect);
	Render(animator2, { 256,256 }, a, { 1,1 }, 0);

	Renderer()->DrawPolygon(texture, 0.1, 0, {}, 0, polygon);

	obj->Render(this);
	ground->Render(this);

	Renderer()->Present();
}
