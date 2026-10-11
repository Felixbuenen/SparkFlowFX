#pragma once

namespace sparkflow::core {

	struct vec3 {
		float x{}, y{}, z{};

		constexpr vec3& operator+=(const vec3& rhs) noexcept
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		constexpr vec3& operator-=(const vec3& rhs) noexcept
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		constexpr vec3& operator*=(float s) noexcept
		{
			x *= s;
			y *= s;
			z *= s;
			return *this;
		}

		constexpr vec3& operator/=(float s) noexcept
		{
			x /= s;
			y /= s;
			z /= s;
			return *this;
		}

		friend constexpr vec3 operator-(const vec3& v) noexcept { return { -v.x, -v.y, -v.z }; }

		friend constexpr vec3 operator+(vec3 lhs, const vec3& rhs) noexcept { return lhs += rhs; }
		friend constexpr vec3 operator-(vec3 lhs, const vec3& rhs) noexcept { return lhs -= rhs; }
		friend constexpr vec3 operator*(vec3 v, float s) noexcept { return v *= s; }
		friend constexpr vec3 operator*(float s, vec3 v) noexcept { return v *= s; }
		friend constexpr vec3 operator/(vec3 v, float s) noexcept { return v /= s; }

		friend constexpr bool operator==(const vec3&, const vec3&) noexcept = default;
	};

}