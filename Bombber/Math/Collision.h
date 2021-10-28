#pragma once
#include <cmath>

//#include <algorithm>

#include <minmax.h>

#include <DirectXCollision.h>

#include "Math.h"

namespace Math
{

#define IsFloatEqual(f1, f2, epsilon) (std::abs(f1 - f2) < epsilon)


inline bool Point2DInTriangle(const Vec2& p, const Vec2& p0, const Vec2& p1, const Vec2& p2)
{
	auto s = (p0.x - p2.x) * (p.y - p2.y) - (p0.y - p2.y) * (p.x - p2.x);
	auto t = (p1.x - p0.x) * (p.y - p0.y) - (p1.y - p0.y) * (p.x - p0.x);

	if ((s < 0) != (t < 0) && s != 0 && t != 0)
		return false;

	auto d = (p2.x - p1.x) * (p.y - p1.y) - (p2.y - p1.y) * (p.x - p1.x);

	return d == 0 || (d < 0) == (s + t <= 0);
}

//typedef Vec4 Plane3D;

typedef Vec3 Plane2D;

class Line3D
{
private:
	friend class Plane3D;

public:
	Vec3 m_point;
	Vec3 m_direction;

public:
	inline Line3D() {};

	inline Line3D(const Vec3& point, const Vec3& direction) : m_point(point), m_direction(direction)
	{
		m_direction.Normalize();
	};

	inline Line3D(void* padding, const Vec3& point1, const Vec3& point2) : m_point(point1)
	{
		m_direction = (point2 - point1).Normalize();
	};


public:
	Vec3 Intersect(const Plane3D& plane, bool* done = 0) const noexcept;

public:
	inline std::string ToString() const noexcept
	{
		std::string ret = "Line3D[point = ";
		ret += m_point.ToString();
		ret += ", direction = " + m_direction.ToString() + "]";
		return ret;
	};

	inline friend std::ostream& operator<<(std::ostream& os, const Line3D& line)
	{
		os << line.ToString();
		return os;
	};

};


//ax + by + cz + d = 0;
//(a,b,c,d) ~ (x,y,z,w)
class Plane3D : public XMFLOAT4
{
public:
	inline Plane3D() :XMFLOAT4(0, 0, 0, 0) {};
	inline Plane3D(const Vec3& v1, const Vec3& v2, const Vec3& v3)
	{
		XMStoreFloat4(this, XMPlaneFromPoints(XMLoadFloat3(&v1), XMLoadFloat3(&v2), XMLoadFloat3(&v3)));
		Normalize();
	};

	inline Plane3D(const Vec3& point, const Vec3& normal)
	{
		XMStoreFloat4(this, XMPlaneFromPointNormal(XMLoadFloat3(&point), XMLoadFloat3(&normal)));
		Normalize();
	};

public:
	inline Plane3D& Normalize()
	{
		XMStoreFloat4(this, XMPlaneNormalize(XMLoadFloat4(this)));
		return *this;
	};

	inline Vec3 GetNormal() const noexcept
	{
		return Vec3(x, y, z);
	};

	//get a point on Plane
	inline Vec3 GetPoint() const noexcept
	{
		if (x != 0) return Vec3(-w / x, 0, 0);
		else if (y != 0) return Vec3(0, -w / y, 0);
		return Vec3(0, 0, -w / z);
	};

public:
	inline Line3D Intersect(const Plane3D& plane, bool* done = 0) const noexcept
	{
		Line3D ret;

		//assume that n1, n2 normalized
		auto n1 = GetNormal();
		auto n2 = plane.GetNormal();

		if (n1 == n2)
		{
			if (done) *done = false;
			return {};
		}

		ret.m_direction = CrossProduct(n1, n2).Normalize();

		Line3D line = Line3D(GetPoint(), CrossProduct(n1, ret.m_direction));
		ret.m_point = line.Intersect(plane);

		return ret;
	};

	inline float Distance(const Vec3& point) const noexcept
	{
		return std::abs(DotProduct(Vec4(point, 1.0f), (*(Vec4*)this)));
	};

	//just cal value of Plane with an specify point
	inline float Value(const Vec3& point) const noexcept
	{
		return Distance(point);
	};

	//plane must be normlize
	//get vector that translate from the nearest point on plane of point to point 
	inline Vec3 VecDistance(const Vec3& point) const noexcept
	{
		auto n = GetNormal();
		auto dis = DotProduct(Vec4(point, 1.0f), (*(Vec4*)this));
		return n * dis;
	}

public:
	inline std::string ToString() const noexcept
	{
		std::string ret = "Plane3D[point = ";
		ret += GetPoint().ToString();
		ret += ", normal = " + GetNormal().ToString() + "]";
		return ret;
	};

	inline friend std::ostream& operator<<(std::ostream& os, const Plane3D& plane)
	{
		os << plane.ToString();
		return os;
	};

};

inline Vec3 Line3D::Intersect(const Plane3D& plane, bool* done) const noexcept
{
	auto vec = plane.VecDistance(m_point);

	auto length = vec.Length();

	if (length == 0) return m_point;

	auto dot = DotProduct(vec.Normal(), m_direction);

	if (dot == 0)
	{
		if (done) *done = false;
		return Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	}
	else
	{
		return m_point - (m_direction * length) / dot;
	}
}

inline Plane3D PlaneFrom(const Vec3& v1, const Vec3& v2, const Vec3& v3) noexcept
{
	Plane3D ret;
	XMStoreFloat4(&ret, XMPlaneFromPoints(XMLoadFloat3(&v1), XMLoadFloat3(&v2), XMLoadFloat3(&v3)));
	return ret;
}

inline Plane3D PlaneFrom(const Vec3& point, const Vec3& normal) noexcept
{
	Plane3D ret;
	XMStoreFloat4(&ret, XMPlaneFromPointNormal(XMLoadFloat3(&point), XMLoadFloat3(&normal)));
	return ret;
}

inline Vec3 PlaneToPlane(const Plane3D& p1, const Plane3D& p2) noexcept
{

}


class Rect2Di
{
public:
	int m_x = 0;
	int m_y = 0;
	int m_w = 0;
	int m_h = 0;

public:
	inline Rect2Di(int x, int y, int w, int h)
	{
		m_x = x;
		m_y = y;
		m_w = w;
		m_h = h;
	}


};

class Rect2D
{
public:
	//Vec2 m_topLeft;
	//Vec2 m_bottomRight;

