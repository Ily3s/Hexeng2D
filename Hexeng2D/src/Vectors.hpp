#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <cmath>
#include <iostream>

namespace Hexeng
{

	template <typename T>
	struct Vec2
	{
		T x, y;

		Vec2() = default;
		Vec2(T x, T y) : x(x), y(y) {}

		template <typename T2>
		Vec2<T>(Vec2<T2> other) : x((T)other.x), y((T)other.y) {}

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
	std::ostream& operator<<(std::ostream& os, const Vec2<T>& vec)
	{
		os << vec.x << " ; " << vec.y;
		return os;
	}

	template <typename T>
	std::istream& operator>>(std::istream& is, Vec2<T>& vec)
	{
		is >> vec.x >> vec.y;
		return is;
	}

	inline Vec2<int> operator*(Vec2<int> vec, float factor)
	{
		return { (int)((float)vec.x * factor), (int)((float)vec.y * factor) };
	}

	template <typename T>
	struct Vec3
	{
		T x, y, z;

		Vec3() = default;
		Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

		template <typename T2>
		Vec3<T>(Vec3<T2> other) : x((T)other.x), y((T)other.y), z((T)other.z) {}

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
	std::ostream& operator<<(std::ostream& os, const Vec3<T>& vec)
	{
		os << vec.x << " ; " << vec.y << " ; " << vec.z;
		return os;
	}

	template <typename T>
	std::istream& operator>>(std::istream& is, Vec3<T>& vec)
	{
		is >> vec.x >> vec.y >> vec.z;
		return is;
	}

	template <typename T>
	struct Vec4
	{
		T x, y, z, w;

		Vec4() = default;
		Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

		template <typename T2>
		Vec4<T>(Vec4<T2> other) : x((T)other.x), y((T)other.y), z((T)other.z), w((T)other.w) {}

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

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Vec4<T>& vec)
	{
		os << vec.x << " ; " << vec.y << " ; " << vec.z << " ; " << vec.w;
		return os;
	}

	template <typename T>
	std::istream& operator>>(std::istream& is, Vec4<T>& vec)
	{
		is >> vec.x >> vec.y >> vec.z >> vec.w;
		return is;
	}

}

#endif // !VECTORS_HPP