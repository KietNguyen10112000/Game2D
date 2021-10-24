#pragma once
#include <Math/Math.h>
#include <Math/Collision.h>

struct SDL_Window;
struct SDL_Renderer;

class Window;
class Texture2D;
class Polygon;

class Renderer2D
{
public:
	SDL_Window* m_window = 0;

protected:
	uint32_t m_polygonShader = 0;

	Vec4 m_polygonTransform;
	uint32_t m_shaderPolygonTransformLocation = 0;

	Vec4 m_screenDimensions;
	uint32_t m_shaderScreenDimensionsLocation = 0;

	Vec4 m_fsInfo;
	uint32_t m_shaderFsInfoLocation = 0;

	uint32_t m_shaderColorTextureLocation = 0;

	//to render quad, triangle, line, ...
	class Polygon* m_polygon = 0;

public:
	int m_width = 0;
	int m_height = 0;

public:
	Renderer2D(Window* window);
	~Renderer2D();

protected:
	bool InitSDL(Window* window);

public:
	//border == FLT_MAX to solid fill
	void DrawRect(int x, int y, int w, int h, const float* rgba, float border);

	//draw texture2d at (dx, dy) + dimension(tw, th) to screen at (x, y), dimension(w,h), rotated angle
	void Draw(Texture2D* texture, float alpha, double angle, const Vec2& point, int flip,
		float x, float y, float w, float h, float dx, float dy, float tw, float th);

	void Draw(Texture2D* texture, float alpha, double angle, const Vec2& point, int flip,
		const Rect2Di& screenRect, const Rect2Di& textureRect);

	void Draw(Texture2D* texture, float alpha, double angle, const Vec2& point, int flip,
		const Rect2D& screenRect, const Rect2D& textureRect);

	void DrawPolygon(Texture2D* texture, float alpha, double angle, const Vec2& point, int flip,
		class Polygon* polygon);

	void ClearFrame(const float* rgba);

	void Present();

};