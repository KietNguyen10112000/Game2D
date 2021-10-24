#pragma once

#include <Engine/IResource.h>

#include <SDL.h>
#include <SDL_image.h>

class Texture2D : public IResource
{
protected:
	//SDL_Texture* m_texture = 0;
	uint32_t m_texture = 0;

	int m_width = 0;
	int m_height = 0;

public:
	enum FLAG
	{
		DISCARD_SRGB = 1
	};

public:
	Texture2D(const std::wstring& path, uint32_t numArg, void** args);
	~Texture2D();

public:
	inline auto& GetNativeHandle() { return m_texture; };
	inline void Free();

public:
	inline auto Width() { return m_width; };
	inline auto Height() { return m_height; };

};