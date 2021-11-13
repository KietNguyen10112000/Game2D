#include "Camera.h"
#include "Game.h"

Camera::Camera(Game* game)
{
	m_camRect = &game->m_camRect;

	Update(game);

	//game->Input()->SetHideCursor(true);

	new(&m_sprite) Sprite(L"../../../../Bombber/Assets/aim-icon-red.png");

	//game->GetDimensions(&m_windowWidth, &m_windowHeight);
	//game->GetPosition(&m_windowPosX, &m_windowPosY);
	//game->Input()->SetLockMouse(0, posX + m_windowPosX, posY + m_windowPosY);
}

float crossAimAngle = 0, scale = PI;

void Camera::Update(Game* game)
{
	HandleMouse(game);
	HandleCamera(game);

	if (game->Input()->GetMouseButton(LEFT))
	{
		crossAimAngle += (PI * 1.5f) * game->FDeltaTime();

		scale += (PI * 1.5f) * game->FDeltaTime();
	}
	else
	{
		crossAimAngle = 0;
		scale = PI;
	}
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
	auto& camTopLeft = camRect.m_point;

	auto camDiagonal = camRect.Diagonal();

	auto camBottomRight = camTopLeft + camDiagonal;
	auto bottomRightX = min(camBottomRight.x, m_camLimitBottomRight.x);
	auto bottomRightY = min(camBottomRight.y, m_camLimitBottomRight.y);

	auto newTopLeft = Vec2(bottomRightX, bottomRightY) - camDiagonal;

	auto topLeftX = max(newTopLeft.x, m_camLimitTopLeft.x);
	auto topLeftY = max(newTopLeft.y, m_camLimitTopLeft.y);

	camTopLeft = { topLeftX, topLeftY };


}

#define Lerp(x,y,s) (y*s + x*(1-s))

void Camera::HandleCamera(Game* game)
{
	if (m_targetPosition)
	{
		m_camRect->m_point = *m_targetPosition - m_followOffset;
	}
	else
	{
		switch (m_speedAcceType)
		{
		case Camera::LINEAR:
			HandleCameraLinear(game);
			break;
		case Camera::EXPONENT:
			HandleCameraExp(game);
			break;
		default:
			break;
		}
	}
	
	auto& camRect = game->CameraRect();
	ClampCamera(camRect);
}

void Camera::HandleCameraLinear(Game* game)
{
	const float minSpeed = m_speedLowerBound;
	const float maxSpeed = m_speedUpperBound;

	const float halfW = m_windowWidth / 2;
	const float halfH = m_windowHeight / 2;

	float vecX = m_preMousePosX - halfW;
	float vecY = m_preMousePosY - halfH;

	//just do exp lerp()
	const float sX = vecX / halfW;
	const float sY = vecY / halfH;

	const float speedX = Lerp(minSpeed, maxSpeed, std::abs(sX)) * game->FDeltaTime();
	const float speedY = Lerp(minSpeed, maxSpeed, std::abs(sY)) * game->FDeltaTime();

	vecX = (vecX != 0 && vecX > 0) ? 1 : -1;
	vecY = (vecY != 0 && vecY > 0) ? 1 : -1;

	auto vecSpeed = Vec2(vecX, vecY) * Vec2(speedX, speedY);

	auto& camRect = game->CameraRect();
	camRect.m_point = camRect.m_point + vecSpeed;
}

void Camera::HandleCameraExp(Game* game)
{
	const float minSpeed = m_speedLowerBound;
	const float maxSpeed = m_speedUpperBound;

	const float halfW = m_windowWidth / 2;
	const float halfH = m_windowHeight / 2;

	float vecX = m_preMousePosX - halfW;
	float vecY = m_preMousePosY - halfH;

	//just do exp lerp()
	const float sX = vecX / halfW;
	const float sY = vecY / halfH;

	const float speedX = std::exp(Lerp(minSpeed, maxSpeed, std::abs(sX))) * game->FDeltaTime();
	const float speedY = std::exp(Lerp(minSpeed, maxSpeed, std::abs(sY))) * game->FDeltaTime();

	vecX = (vecX != 0 && vecX > 0) ? 1 : -1;
	vecY = (vecY != 0 && vecY > 0) ? 1 : -1;

	auto vecSpeed = Vec2(vecX, vecY) * Vec2(speedX, speedY);

	auto& camRect = game->CameraRect();
	camRect.m_point = camRect.m_point + vecSpeed;
}

void Camera::HandleFollow(Game* game)
{
	m_camRect->m_point = *m_targetPosition - m_followOffset;
}

void Camera::Follow(Vec2* position)
{
	m_targetPosition = position;

	if (position)
	{
		switch (m_followType)
		{
		case Camera::CENTER:
			m_followOffset = { m_windowWidth / 2.0f,m_windowHeight / 2.0f };
			break;
		case Camera::LOCK_POSITION:
			m_followOffset = *m_targetPosition - m_camRect->m_point;
			break;
		default:
			break;
		}
	}
	
}


void Camera::Render(Game* game)
{
	auto renderer = game->Renderer();

	auto rect = m_sprite.GetRenderRect();
	auto texture = m_sprite.GetRenderTexture();

	auto _scale = (std::cos(scale) / 2.0f + 2);
	const float crossAimW = 100 * _scale;
	const float crossAimH = 100 * _scale;

	Rect2D screenRect = {
		(float)m_preMousePosX - crossAimW / 2.0f, 
		(float)m_preMousePosY - crossAimH / 2.0f, 
		crossAimW, 
		crossAimH 
	};

	renderer->Draw(texture, 0, crossAimAngle, { (float)m_preMousePosX,(float)m_preMousePosY }, 0, screenRect, *rect);
}
