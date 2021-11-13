#pragma once
#include "Window.h"

#define _TIME_FACTOR 1000.0f


#define _TIMING_						\
auto cur = GetTime();					\
m_deltaTime = cur - m_currentTime;		\
m_currentTime = cur;					\
m_time += m_deltaTime / _TIME_FACTOR;	\
m_input->deltaClickTimer += m_deltaTime;	\
Sleep(2);								


class Renderer2D;
class Resource;

class Engine : public Window
{
protected:
	uint64_t m_currentTime = 0;
	uint64_t m_deltaTime = 0;
	float m_time = 0;

	Renderer2D* m_renderer = nullptr;
	Resource* m_resourceManager = nullptr;

public:
	Engine(const wchar_t* title, int width, int height);
	virtual ~Engine();

public:
	inline auto Input() { return m_input; };
	inline auto Renderer() { return m_renderer; };
	inline auto ResourceManager() { return m_resourceManager; };

	//in milisec
	inline auto DeltaTime() { return m_deltaTime; };

	//in sec
	inline auto FDeltaTime() { return m_deltaTime / _TIME_FACTOR; };

	//in milisec, from 1/1/1970
	inline auto CurrentTime() { return m_currentTime; };

	//in sec, from 1/1/1970
	inline auto FCurrentTime() { return m_currentTime / _TIME_FACTOR; };

	//in sec, from start engine
	inline auto FTime() { return m_time; };

};