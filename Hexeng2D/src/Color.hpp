#ifndef COLOR_HPP
#define COLOR_HPP

#include "Functions.hpp"
#include "Vectors.hpp"

namespace Hexeng
{

	class Color3 : public Vec3<float>
	{
	public :

		float& R = Vec3<float>::x;
		float& G = Vec3<float>::y;
		float& B = Vec3<float>::z;

		using Vec3<float>::operator=;

		bool operator==(const Color3& second)
		{
			return R == second.R && G == second.G && B == second.B;
		}

		bool is_close_to(const Color3& second, float sensi = 0.05f)
		{
			return close(R, second.R, sensi) && close(G, second.G, sensi) && close(B, second.B, sensi);
		}
	};

	class Color4 : public Vec4<float>
	{
	public :

		float& R = Vec4<float>::x;
		float& G = Vec4<float>::y;
		float& B = Vec4<float>::z;
		float& A = Vec4<float>::w;

		using Vec4<float>::operator*;

		bool operator==(const Color4& second)
		{
			return R == second.R && G == second.G && B == second.B && A == second.A;
		}

		bool is_close_to(const Color4& second, float sensi = 0.05f)
		{
			return close(R, second.R, sensi) && close(G, second.G, sensi) && close(B, second.B, sensi) && close(A, second.A, sensi);
		}
	};

}

#endif // !COLOR_HPP