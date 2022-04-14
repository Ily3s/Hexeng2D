#ifndef COLOR_HPP
#define COLOR_HPP

#include <cassert>

#include "Vectors.hpp"

namespace Hexeng
{

	inline bool close(float first, float second, float sensi)
	{
		return second - first > -sensi && second - first < sensi;
	}

	class Color3 : public Vec3<float>
	{
	public:

		float& R = Vec3<float>::x;
		float& G = Vec3<float>::y;
		float& B = Vec3<float>::z;

		Color3(float R, float G, float B) : Vec3<float>(R, G, B) {}
		Color3(const Color3& other) : Vec3<float>(other) {}
		Color3& operator=(const Color3& other) { R = other.R; G = other.G; B = other.B; return *this; }

		Color3(std::string hex_code)
		{
			assert(hex_code.size() == 7 && "A color3 hexcode must be in the format \"#xxxxxx\"");

			std::string rgb_str[3] = { {hex_code[1], hex_code[2]}, {hex_code[3], hex_code[4]}, {hex_code[5], hex_code[6]} };

			R = static_cast<float>(std::stoi(rgb_str[0], nullptr, 16)) / 255;
			G = static_cast<float>(std::stoi(rgb_str[1], nullptr, 16)) / 255;
			B = static_cast<float>(std::stoi(rgb_str[2], nullptr, 16)) / 255;
		}

		static Color3 red;
		static Color3 green;
		static Color3 blue;
		static Color3 cyan;
		static Color3 magenta;
		static Color3 yellow;
		static Color3 black;
		static Color3 white;

		Color3 operator+(const Color3& other) const
		{
			Vec3<float> output = *(Vec3<float>*) & other + *this;
			float max = std::max(std::max(output.x, output.y), output.z);
			if (max > 1)
				output /= max;
			return *(Color3*)&(output);
		}

		Color3 operator-(const Color3& other) const
		{
			Vec3<float> output = abs(*(Vec3<float>*) & other - *this);
			return *(Color3*)&(output);
		}

		Color3 operator*(const Color3& other) const
		{
			return *(Color3*)&(*(Vec3<float>*) & other * *(Vec3<float>*)this);
		}

		Color3 operator-() const
		{
			return Color3::white - *this;
		}

		Color3& operator+=(const Color3& other)
		{
			*this = *this + other;
			return *this;
		}

		Color3& operator-=(const Color3& other)
		{
			*this = *this - other;
			return *this;
		}

		Color3& operator*=(const Color3& other)
		{
			*this = *this * other;
			return *this;
		}

		bool operator==(const Color3& second)
		{
			return R == second.R && G == second.G && B == second.B;
		}

		bool is_close_to(const Color3& second, float sensi = 0.05f)
		{
			return close(R, second.R, sensi) && close(G, second.G, sensi) && close(B, second.B, sensi);
		}
	};

	inline Color3 Color3::red{ 1.0f, 0.0f, 0.0f };
	inline Color3 Color3::green{ 0.0f, 1.0f, 0.0f };
	inline Color3 Color3::blue{ 0.0f, 0.0f, 1.0f };
	inline Color3 Color3::cyan{ 0.0f, 1.0f, 1.0f };
	inline Color3 Color3::magenta{ 1.0f, 0.0f, 1.0f };
	inline Color3 Color3::yellow{ 1.0f, 1.0f, 0.0f };
	inline Color3 Color3::black{ 0.0f, 0.0f, 0.0f };
	inline Color3 Color3::white{ 1.0f, 1.0f, 1.0f };

	class Color4 : public Vec4<float>
	{
	public:

		float& R = Vec4<float>::x;
		float& G = Vec4<float>::y;
		float& B = Vec4<float>::z;
		float& A = Vec4<float>::w;

		Color4(float R, float G, float B, float A) : Vec4<float>(R, G, B, A) {}
		Color4(const Color4& other) : Vec4<float>(other) {}
		Color4& operator=(const Color4& other) { R = other.R; G = other.G; B = other.B; A = other.A; return *this; }

		Color4(std::string hex_code)
		{
			assert(hex_code.size() == 9 && "A color4 hexcode must be in the format \"#xxxxxxxx\"");

			std::string rgb_str[4] = { {hex_code[1], hex_code[2]}, {hex_code[3], hex_code[4]}, {hex_code[5], hex_code[6]}, {hex_code[7], hex_code[8]} };

			R = static_cast<float>(std::stoi(rgb_str[0], nullptr, 16)) / 255;
			G = static_cast<float>(std::stoi(rgb_str[1], nullptr, 16)) / 255;
			B = static_cast<float>(std::stoi(rgb_str[2], nullptr, 16)) / 255;
			A = static_cast<float>(std::stoi(rgb_str[3], nullptr, 16)) / 255;
		}

		// other is above
		Color4 operator+(const Color4& other) const
		{
			return *(Color4*)&(*(Vec4<float>*) & other * other.A + *(Vec4<float>*)this * (1 - other.A));
		}

		// other is above
		Color4 operator-(const Color4& other) const
		{
			Vec4<float> output = abs(*(Vec4<float>*) & other * other.A - *(Vec4<float>*)this * (1 - other.A));
			return *(Color4*)&(output);
		}

		// other is above
		Color4 operator*(const Color4& other) const
		{
			return *(Color4*)&(*(Vec4<float>*) & other * other.A * *(Vec4<float>*)this * (1 - other.A));
		}

		Color4 operator-() const
		{
			return Color4::white - *this;
		}

		Color4& operator+=(const Color4& other)
		{
			*this = *this + other;
			return *this;
		}

		Color4& operator-=(const Color4& other)
		{
			*this = *this - other;
			return *this;
		}

		Color4& operator*=(const Color4& other)
		{
			*this = *this * other;
			return *this;
		}

		bool operator==(const Color4& second)
		{
			return R == second.R && G == second.G && B == second.B && A == second.A;
		}

		bool is_close_to(const Color4& second, float sensi = 0.05f)
		{
			return close(R, second.R, sensi) && close(G, second.G, sensi) && close(B, second.B, sensi) && close(A, second.A, sensi);
		}

		static Color4 red;
		static Color4 green;
		static Color4 blue;
		static Color4 cyan;
		static Color4 magenta;
		static Color4 yellow;
		static Color4 black;
		static Color4 white;
	};

	inline Color4 Color4::red{ 1.0f, 0.0f, 0.0f, 1.0f };
	inline Color4 Color4::green{ 0.0f, 1.0f, 0.0f, 1.0f };
	inline Color4 Color4::blue{ 0.0f, 0.0f, 1.0f, 1.0f };
	inline Color4 Color4::cyan{ 0.0f, 1.0f, 1.0f, 1.0f };
	inline Color4 Color4::magenta{ 1.0f, 0.0f, 1.0f, 1.0f };
	inline Color4 Color4::yellow{ 1.0f, 1.0f, 0.0f, 1.0f };
	inline Color4 Color4::black{ 0.0f, 0.0f, 0.0f, 1.0f };
	inline Color4 Color4::white{ 1.0f, 1.0f, 1.0f, 1.0f };
}

#endif // !COLOR_HPP