	//     vec1
	//o----------->
	//|
	//vec2
	//|
	//V
	Vec2 m_point;
	Vec2 m_vec1;
	Vec2 m_vec2;

public:
	inline Rect2D() {};
	inline Rect2D(float x, float y, float w, float h)
	{
		//m_topLeft = { x,y };
		//m_bottomRight = { x + w, y + h };
		m_point = { x,y };
		m_vec1 = { w, 0 };
		m_vec2 = { 0, h };
	}

public:
	inline auto Width() const { return m_vec1.Length(); }
	inline auto Height() const { return m_vec2.Length(); }
	inline auto Center() const { return m_point + (m_vec1 + m_vec2) / 2.0f; }
	inline auto Diagonal() const { return m_vec1 + m_vec2; }

	inline auto Area() const { return m_vec1.Length() * m_vec2.Length(); }

	//"can" but not sure
	inline bool CanOverlap(const Rect2D& rect) const
	{
		const Rect2D* rect1 = this;
		const Rect2D* rect2 = &rect;

		auto p1 = rect1->m_point;
		auto p2 = rect1->m_point + rect1->m_vec1;
		auto p3 = rect1->m_point + rect1->m_vec2;
		auto p4 = p3 + rect1->m_vec1;

		auto _p1 = rect2->m_point;
		auto _p2 = rect2->m_point + rect2->m_vec1;
		auto _p3 = rect2->m_point + rect2->m_vec2;
		auto _p4 = _p3 + rect2->m_vec1;

		auto rectALeft = min(min(min(p1.x, p2.x), p3.x), p4.x);
		auto rectARight = max(max(max(p1.x, p2.x), p3.x), p4.x);
		auto rectATop = min(min(min(p1.y, p2.y), p3.y), p4.y);
		auto rectABottom = max(max(max(p1.y, p2.y), p3.y), p4.y);

		auto rectBLeft = min(min(min(_p1.x, _p2.x), _p3.x), _p4.x);
		auto rectBRight = max(max(max(_p1.x, _p2.x), _p3.x), _p4.x);
		auto rectBTop = min(min(min(_p1.y, _p2.y), _p3.y), _p4.y);
		auto rectBBottom = max(max(max(_p1.y, _p2.y), _p3.y), _p4.y);

		return rectALeft < rectBRight && rectARight > rectBLeft &&
			rectATop < rectBBottom && rectABottom > rectBTop;
	}

	inline bool IsOverlap(const Rect2D& rect) const
	{
		auto area1 = Area();
		auto area2 = rect.Area();

		const Rect2D* rect1 = 0;
		const Rect2D* rect2 = 0;

		if (area1 > area2)
		{
			rect1 = &rect;
			rect2 = this;
		}
		else
		{
			rect2 = &rect;
			rect1 = this;
		}

		auto p1 = rect1->m_point;
		auto p2 = rect1->m_point + rect1->m_vec1;
		auto p3 = rect1->m_point + rect1->m_vec2;
		auto p4 = p3 + rect1->m_vec1;

		auto _p1 = rect2->m_point;
		auto _p2 = rect2->m_point + rect2->m_vec1;
		auto _p3 = rect2->m_point + rect2->m_vec2;
		auto _p4 = _p3 + rect2->m_vec1;

		return Point2DInTriangle(p1, _p1, _p2, _p3) || Point2DInTriangle(p2, _p1, _p2, _p3) 
			|| Point2DInTriangle(p3, _p1, _p2, _p3) || Point2DInTriangle(p4, _p1, _p2, _p3)
			|| Point2DInTriangle(p1, _p4, _p2, _p3) || Point2DInTriangle(p2, _p4, _p2, _p3)
			|| Point2DInTriangle(p3, _p4, _p2, _p3) || Point2DInTriangle(p4, _p4, _p2, _p3);
	}

	//self rotation
	void Rotate(float radAngle)
	{
		Vec2 pivot = Center();

		auto cosAngle = cos(radAngle);
		auto sinAngle = sin(radAngle);

		float rX = pivot.x + (m_point.x - pivot.x) * cosAngle - (m_point.y - pivot.y) * sinAngle;
		float rY = pivot.y + (m_point.x - pivot.x) * sinAngle + (m_point.y - pivot.y) * cosAngle;

		m_point = { rX,rY };

		rX = (m_vec1.x) * cosAngle - (m_vec1.y) * sinAngle;
		rY = (m_vec1.x) * sinAngle + (m_vec1.y) * cosAngle;

		m_vec1 = { rX,rY };

		rX = (m_vec2.x) * cosAngle - (m_vec2.y) * sinAngle;
		rY = (m_vec2.x) * sinAngle + (m_vec2.y) * cosAngle;

		m_vec2 = { rX,rY };
	}

};

};