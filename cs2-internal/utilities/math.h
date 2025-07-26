#pragma once

#define rad2deg(x) DirectX::XMConvertToDegrees( x ) 
#define deg2rad(x) DirectX::XMConvertToRadians( x )
#define m_rad_pi 57.295779513082f
#define k_pi 3.14159265358979323846f

#include "../utilities/common.h"
// used: std::is_integral_v
#include <type_traits>
// used: sin, cos, pow, abs, sqrt
#include <corecrt_math.h>// used: MATH::Sin, cos, MATH::Pow, abs, sqrt
#include <corecrt_math.h>
// used: rand, srand
#include <cstdlib>
// used: time
#include <ctime>
#include "../imgui/imgui.h"
#include "../sdk/datatypes/matrix.h"
#include "../sdk/datatypes/vector.h"
#include "../sdk/datatypes/qangle.h"
// convert angle in degrees to radians
#define M_DEG2RAD(DEGREES) ((DEGREES) * (MATH::_PI / 180.f))
// convert angle in radians to degrees
#define M_RAD2DEG(RADIANS) ((RADIANS) * (180.f / MATH::_PI))
/// linearly interpolate the value between @a'X0' and @a'X1' by @a'FACTOR'
#define M_LERP(X0, X1, FACTOR) ((X0) + ((X1) - (X0)) * (FACTOR))
/// trigonometry
#define M_COS(ANGLE) cos(ANGLE)
#define M_SIN(ANGLE) sin(ANGLE)
#define M_TAN(ANGLE) tan(ANGLE)
/// power
#define M_POW(BASE, EXPONENT) pow(BASE, EXPONENT)
/// absolute value
#define M_ABS(VALUE) abs(VALUE)
/// square root
#define M_SQRT(VALUE) sqrt(VALUE)
/// floor
#define M_FLOOR(VALUE) floor(VALUE)

constexpr auto RadPi = 3.14159265358979323846;
constexpr auto DegPi = 180.0;
template<typename T>
T ToDegrees(T radians) {
	return (radians * (static_cast<T>(DegPi) / static_cast<T>(RadPi)));
}

namespace MATH
{
	inline constexpr float _PI = 3.141592654f;
	inline constexpr float _2PI = 6.283185307f;
	inline constexpr float _HPI = 1.570796327f;
	inline constexpr float _QPI = 0.785398163f;
	inline constexpr float _1DIV2PI = 0.159154943f;
	inline constexpr float _PHI = 1.618033988f;

	bool world_to_screen(const Vector_t& in, Vector_t& out, ImVec2 screen);
	bool WorldToScreen(const Vector_t& in, ImVec2& out);
	bool world_screen(Vector_t&, Vector_t&, ImVec2);
	bool Setup();

	void TransformAABB(const Matrix3x4a_t& transform, const Vector_t& minsIn, const Vector_t& maxsIn, Vector_t& minsOut, Vector_t& maxsOut);
	void angle_vector(const QAngle_t& angles, Vector_t& forward) noexcept;

	Vector_t angle_vector(const QAngle_t& angles) noexcept;

	void new_vector_angles(const Vector_t& forward, Vector_t& angles);
	void normalize_angles(Vector_t& angles);

	QAngle_t angle_normalize(QAngle_t& angle);

	float AngleNormalize(float angle);
	float AngleDiff(float next, float cur);
	float segment_dist(Vector_t start1, Vector_t end1, Vector_t start2, Vector_t end2) noexcept;

	void rotate_triangle(std::array<ImVec2, 3>& points, float rotation);

	template <typename T>
	[[nodiscard]] CS_INLINE constexpr const T& Min(const T& left, const T& right) noexcept
	{
		return (right < left) ? right : left;
	}

	inline float Q_sqrt(const float number) {
		const unsigned int i = (*(unsigned int*)&number + 0x3f800000) >> 1;
		const float approx = *(float*)&i;
		return (approx + number / approx) * 0.5f;
	}

	template <typename T>
	CS_INLINE T clamp(const T& n, const T& lower, const T& upper)
	{
		return std::max(lower, std::min(n, upper));
	}

	template <typename T>
	[[nodiscard]] CS_INLINE constexpr const T& Max(const T& left, const T& right) noexcept
	{
		return (right > left) ? right : left;
	}

	[[nodiscard]] CS_INLINE float radius2degr(float rad) {
		float result = rad * (180.0f / 3.14159265358f);
		return result;
	}

	[[nodiscard]] CS_INLINE float deg2radius(float deg) {
		float result = deg * (3.14159265358f / 180.0f);
		return result;
	}

	[[nodiscard]] CS_INLINE float normalize_yaw(float yaw) noexcept
	{
		while (yaw < -180.0f)
			yaw += 360.0f;

		while (yaw > 180.0f)
			yaw -= 360.0f;

		return yaw;
	}

	template <typename T>
	[[nodiscard]] CS_INLINE constexpr const T& Clamp(const T& value, const T& minimal, const T& maximal) noexcept
	{
		return (value < minimal) ? minimal : (value > maximal) ? maximal : value;
	}

