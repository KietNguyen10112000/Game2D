#include "Renderer2D.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

#include "Window.h"

#include "Resource/Texture2D.h"

#include "RenderAPI/OpenGL/Shader.h" 

#include "Polygon.h"

#pragma comment(lib, "opengl32.lib")

GLuint VertexArrayID;

Renderer2D::Renderer2D(Window* window)
{
	if (!InitSDL(window))
	{
		exit(-1);
	}

#ifdef _DEBUG
	m_polygonShader = LoadShaders("../../../../Bombber/Engine/RenderAPI/OpenGL/GLSL/Polygon.vs", 
		"../../../../Bombber/Engine/RenderAPI/OpenGL/GLSL/Polygon.fs");
#else
	//m_polygonShader = LoadShaders("", "");
	m_polygonShader = LoadShaders("GLSL/Polygon.vs",
		"GLSL/Polygon.fs");
#endif // DEBUG

	m_shaderPolygonTransformLocation = glGetUniformLocation(m_polygonShader, "transform");
	m_shaderScreenDimensionsLocation = glGetUniformLocation(m_polygonShader, "screenDim");
	m_shaderFsInfoLocation = glGetUniformLocation(m_polygonShader, "info");
	m_shaderColorTextureLocation = glGetUniformLocation(m_polygonShader, "colorTexture");
	m_shaderPolygonToWorldLocation = glGetUniformLocation(m_polygonShader, "polygonTransform");

	int w = 0, h = 0;
	SDL_GetWindowSize(m_window, &w, &h);
	m_width = (float)w;
	m_height = (float)h;

	m_polygonTransform = { 0,0,0,0 };
	m_screenDimensions = { (float)m_width, (float)m_height, 0,0 };

	
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//glEnable(GL_MULTISAMPLE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_polygon = new class Polygon(32);

}

Renderer2D::~Renderer2D()
{
	delete m_polygon;

	glDeleteProgram(m_polygonShader);

	SDL_DestroyWindow(m_window);

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool Renderer2D::InitSDL(Window* window)
{
	//Initialization flag
	bool success = true;
	
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		SDL_Window* pSampleWin = SDL_CreateWindow("", 0, 0, 1, 1, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
		char buf[256] = {};
		sprintf(buf, "%p", pSampleWin);
		if (!SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, buf))
		{
			printf("Warning: SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT failed!");
		}

		m_window = SDL_CreateWindowFrom(window->GetNativeHandle());

		SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, nullptr);
		SDL_DestroyWindow(pSampleWin);

		SDL_GL_LoadLibrary(NULL);

		if (m_window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); 
			//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
			
			SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			auto context = SDL_GL_CreateContext(m_window);

			if (context == 0)
			{
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				glewInit();
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}
				glCreateProgram();
			}

			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				success = false;
			}
			
		}
	}

	return success;
}


void Renderer2D::DrawRect(int x, int y, int w, int h, const float* rgba, float border)
{
	
}


void Renderer2D::Draw(Texture2D* texture, float alpha, double angle, const Vec2& point, int flip, 
	float x, float y, float w, float h, float dx, float dy, float tw, float th)
{
	float imgw = texture->Width();
	float imgh = texture->Height();

	float imgx = dx / imgw;
	float imgy = dy / imgh;
	float duvx = tw / imgw;
	float duvy = th / imgh;

	m_polygon->Clear();
	m_polygon->PushVertex({ x		, y },			{},		{ imgx,imgy });
	m_polygon->PushVertex({ x		, y + h },		{},		{ imgx,imgy + duvy });
	m_polygon->PushVertex({ x + w	, y },			{},		{ imgx + duvx,imgy });
	m_polygon->PushVertex({ x + w	, y + h },		{},		{ imgx + duvx,imgy + duvy });

	DrawPolygon(texture, alpha, angle, point, flip, m_polygon);
}

void Renderer2D::Draw(Texture2D* texture, float alpha, double angle, const Vec2& point, int flip, 
	const Rect2Di& screenRect, const Rect2Di& textureRect)
{
	Draw(texture, alpha, angle, point, flip, screenRect.m_x, screenRect.m_y, screenRect.m_w, screenRect.m_h,
		textureRect.m_x, textureRect.m_y, textureRect.m_w, textureRect.m_h);
}

