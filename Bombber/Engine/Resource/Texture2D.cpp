#include "Texture2D.h"

#include "../Global.h"

#include "../Renderer2D.h"

#include <Engine/Common.h>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

Texture2D::Texture2D(const std::wstring& path, uint32_t numArg, void** args) : IResource(path)
{
	Free();

	//The final texture
	uint32_t textureID = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(WStringToString(path).c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Get image dimensions
		m_width = loadedSurface->w;
		m_height = loadedSurface->h;

		auto type = GL_RGBA;
		if (loadedSurface->format->BitsPerPixel == 24) type = GL_RGB;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, type, GL_UNSIGNED_BYTE, loadedSurface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	m_texture = textureID;
}

Texture2D::~Texture2D()
{
	Free();
}

inline void Texture2D::Free()
{
	glDeleteTextures(1, &m_texture);
}
