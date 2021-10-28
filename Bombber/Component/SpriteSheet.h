#pragma once

#include <vector>

#include <Engine/Resource/Texture2D.h>

#include <Math/Collision.h>

#include <Engine/Resource.h>

#include <Engine/Engine.h>

class Sprite
{
public:
	Texture2D* m_texture = 0;
	Rect2D m_rect;

public:
	inline Sprite() {};
	inline Sprite(const std::wstring& path, const Rect2D& rect = { 0,0,0,0 })
	{
		m_texture = Resource::Get<Texture2D>(path);

		if (rect.Width() == 0 && rect.Height() == 0)
		{
			m_rect = { 0,0,(float)m_texture->Width(),(float)m_texture->Height() };
		}
		else
		{
			m_rect = rect;
		}
	}

	inline ~Sprite()
	{
		Resource::Release<Texture2D>(&m_texture);
	}

public:
	inline Texture2D* GetRenderTexture() { return m_texture; };
	inline Rect2D* GetRenderRect() { return &m_rect; };

public:
	inline void SetTexture(const std::wstring& path)
	{
		Resource::Release(&m_texture);
		m_texture = Resource::Get<Texture2D>(path);
	}

};