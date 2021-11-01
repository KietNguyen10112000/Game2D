#pragma once

#include <Math/Math.h>

#include <vector>

class Polygon
{
private:
	friend class Renderer2D;

public:
	enum LIMIT
	{
		//64k vertices
		MAX_VERTEX = 65536
	};

	struct Vertex
	{
		Vec2 pos;
		Vec2 uv;
		Vec4 color;
	};

	enum PIVOT_TYPE
	{
		NONE_PIVOT = -1
	};

protected:
	uint32_t m_vb = 0;
	uint32_t m_ib = 0;

public:
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

	bool m_needUpdate = 0;

	long long m_pivotVertex = NONE_PIVOT;

public:
	Polygon(size_t capacity = MAX_VERTEX);
	~Polygon();

public:
	void Update();

	inline void SetVertexPivot(long long index)
	{
		//if (index < 0 || index >= m_vertices.size()) return;
		m_pivotVertex = index;
	}

public:
	inline void PushVertex(const Vec2& pos, const Vec4& color = {}, const Vec2& uv = {})
	{
		auto size = m_vertices.size();

		if (size > 2)
		{
			if (m_pivotVertex == NONE_PIVOT)
			{
				m_indices.push_back(size - 2);
				m_indices.push_back(size - 1);
				m_indices.push_back(size);
			}
			else
			{
				m_indices.push_back(m_pivotVertex);
				m_indices.push_back(size - 1);
				m_indices.push_back(size);
			}
		}
		else
		{
			m_indices.push_back(size);
		}

		m_vertices.push_back({ pos, uv, color });
		m_needUpdate = true;
	};

	inline void PopVertex()
	{
		m_vertices.pop_back();

		auto size = m_indices.size();

		if (size >= 6)
			m_indices.resize(m_indices.size() - 3);
		else
			m_indices.pop_back();

		m_needUpdate = true;
	};

	inline void Clear() 
	{
		m_vertices.clear();
		m_indices.clear();

		m_needUpdate = true;
	};

public:
	inline auto GetVerticesCount() { return m_vertices.size(); };
	inline auto GetIndicesCount() { return m_indices.size(); };

};