void Renderer2D::Draw(Texture2D* texture, float alpha, double angle, const Vec2& point, int flip, 
	const Rect2D& screenRect, const Rect2D& textureRect)
{
	/*Draw(texture, alpha, angle, point, flip, 
		screenRect.m_topLeft.x, screenRect.m_topLeft.y, 
		screenRect.m_bottomRight.x - screenRect.m_topLeft.x, 
		screenRect.m_bottomRight.y - screenRect.m_topLeft.y,
		textureRect.m_topLeft.x, textureRect.m_topLeft.y,
		textureRect.m_bottomRight.x - textureRect.m_topLeft.x,
		textureRect.m_bottomRight.y - textureRect.m_topLeft.y);*/

	Draw(texture, alpha, angle, point, flip,
		screenRect.m_point.x, screenRect.m_point.y,
		screenRect.Width(),
		screenRect.Height(),
		textureRect.m_point.x, textureRect.m_point.y,
		textureRect.Width(),
		textureRect.Height());
}

//void Renderer2D::DrawPolygon(Texture2D* texture, float alpha, double angle, const Vec2& point, int flip, 
//	class Polygon* polygon)
//{
//	//if (alpha <= 0) return;
//
//	//polygon->Update();
//
//	//auto vb = polygon->m_vb;
//	//auto ib = polygon->m_ib;
//
//	//glUseProgram(m_polygonShader);
//
//	//glBindBuffer(GL_ARRAY_BUFFER, vb);
//	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	//glEnableVertexAttribArray(0);
//	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
//	//glEnableVertexAttribArray(1);
//	//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
//	//glEnableVertexAttribArray(2);
//
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
//
//	//glUniform4fv(m_shaderScreenDimensionsLocation, 1, &m_screenDimensions.x);
//
//	//if (angle != 0)
//	//{
//	//	m_polygonTransform.x = angle;
//	//	m_polygonTransform.y = point.x;
//	//	m_polygonTransform.z = point.y;
//	//}
//	//else
//	//{
//	//	m_polygonTransform.x = 0;
//	//}
//	//glUniform4fv(m_shaderPolygonTransformLocation, 1, &m_polygonTransform.x);
//
//	//if (texture)
//	//{
//	//	auto textureID = texture->GetNativeHandle();
//	//	glActiveTexture(GL_TEXTURE0);
//	//	glBindTexture(GL_TEXTURE_2D, textureID);
//	//	glUniform1i(m_shaderColorTextureLocation, 0);
//	//	m_fsInfo.x = 1;
//	//}
//	//else
//	//{
//	//	m_fsInfo.x = 0;
//	//}
//
//	//m_fsInfo.y = (float)flip;
//	//m_fsInfo.w = alpha;
//	//glUniform4fv(m_shaderFsInfoLocation, 1, &m_fsInfo.x);
//
//	////render
//	//glDrawElements(GL_TRIANGLES, polygon->GetIndicesCount(), GL_UNSIGNED_INT, 0);
//
//	//glDisableVertexAttribArray(0);
//	//glDisableVertexAttribArray(1);
//	//glDisableVertexAttribArray(2);
//	DrawPolygon(texture, alpha, angle, point, flip, polygon, {});
//}

void Renderer2D::DrawPolygon(Texture2D* texture, float alpha, double angle, const Vec2& point, int flip, 
	class Polygon* polygon, const Mat3x3& transform)
{
	if (alpha < 0) return;

	polygon->Update();

	auto vb = polygon->m_vb;
	auto ib = polygon->m_ib;

	glUseProgram(m_polygonShader);

	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);

	glUniform4fv(m_shaderScreenDimensionsLocation, 1, &m_screenDimensions.x);

	if (angle != 0)
	{
		m_polygonTransform.x = angle;
		m_polygonTransform.y = point.x;
		m_polygonTransform.z = point.y;
	}
	else
	{
		m_polygonTransform.x = 0;
	}
	glUniform4fv(m_shaderPolygonTransformLocation, 1, &m_polygonTransform.x);

	glUniformMatrix3fv(m_shaderPolygonToWorldLocation, 1, true, &transform.m[0][0]);

	if (texture)
	{
		auto textureID = texture->GetNativeHandle();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1i(m_shaderColorTextureLocation, 0);
		m_fsInfo.x = 1;
	}
	else
	{
		m_fsInfo.x = 0;
	}

	m_fsInfo.y = (float)flip;
	m_fsInfo.w = alpha;
	glUniform4fv(m_shaderFsInfoLocation, 1, &m_fsInfo.x);

	//render
	glDrawElements(GL_TRIANGLES, polygon->GetIndicesCount(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Renderer2D::ClearFrame(const float* rgba)
{
	glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::Present()
{
	SDL_GL_SwapWindow(m_window);
}