	void  anglevectors(const QAngle_t& angles, Vector_t* forward, Vector_t* right = nullptr, Vector_t* up = nullptr);
	void super_fast_rsqrt(float a, float* out);

	float super_fast_vec_normalize(Vector_t& vec);

	void vector_angles_gomo(const Vector_t& forward, Vector_t& angles);
	void angle_vectors_second(const Vector_t& angles, Vector_t& forward);
	void new_angle_vectors(const Vector_t& angles, Vector_t* forward, Vector_t* right, Vector_t* up);

	Vector_t new_calculate_angle(const Vector_t& src, const Vector_t& dst);

	QAngle_t CalcAngles(Vector_t viewPos, Vector_t aimPos);

	__forceinline float delta_angle(float first, float second)
	{
		const float delta = first - second;
		float res = std::isfinite(delta) ? std::remainder(delta, 6.283f) : 0.0f;

		if (first > second)
		{
			if (res >= 3.1415f)
				res -= 6.283f;
		}
		else
		{
			if (res <= -3.1415f)
				res += 6.283f;
		}

		return res;
	};

	void angle_vectores(const QAngle_t& angles, QAngle_t& forward, QAngle_t& right, QAngle_t& up);

	void angle_vectors(const Vector_t& angles, Vector_t& forward);

	void VectorAngless(const Vector_t& forward, QAngle_t& angles, Vector_t* up = nullptr);

	Vector_t angle_vectors_fs(const Vector_t& angles);
	void angle_vectors_fs(Vector_t& angles, Vector_t& forward);

	[[nodiscard]] inline void vec_angles(Vector_t forward, Vector_t* angles)
	{
		float tmp, yaw, pitch;

		if (forward.y == 0.f && forward.x == 0.f) {
			yaw = 0;
			if (forward.z > 0) {
				pitch = 270;
			}
			else {
				pitch = 90.f;
			}
		}
		else {
			yaw = (float)(atan2(forward.y, forward.x) * 180.f / 3.14159265358979323846f);
			if (yaw < 0) {
				yaw += 360.f;
			}
			tmp = (float)sqrt(forward.x * forward.x + forward.y * forward.y);
			pitch = (float)(atan2(-forward.z, tmp) * 180.f / 3.14159265358979323846f);
			if (pitch < 0) {
				pitch += 360.f;
			}
		}
		angles->x = pitch;
		angles->y = yaw;
		angles->z = 0.f;
	}

	inline void sin_cos(float rad, float* sine, float* cosine) {
		*sine = std::sinf(rad);
		*cosine = std::cosf(rad);
	}

	inline void angle_vectors4(const Vector_t& angles, Vector_t* forward, Vector_t* right, Vector_t* up) {
		float sp, sy, sr, cp, cy, cr;

		sin_cos(angles.x * 0.017453292f, &sp, &cp);
		sin_cos(angles.y * 0.017453292f, &sy, &cy);
		sin_cos(angles.z * 0.017453292f, &sr, &cr);

		if (forward)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = sr * sp * cy - cr * sy;
			right->y = cr * cy + sr * sp * sy;
			right->z = sr * cp;
		}

		if (up)
		{
			up->x = sr * sy + cr * sp * cy;
			up->y = cr * sp * sy - sr * cy;
			up->z = cr * cp;
		}
	}

	inline void vector_angles23(const Vector_t& vecForward, Vector_t& vecAngles) {
		Vector_t vecView;
		if (vecForward.y == 0 && vecForward.x == 0)
		{
			vecView.x = 0.f;
			vecView.y = 0.f;
		}
		else
		{
			vecView.y = atan2(vecForward.y, vecForward.x) * 180.f / 3.14f;

			if (vecView.y < 0.f)
				vecView.y += 360.f;

			auto tmp = vecForward.Length2D();

			vecView.x = atan2(-vecForward.z, tmp) * 180.f / 3.14f;

			if (vecView.x < 0.f)
				vecView.x += 360.f;
		}

		vecAngles.x = vecView.x;
		vecAngles.y = vecView.y;
		vecAngles.z = 0.f;
	}

	template <typename T> requires (std::is_integral_v<T>)
		[[nodiscard]] CS_INLINE constexpr bool IsPowerOfTwo(const T value) noexcept
	{
		return value != 0 && (value & (value - 1)) == 0;
	}

	inline int(CS_CDECL* fnRandomSeed)(int iSeed) = nullptr;
	inline float(CS_CDECL* fnRandomFloat)(float flMinValue, float flMaxValue) = nullptr;
	inline float(CS_CDECL* fnRandomFloatExp)(float flMinValue, float flMaxValue, float flExponent) = nullptr;
	inline int(CS_CDECL* fnRandomInt)(int iMinValue, int iMaxValue) = nullptr;
	inline float(CS_CDECL* fnRandomGaussianFloat)(float flMean, float flStdDev) = nullptr;
}