#pragma once
#include <Windows.h>
#include "Input.h"
#include <string>

class Window
{
private:
	HWND m_hwnd = NULL;

protected:
	int m_offsetX = 0;
	int m_offsetY = 0;

	int m_x = 0;
	int m_y = 0;
	int m_width = 0;
	int m_height = 0;

	std::wstring m_title;

	Input* m_input = nullptr;

public:
	//width, height resolution
	Window(const wchar_t* title, int& width, int& height, bool fullScreen = false);
	virtual ~Window();

protected:
	void CalOffset();

public:
	//must be expand
	inline virtual void Update() {};

public:
	void Loop();

public:
	auto& GetNativeHandle() const { return m_hwnd; };

	inline void GetPosition(int* x, int* y) { *x = m_x; *y = m_y; };
	inline void GetDimensions(int* w, int* h) { *w = m_width; *h = m_height; };

};