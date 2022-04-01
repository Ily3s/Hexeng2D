#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <cmath>

namespace Hexeng
{

	template <typename T>
	struct Vec2
	{
		T x, y;

		Vec2 operator+(const Vec2& other) const
		{
			return { x + other.x, y + other.y };
		}

		Vec2 operator-(const Vec2& other) const
		{
			return { x - other.x, y - other.y };
		}

		Vec2 operator*(const Vec2& other) const
		{
			return { x * other.x, y * other.y };
		}

		Vec2 operator/(const Vec2& other) const
		{
			return { x / other.x, y / other.y };
		}

		Vec2& operator+=(const Vec2& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		Vec2& operator-=(const Vec2& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		Vec2& operator*=(const Vec2& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}

		Vec2& operator/=(const Vec2& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}

		Vec2 operator+(const T other) const
		{
			return { x + other, y + other };
		}

		Vec2 operator-(const T other) const
		{
			return { x - other, y - other };
		}

		Vec2 operator*(const T other) const
		{
			return { x * other, y * other };
		}

		Vec2 operator/(const T other) const
		{
			return { x / other, y / other };
		}

		Vec2 operator-() const
		{
			return { -x, -y };
		}

		Vec2& operator+=(const T other)
		{
			x += other;
			y += other;
			return *this;
		}

		Vec2& operator-=(const T other)
		{
			x -= other;
			y -= other;
			return *this;
		}

		Vec2& operator*=(const T other)
		{
			x *= other;
			y *= other;
			return *this;
		}

		Vec2& operator/=(const T other)
		{
			x /= other;
			y /= other;
			return *this;
		}

		static Vec2 abs(const Vec2& other)
		{
			return { std::abs(other.x), std::abs(other.y) };
		}
	};

	template <typename T>
	struct Vec3
	{
		T x, y, z;

		Vec3 operator+(const Vec3& other) const
		{
			return { x + other.x, y + other.y, z + other.z };
		}

		Vec3 operator-(const Vec3& other) const
		{
			return { x - other.x, y - other.y, z - other.z };
		}

		Vec3 operator*(const Vec3& other) const
		{
			return { x * other.x, y * other.y, z * other.z };
		}

		Vec3 operator/(const Vec3& other) const
		{
			return { x / other.x, y / other.y, z / other.z };
		}

		Vec3 operator-() const
		{
			return { -x, -y, -z };
		}

		Vec3& operator+=(const Vec3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		Vec3& operator-=(const Vec3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		Vec3& operator*=(const Vec3& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		Vec3& operator/=(const Vec3& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}

		Vec3 operator+(const T other) const
		{
			return { x + other, y + other, z + other };
		}

		Vec3 operator-(const T other) const
		{
			return { x - other, y - other, z - other };
		}

		Vec3 operator*(const T other) const
		{
			return { x * other, y * other, z * other };
		}

		Vec3 operator/(const T other) const
		{
			return { x / other, y / other, z / other };
		}

		Vec3& operator+=(const T other)
		{
			x += other;
			y += other;
			z += other;
			return *this;
		}

		Vec3& operator-=(const T other)
		{
			x -= other;
			y -= other;
			z -= other;
			return *this;
		}

		Vec3& operator*=(const T other)
		{
			x *= other;
			y *= other;
			z *= other;
			return *this;
		}

		Vec3& operator/=(const T other)
		{
			x /= other;
			y /= other;
			z /= other;
			return *this;
		}

		static Vec3 abs(const Vec3& other)
		{
			return { std::abs(other.x), std::abs(other.y), std::abs(other.z) };
		}
	};

	template <typename T>
	struct Vec4
	{
		T x, y, z, w;

		Vec4 operator+(const Vec4& other) const
		{
			return { x + other.x, y + other.y, z + other.z, w + other.w };
		}

		Vec4 operator-(const Vec4& other) const
		{
			return { x - other.x, y - other.y, z - other.z, w - other.w };
		}

		Vec4 operator*(const Vec4& other) const
		{
			return { x * other.x, y * other.y, z * other.z, w * other.w };
		}

		Vec4 operator/(const Vec4& other) const
		{
			return { x / other.x, y / other.y, z / other.z, w / other.w };
		}

		Vec4 operator-() const
		{
			return { -x, -y, -z, -w };
		}

		Vec4& operator+=(const Vec4& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}

		Vec4& operator-=(const Vec4& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}

		Vec4& operator*=(const Vec4& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
			return *this;
		}

		Vec4& operator/=(const Vec4& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
			return *this;
		}

		Vec4 operator+(const T other) const
		{
			return { x + other, y + other, z + other, w + other };
		}

		Vec4 operator-(const T other) const
		{
			return { x - other, y - other, z - other, w - other };
		}

		Vec4 operator*(const T other) const
		{
			return { x * other, y * other, z * other, w * other };
		}

		Vec4 operator/(const T other) const
		{
			return { x / other, y / other, z / other, w / other };
		}

		Vec4& operator+=(const T other)
		{
			x += other;
			y += other;
			z += other;
			w += other;
			return *this;
		}

		Vec4& operator-=(const T other)
		{
			x -= other;
			y -= other;
			z -= other;
			w -= other;
			return *this;
		}

		Vec4& operator*=(const T other)
		{
			x *= other;
			y *= other;
			z *= other;
			w *= other;
			return *this;
		}

		Vec4& operator/=(const T other)
		{
			x /= other;
			y /= other;
			z /= other;
			w /= other;
			return *this;
		}

		static Vec4 abs(const Vec4& other)
		{
			return { std::abs(other.x), std::abs(other.y), std::abs(other.z), std::abs(other.w) };
		}
	};

}

#endif // !VECTORS_HPP