#include "Camera.h"
#include "Game.h"

Camera::Camera(Game* game)
{
	Update(game);

	//game->Input()->SetHideCursor(true);

	new(&m_sprite) Sprite(L"../../../../Bombber/Assets/aim-icon-red.png");

	//game->GetDimensions(&m_windowWidth, &m_windowHeight);
	//game->GetPosition(&m_windowPosX, &m_windowPosY);
	//game->Input()->SetLockMouse(0, posX + m_windowPosX, posY + m_windowPosY);
}

void Camera::Update(Game* game)
{
	HandleMouse(game);
	HandleCamera(game);
}


void Camera::UpdateWhileLock(Game* game)
{
	auto input = game->Input();

	auto deltaMousePos = input->GetDeltaMousePos();

	auto curX = m_preMousePosX + deltaMousePos[0];

	auto curY = m_preMousePosY + deltaMousePos[1];

	ClampMouse(&m_preMousePosX, &m_preMousePosY, &curX, &curY);
}

void Camera::ClampMouse(int* outX, int* outY, int *inX, int* inY)
{
	const int minX = 50;
	const int minY = 50;
	const int paddingRight = 50;
	const int paddingBottom = 50;

	*outX = min(max(minX, *inX), m_windowWidth - paddingRight);
	*outY = min(max(minY, *inY), m_windowHeight - paddingBottom);
}

void Camera::HandleMouse(Game* game)
{
	auto input = game->Input();

	if (input->GetPressKey(TAB))
	{
		if (m_lockMouse)
		{
			input->SetLockMouse(false, m_windowPosX + m_preMousePosX, m_windowPosY + m_preMousePosY);
			input->SetHideCursor(false);
		}
		else
		{
			input->SetLockMouse(true, m_windowPosX + m_windowWidth / 2, m_windowPosY + m_windowHeight / 2);
			input->SetHideCursor(true);
		}
		m_lockMouse = !m_lockMouse;
	}

	if (m_lockMouse)
	{
		UpdateWhileLock(game);
		return;
	}

	auto mousePos = input->GetMousePos();

	game->GetDimensions(&m_windowWidth, &m_windowHeight);
	game->GetPosition(&m_windowPosX, &m_windowPosY);

	int mousePosXInWindow = mousePos[0] - m_windowPosX;
	int mousePosYInWindow = mousePos[1] - m_windowPosY;

	int copyX = mousePosXInWindow;
	int copyY = mousePosYInWindow;

	//std::cout << copyX << ", " << copyY << "\n";

	ClampMouse(&m_preMousePosX, &m_preMousePosY, &mousePosXInWindow, &mousePosYInWindow);
}

void Camera::ClampCamera(Rect2D& camRect)
{
	const float minX = 0, maxX = 0;
	const float minY = 0, maxY = 0;


}

#define Lerp(x,y,s) (y - x) * s

void Camera::HandleCamera(Game* game)
{
	const float minSpeed = 0;
	const float maxSpeed = 100;

	const float halfW = m_windowWidth / 2;
	const float halfH = m_windowHeight / 2;

	const float vecX = m_preMousePosX - halfW;
	const float vecY = m_preMousePosY - halfH;

	//just do lerp()
	const float sX = vecX / halfW;
	const float sY = vecY / halfH;

	const float speedX = Lerp(minSpeed, maxSpeed, sX) * game->FDeltaTime();
	const float speedY = Lerp(minSpeed, maxSpeed, sY) * game->FDeltaTime();

	auto& camRect = game->CameraRect();
	//camRect.m_point = camRect.m_point + Vec2(50  * game->FDeltaTime(), 0);
	camRect.m_point = camRect.m_point + Vec2(speedX, speedY);

	ClampCamera(camRect);

}


void Camera::Render(Game* game)
{
	auto renderer = game->Renderer();

	auto rect = m_sprite.GetRenderRect();
	auto texture = m_sprite.GetRenderTexture();

	const float crossAimW = 100;
	const float crossAimH = 100;

	Rect2D screenRect = {
		(float)m_preMousePosX - crossAimW / 2.0f, 
		(float)m_preMousePosY - crossAimH / 2.0f, 
		crossAimW, 
		crossAimH 
	};

	renderer->Draw(texture, 0, 0, {}, 0, screenRect, *rect);
}
