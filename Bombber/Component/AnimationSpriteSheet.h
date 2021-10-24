#pragma once

#include <vector>
#include <Engine/Resource/Texture2D.h>

#include <Math/Collision.h>

#include <Engine/Resource.h>

#include <Engine/Engine.h>

struct Anim2DKeyFrame
{
	float tick;
	Rect2D frame;
};

class Animation2DSingleTexture
{
public:
	std::string m_name;

	Texture2D* m_texture = 0;
	std::vector<Anim2DKeyFrame> m_frames;
	float m_totalTick = 0;
	float m_tickPerSecond = 24.0f; //default is 24 tick per second

public:
	inline Animation2DSingleTexture(const Animation2DSingleTexture& anim)
	{
		m_frames = anim.m_frames;
		m_totalTick = anim.m_totalTick;
		m_tickPerSecond = anim.m_tickPerSecond;
		m_texture = Resource::Get(anim.m_texture);
	};

	inline Animation2DSingleTexture(const std::wstring& texturePath, const std::vector<Rect2D>& frame)
	{
		size_t count = 1;
		for (auto& f : frame)
		{
			m_frames.push_back({ (float)count, f });
			count++;
		}
		m_totalTick = count - 1;

		m_tickPerSecond = m_tickPerSecond > m_totalTick ? 1 : m_tickPerSecond;

		m_texture = Resource::Get<Texture2D>(texturePath);
	};

	inline Animation2DSingleTexture(const std::wstring& texturePath, const std::vector<Rect2D>& frame,
		std::string name) : Animation2DSingleTexture(texturePath, frame)
	{
		m_name = name;
	};

	inline ~Animation2DSingleTexture()
	{
		Resource::Release(&m_texture);
	};

	inline Texture2D* GetTexture(int index) { return m_texture; };
};


class Animation2DMultiTexture
{
public:
	std::string m_name;

	std::vector<Anim2DKeyFrame> m_frames;
	std::vector<Texture2D*> m_textures;
	float m_totalTick = 0;
	float m_tickPerSecond = 24.0f; //default is 24 tick per second

public:
	inline Animation2DMultiTexture(const Animation2DMultiTexture& anim)
	{
		size_t i = 0;
		for (auto& f : anim.m_frames)
		{
			m_frames.push_back(f);
			m_textures.push_back(Resource::Get(anim.m_textures[i]));
			i++;
		}
		m_totalTick = anim.m_totalTick;
		m_tickPerSecond = anim.m_tickPerSecond;
	};

	inline Animation2DMultiTexture(const std::vector<std::wstring>& texturesPath, const std::vector<Rect2D>& frame)
	{
		size_t count = 1;
		for (auto& f : frame)
		{
			m_frames.push_back({(float)count, f });
			m_textures.push_back(Resource::Get<Texture2D>(texturesPath[count - 1]));
			count++;
		}
		m_totalTick = count - 1;

		m_tickPerSecond = m_tickPerSecond > m_totalTick ? 1 : m_tickPerSecond;
	};

	inline Animation2DMultiTexture(const std::vector<std::wstring>& texturesPath, const std::vector<Rect2D>& frame,
		std::string name) : Animation2DMultiTexture(texturesPath, frame)
	{
		m_name = name;
	};

	inline ~Animation2DMultiTexture()
	{
		for (auto& t : m_textures)
		{
			Resource::Release(&t);
		}
	};

	inline Texture2D* GetTexture(int index) { return m_textures[index]; };
};

template<typename _Animation>
class Animator2D
{
public:
	std::vector<_Animation> m_animations;

	_Animation* m_curAnimation = 0;

	float m_time = 0;
	float m_durationRatio = 1;

	int m_curIndex = 0;


protected:
	inline void GetIndex(int& index, float time, std::vector<Anim2DKeyFrame>& keyframe)
	{
		if (keyframe.size() == 0)
		{
			index = -1;
			return;
		}
		else if (keyframe.size() == 1)
		{
			index = 0;
			return;
		}

		if (time > keyframe.back().tick)
		{
			index = keyframe.size() - 1;
		}
		else
		{
			while (keyframe[index].tick < time)
			{
				index++;
			}
		}
	};

public:
	inline void Play(Engine* engine, bool* end = 0)
	{
		m_time += engine->FDeltaTime();

		auto time = m_time * m_durationRatio * m_curAnimation->m_tickPerSecond;

		if (time >= m_curAnimation->m_totalTick)
		{
			m_time = 0;
			time = 0;
			m_curIndex = 0;

			if (end) *end = true;
		}
		else
		{
			if (end) *end = false;
		}

		GetIndex(m_curIndex, time, m_curAnimation->m_frames);

	};

	inline bool SetAnimation(int index)
	{
		if (index < 0 || index >= m_animations.size()) return 0;
		m_curAnimation = &m_animations[index];
		return 1;
	};

	inline bool SetAnimation(const std::string& name)
	{
		for (size_t i = 0; i < m_animations.size(); i++)
		{
			if (m_animations[i].m_name == name)
				return SetAnimation(i);
		}
		return 0;
	};

	//duration in sec
	inline void SetDuration(float duration)
	{
		if (duration <= 0) return;
		auto durationTime = (m_curAnimation->m_totalTick / m_curAnimation->m_tickPerSecond) / m_durationRatio;
		m_time = m_time * (duration / durationTime);
		m_durationRatio = (m_curAnimation->m_totalTick / m_curAnimation->m_tickPerSecond) / duration;
	};

	inline void Reset(bool resetTimer)
	{
		m_durationRatio = 1;
		if (resetTimer) m_time = 0;
	}

public:
	inline float GetDuration() { return (m_curAnimation->m_totalTick / m_curAnimation->m_tickPerSecond) / m_durationRatio; };

public:
	inline Rect2D* GetRenderRect() { return &m_curAnimation->m_frames[m_curIndex].frame; };
	inline Texture2D* GetRenderTexture() { return m_curAnimation->GetTexture(m_curIndex); };

public:
	inline void AddAnimation(const _Animation& animation)
	{
		m_animations.push_back(animation);
	};

};

//each animation is on only one texture,
//can contain many animations on many textures
typedef Animator2D<Animation2DSingleTexture> Animator2DSingleTexture;

//each key frame in animation is on a texture,
typedef Animator2D<Animation2DMultiTexture> Animator2DMultiTexture;