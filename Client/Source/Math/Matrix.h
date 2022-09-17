#pragma once

#include<DirectXMath.h>

namespace CLIENT
{
	class Vector3;
	class Vector4;

	class Matrix final
	{
	private:
		DirectX::XMFLOAT4X4 mMatrix;
	public:
		Matrix()
			: mMatrix
			(
				1.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 1.f,
				0.f, 0.f, 1.f, 0.f,
				0.f, 0.f, 0.f, 1.f
			)
		{}
		Matrix(const Matrix& m) = default;
		Matrix(Matrix&& m)      = default;
	public:
		Matrix& operator = (const Matrix& m) = default;
		Matrix& operator = (Matrix&& m)      = default;
	public:
		inline Matrix operator * (const Matrix& m) const
		{
			Matrix mat;
			auto sim_result = DirectX::XMMatrixMultiply(GetSIMD(), m.GetSIMD());
			mat.SetSIMD(sim_result);
			return mat;
		}
	private:
		inline DirectX::XMMATRIX GetSIMD() const
		{
			return DirectX::XMLoadFloat4x4(&mMatrix);
		}
		inline void SetSIMD(DirectX::XMMATRIX sim)
		{
			DirectX::XMStoreFloat4x4(&mMatrix, sim);
		}
	};
}