#pragma once

#include<DirectXMath.h>

namespace CLIENT
{
	class Vector2 final
	{
	public:
		float x, y;
	public:
		Vector2() : x(0), y(0) {}
		Vector2(float x, float y) : x(x), y(y) {}
		Vector2(const Vector2& v) = default;
		Vector2(Vector2&& v)      = default;
	public:
		Vector2& operator=(const Vector2& v) = default;
		Vector2& operator=(Vector2&& v)      = default;
	public:
		inline Vector2 operator + (const Vector2& v) const
		{
			return Vector2(x + v.x, y + v.y);
		}
		inline Vector2 operator - (const Vector2& v) const
		{
			return Vector2(x - v.x, y - v.y);
		}
		inline Vector2& operator += (const Vector2& v)
		{
			x += v.x; y += v.y; return *this;
		}
		inline Vector2& operator -= (const Vector2& v)
		{
			x -= v.x; y -= v.y; return *this;
		}
		inline Vector2 operator * (float k) const
		{
			return Vector2(x * k, y * k);
		}
		inline Vector2 operator / (float k) const
		{
			k = 1.0f / k; 
			return Vector2(x * k, y * k);
		}
		inline Vector2& operator *= (float k)
		{
			x *= k;	 y *= k;
			return *this;
		}
		inline Vector2& operator /= (float k)
		{
			k = 1.0f / k;
			x *= k; y *= k;
			return *this;
		}
		inline float& operator [] (int idx)
		{
			switch (idx)
			{
			case 0:
				return x;
			case 1:
				return y;
			default:
				return x;
			}
		}
	public:
		bool operator == (const Vector2& v)
		{
			return (x == v.x && y == v.y);
		}
		bool operator != (const Vector2& v)
		{
			return (x != v.x || y != v.y);
		}
	public:
		inline static float Length(const Vector2& v)
		{
			auto len = DirectX::XMVector2Length(GetSIMD(v));
			return DirectX::XMVectorGetX(len);
		}
		inline static Vector2 Normalize(const Vector2& v)
		{
			auto sim = DirectX::XMVector2Normalize(GetSIMD(v));
			return ConvertVector2(sim);
		}
		inline static float Dot(const Vector2& v1, const Vector2& v2)
		{
			auto sim = DirectX::XMVector2Dot(GetSIMD(v1), GetSIMD(v2));
			return DirectX::XMVectorGetX(sim);
		}
		inline static Vector2 Cross(const Vector2& v1, const Vector2& v2)
		{
			auto sim = DirectX::XMVector2Dot(GetSIMD(v1), GetSIMD(v2));
			return ConvertVector2(sim);
		}
	private:
		inline static DirectX::XMVECTOR GetSIMD(const Vector2& v)
		{
			DirectX::XMFLOAT2 v2(v.x, v.y);
			return DirectX::XMLoadFloat2(&v2);
		}
		inline static Vector2 ConvertVector2(DirectX::XMVECTOR v)
		{
			DirectX::XMFLOAT2 v2;
			DirectX::XMStoreFloat2(&v2, v);
			return Vector2(v2.x, v2.y);
		}
	};

