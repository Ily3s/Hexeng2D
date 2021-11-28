#ifndef VECTORS_HPP
#define VECTORS_HPP

namespace Hexeng
{

	template <typename T>
	struct Vec2
	{
		T x, y;

		const Vec2 operator+(const Vec2& other)
		{
			return { x + other.x, y + other.y };
		}

		const Vec2 operator-(const Vec2& other)
		{
			return { x - other.x, y - other.y };
		}

		const Vec2 operator*(const Vec2& other)
		{
			return { x * other.x, y * other.y };
		}

		const Vec2 operator/(const Vec2& other)
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

		const Vec2 operator+(const T other)
		{
			return { x + other, y + other };
		}

		const Vec2 operator-(const T other)
		{
			return { x - other, y - other };
		}

		const Vec2 operator*(const T other)
		{
			return { x * other, y * other };
		}

		const Vec2 operator/(const T other)
		{
			return { x / other, y / other };
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
	};

	template <typename T>
	struct Vec3
	{
		T x, y, z;

		const Vec3 operator+(const Vec3& other)
		{
			return { x + other.x, y + other.y, z + other.z };
		}

		const Vec3 operator-(const Vec3& other)
		{
			return { x - other.x, y - other.y, z - other.z };
		}

		const Vec3 operator*(const Vec3& other)
		{
			return { x * other.x, y * other.y, z * other.z };
		}

		const Vec3 operator/(const Vec3& other)
		{
			return { x / other.x, y / other.y, z / other.z };
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

		const Vec3 operator+(const T other)
		{
			return { x + other, y + other, z + other };
		}

		const Vec3 operator-(const T other)
		{
			return { x - other, y - other, z - other };
		}

		const Vec3 operator*(const T other)
		{
			return { x * other, y * other, z * other };
		}

		const Vec3 operator/(const T other)
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
	};

	template <typename T>
	struct Vec4
	{
		T x, y, z, w;

		const Vec4 operator+(const Vec4& other)
		{
			return { x + other.x, y + other.y, z + other.z, w + other.w };
		}

		const Vec4 operator-(const Vec4& other)
		{
			return { x - other.x, y - other.y, z - other.z, w - other.w };
		}

		const Vec4 operator*(const Vec4& other)
		{
			return { x * other.x, y * other.y, z * other.z, w * other.w };
		}

		const Vec4 operator/(const Vec4& other)
		{
			return { x / other.x, y / other.y, z / other.z, w / other.w };
		}

		Vec4& operator+=(const Vec4& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w
				return *this;
		}

		Vec4& operator-=(const Vec4& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w
				return *this;
		}

		Vec4& operator*=(const Vec4& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w
				return *this;
		}

		Vec4& operator/=(const Vec4& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w
				return *this;
		}

		const Vec4 operator+(const T other)
		{
			return { x + other, y + other, z + other, w + other };
		}

		const Vec4 operator-(const T other)
		{
			return { x - other, y - other, z - other, w - other };
		}

		const Vec4 operator*(const T other)
		{
			return { x * other, y * other, z * other, w * other };
		}

		const Vec4 operator/(const T other)
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
	};

}

#endif // !VECTORS_HPP