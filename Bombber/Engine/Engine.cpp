#include "Engine.h"

#include "Renderer2D.h"

#include "Global.h"

#include "Resource.h"


Engine::Engine(const wchar_t* title, int width, int height) : Window(title, width, height)
{
	m_renderer = new Renderer2D(this);
	m_resourceManager = new Resource();

	Global::engine = this;
	Global::input = Input();
	Global::renderer = m_renderer;
	Global::resourceManager = m_resourceManager;
}

Engine::~Engine()
{
	delete m_renderer;
	delete m_resourceManager;
}