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

		using Vec3<float>::Vec3;
		using Vec3<float>::operator=;

		Color3& operator=(const Color3& b)
		{
			R = b.R;
			G = b.G;
			B = b.B;
			return *this;
		}

		Color3(const Color3& b)
			: R(x), G(y), B(z)
		{
			R = b.R;
			G = b.G;
			B = b.B;
		}

		using Vec3<float>::operator+;
		using Vec3<float>::operator-;
		using Vec3<float>::operator*;
		using Vec3<float>::operator/;

		using Vec3<float>::operator+=;
		using Vec3<float>::operator-=;
		using Vec3<float>::operator*=;
		using Vec3<float>::operator/=;

		using Vec3<float>::operator==;
		using Vec3<float>::operator!=;

		/// <summary>
		/// Loads a color in the format RGB from a hexadecimal encoded color
		/// </summary>
		/// <param name="hex_code">A hexadecimal encoded color in the format #rrggbb</param>
		Color3(std::string hex_code)
		{
			HXG_ASSERT(hex_code.size() == 7,
				HXG_LOG_ERROR("A color3 hexcode must be in the format \"#rrggbb\""); return;);

			std::string rgb_str[3] = { {hex_code[1], hex_code[2]}, {hex_code[3], hex_code[4]}, {hex_code[5], hex_code[6]} };

			R = static_cast<float>(std::stoi(rgb_str[0], nullptr, 16)) / 255;
			G = static_cast<float>(std::stoi(rgb_str[1], nullptr, 16)) / 255;
			B = static_cast<float>(std::stoi(rgb_str[2], nullptr, 16)) / 255;
		}

		bool is_close_to(const Color3& second, float sensi = 0.05f)
		{
			return
				close(R, second.R, sensi) &&
				close(G, second.G, sensi) &&
				close(B, second.B, sensi);
		}
		
		static Color3 red;
		static Color3 green;
		static Color3 blue;
		static Color3 cyan;
		static Color3 magenta;
		static Color3 yellow;
		static Color3 black;
		static Color3 white;
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

		using Vec4<float>::Vec4;
		using Vec4<float>::operator=;

		Color4& operator=(const Color4& b)
		{
			R = b.R;
			G = b.G;
			B = b.B;
			A = b.A;
			return *this;
		}

		Color4(const Color4& b)
			: R(x), G(y), B(z), A(w)
		{
			R = b.R;
			G = b.G;
			B = b.B;
			A = b.A;
		}

		using Vec4<float>::operator+;
		using Vec4<float>::operator-;
		using Vec4<float>::operator*;
		using Vec4<float>::operator/;

		using Vec4<float>::operator+=;
		using Vec4<float>::operator-=;
		using Vec4<float>::operator*=;
		using Vec4<float>::operator/=;

		using Vec4<float>::operator==;
		using Vec4<float>::operator!=;

		/// <summary>
		/// Loads a color in the format RGBA from a hexadecimal encoded color
		/// </summary>
		/// <param name="hex_code">A hexadecimal encoded color in the format #rrggbbaa</param>
		Color4(std::string hex_code)
		{
			HXG_ASSERT(hex_code.size() == 9,
				HXG_LOG_ERROR("A color3 hexcode must be in the format \"#rrggbbaa\""); return;);

			std::string rgb_str[4] = { {hex_code[1], hex_code[2]}, {hex_code[3], hex_code[4]}, {hex_code[5], hex_code[6]}, {hex_code[7], hex_code[8]} };

			R = static_cast<float>(std::stoi(rgb_str[0], nullptr, 16)) / 255;
			G = static_cast<float>(std::stoi(rgb_str[1], nullptr, 16)) / 255;
			B = static_cast<float>(std::stoi(rgb_str[2], nullptr, 16)) / 255;
			A = static_cast<float>(std::stoi(rgb_str[3], nullptr, 16)) / 255;
		}

		bool is_close_to(const Color4& second, float sensi = 0.05f)
		{
			return 
				close(R, second.R, sensi) &&
				close(G, second.G, sensi) &&
				close(B, second.B, sensi) &&
				close(A, second.A, sensi);
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