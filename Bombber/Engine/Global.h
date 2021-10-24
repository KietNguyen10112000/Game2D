#pragma once

class Engine;
class Input;
class Resource;
class Renderer2D;

class Global
{
public:
	inline static Engine* engine = nullptr;
	inline static Renderer2D* renderer = nullptr;
	inline static Input* input = nullptr;
	inline static Resource* resourceManager = nullptr;


};