	class Vector3 final
	{
	public:
		float x, y, z;
	public:
		Vector3() : x(0), y(0), z(0) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
		Vector3(const Vector3& v) = default;
		Vector3(Vector3&& v) = default;
	public:
		Vector3& operator=(const Vector3& v) = default;
		Vector3& operator=(Vector3&& v) = default;
	public:
		inline Vector3 operator + (const Vector3& v) const
		{
			return Vector3(x + v.x, y + v.y, z + v.z);
		}
		inline Vector3 operator - (const Vector3& v) const
		{
			return Vector3(x - v.x, y - v.y, z - v.z);
		}
		inline Vector3& operator += (const Vector3& v)
		{
			x += v.x; y += v.y; z += v.z; return *this;
		}
		inline Vector3& operator -= (const Vector3& v)
		{
			x -= v.x; y -= v.y; z -= v.z; return *this;
		}
		inline Vector3 operator * (float k) const
		{
			return Vector3(x * k, y * k, z * k);
		}
		inline Vector3 operator / (float k) const
		{
			k = 1.0f / k;
			return Vector3(x * k, y * k, z * k);
		}
		inline Vector3& operator *= (float k)
		{
			x *= k; y *= k; z *= k;
			return *this;
		}
		inline Vector3& operator /= (float k)
		{
			k = 1.0f / k;
			x *= k; y *= k; z *= k;
			return *this;
		}
		inline float& operator [] (int idx)
		{
			switch (idx)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				return x;
			}
		}
	public:
		bool operator == (const Vector3& v)
		{
			return (x == v.x && y == v.y && z == v.z);
		}
		bool operator != (const Vector3& v)
		{
			return (x != v.x || y != v.y || z == v.z);
		}
	public:
		inline static float Length(const Vector3& v)
		{
			auto len = DirectX::XMVector3Length(GetSIMD(v));
			return DirectX::XMVectorGetX(len);
		}
		inline static Vector3 Normalize(const Vector3& v)
		{
			auto sim = DirectX::XMVector3Normalize(GetSIMD(v));
			return ConvertVector3(sim);
		}
		inline static float Dot(const Vector3& v1, const Vector3& v2)
		{
			auto sim = DirectX::XMVector3Dot(GetSIMD(v1), GetSIMD(v2));
			return DirectX::XMVectorGetX(sim);
		}
		inline static Vector3 Cross(const Vector3& v1, const Vector3& v2)
		{
			auto sim = DirectX::XMVector3Dot(GetSIMD(v1), GetSIMD(v2));
			return ConvertVector3(sim);
		}
	private:
		inline static DirectX::XMVECTOR GetSIMD(const Vector3& v)
		{
			DirectX::XMFLOAT3 v3(v.x, v.y, v.z);
			return DirectX::XMLoadFloat3(&v3);
		}
		inline static Vector3 ConvertVector3(DirectX::XMVECTOR v)
		{
			DirectX::XMFLOAT3 v3;
			DirectX::XMStoreFloat3(&v3, v);
			return Vector3(v3.x, v3.y, v3.z);
		}
	};

	class Vector4 final
	{
	public:
		float x, y, z, w;
	public:
		Vector4() : x(0), y(0), z(0), w(0) {}
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		Vector4(const Vector4& v) = default;
		Vector4(Vector4&& v) = default;
	public:
		Vector4& operator=(const Vector4& v) = default;
		Vector4& operator=(Vector4&& v) = default;
	public:
		inline Vector4 operator + (const Vector4& v) const
		{
			return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
		}
		inline Vector4 operator - (const Vector4& v) const
		{
			return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
		}
		inline Vector4& operator += (const Vector4& v)
		{
			x += v.x; y += v.y; z += v.z; w += v.w; return *this;
		}
		inline Vector4& operator -= (const Vector4& v)
		{
			x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this;
		}
		inline Vector4 operator * (float k) const
		{
			return Vector4(x * k, y * k, z * k, w * k);
		}
		inline Vector4 operator / (float k) const
		{
			k = 1.0f / k;
			return Vector4(x * k, y * k, z * k, w * k);
		}
		inline Vector4& operator *= (float k)
		{
			x *= k; y *= k; z *= k; w *= k;
			return *this;
		}
		inline Vector4& operator /= (float k)
		{
			k = 1.0f / k;
			x *= k; y *= k; z *= k; w *= k;
			return *this;
		}
		inline float& operator [] (int idx)
		{
			switch (idx)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			default:
				return x;
			}
		}
	public:
		bool operator == (const Vector4& v)
		{
			return (x == v.x && y == v.y && z == v.z && w == v.w);
		}
		bool operator != (const Vector4& v)
		{
			return (x != v.x || y != v.y || z == v.z || w == v.w);
		}
	public:
		inline static float Length(const Vector4& v)
		{
			auto len = DirectX::XMVector4Length(GetSIMD(v));
			return DirectX::XMVectorGetX(len);
		}
		inline static Vector4 Normalize(const Vector4& v)
		{
			auto sim = DirectX::XMVector4Normalize(GetSIMD(v));
			return ConvertVector4(sim);
		}
		inline static float Dot(const Vector4& v1, const Vector4& v2)
		{
			auto sim = DirectX::XMVector4Dot(GetSIMD(v1), GetSIMD(v2));
			return DirectX::XMVectorGetX(sim);
		}
		inline static Vector4 Cross(const Vector4& v1, const Vector4& v2)
		{
			auto sim = DirectX::XMVector4Dot(GetSIMD(v1), GetSIMD(v2));
			return ConvertVector4(sim);
		}
	private:
		inline static DirectX::XMVECTOR GetSIMD(const Vector4& v)
		{
			DirectX::XMFLOAT4 v4(v.x, v.y, v.z, v.w);
			return DirectX::XMLoadFloat4(&v4);
		}
		inline static Vector4 ConvertVector4(DirectX::XMVECTOR v)
		{
			DirectX::XMFLOAT4 v4;
			DirectX::XMStoreFloat4(&v4, v);
			return Vector4(v4.x, v4.y, v4.y, v4.w);
		}
	};
}