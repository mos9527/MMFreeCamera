#pragma once
#pragma warning( disable : 4244 4305 )
#include <cstdint>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// ImGui
#include <imgui.h>
#include <imgui/backends/imgui_impl_dx11.h>
#include <imgui/backends/imgui_impl_win32.h>
// Helpers
#include "Helpers.h"
#include "Logging.h"
#include "Sigscan.h"
// DirectX
#include <d3d11.h>
// Detours
#include <detours.h>
// STL
#include <algorithm>
// Data
inline ID3D11Device* g_pd3dDevice;
inline ID3D11DeviceContext* g_pd3dDeviceContext;
inline IDXGISwapChain* g_pSwapChain;
inline ID3D11RenderTargetView* g_mainRenderTargetView;
inline WNDPROC originalWndProc;
inline HWND window;

// Types & Base Classes
// IDA stuff
typedef uint8_t _BYTE;
typedef uint64_t _QWORD;
typedef uint32_t _DWORD;
typedef uint64_t _OWORD;
// Base Classes
class BaseWindow {
public:
	BaseWindow() {

	};
	virtual void OnInit() {

	};
	virtual void OnImGui() {
	
	};	
};
#define PI 3.14159265359
#define EXPAND2(vec2) vec2.x,vec2.y
#define EXPAND3(vec3) vec3.x,vec3.y,vec3.z
namespace Math {
	inline float Wrap(float& val,float min,float max) {
		if (val > max) val = min;
		if (val < min) val = max;
		return val;
	}
	inline bool InRange(float value, float min, float max) {
		return value == std::clamp(value, min, max);
	}
	constexpr float Angle2Rad(float angle) {
		return PI * angle / 180.0f;
	}
	constexpr float Rad2Angle(float rad) {
		return rad * 180.0f / PI;
	}
	template <class T>
	struct Vec2 {
		T x;
		T y;
		Vec2(T x0, T y0) {
			x = x0; y = y0;
		}
		float norm() {
			return sqrtf(pow(x, 2) + pow(y, 2));
		}
		float rad() {
			return atan2(this->y, this->x);
		}
		static Vec2<float> fromRad(float rad) {
			return Vec2(cos(rad), sin(rad));
		}
		Vec2<T> normal() {
			auto n = norm();
			return Vec2<T>(x / n, y / n);
		}
		Vec2<T>& operator+= (const Vec2<T>& rhs) {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
		Vec2<T>& operator* (float scalar) {
			x *= scalar;
			y *= scalar;
			return *this;
		}
		Vec2<T> operator- (const Vec2<T>& rhs) {
			return Vec2<T>(this->x - rhs.x, this->y - rhs.y);
		}
	};
	template <class T>
	struct Vec3 {
		T x;
		T y;
		T z;
		Vec3(T x0, T y0, T z0) {
			x = x0; y = y0; z = z0;
		}
		float norm() {
			return sqrtf(pow(x, 2) + pow(y, 2) + pow(z, 2));
		}
		Vec3<T> normal() {
			auto n = norm();
			return Vec3<T>(x / n, y / n, z / n);
		}
		Vec3<T>& operator+= (const Vec2<T>& rhs) {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
		Vec3<T>& operator+= (const Vec3<T>& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		Vec3<T>& operator* (float scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}
		Vec3<T> operator- (const Vec3<T>& rhs) {
			return Vec3<T>(this->x - rhs.x, this->y - rhs.y,this->z-rhs.z);
		}
		Vec3<T> operator+ (const Vec3<T>& rhs) {
			return Vec3<T>(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
		}
		Vec3<T> Cross(const Vec3<T>& rhs) {
			return Vec3<T>(
				(this->y * rhs.z - this->z * rhs.y),
				-(this->x * rhs.z - this->z * rhs.x),
				(this->x * rhs.y - this->y * rhs.x)
			);
		}
	};
}
namespace MegaMix {
	// Structs
	struct DivaInputState
	{
		enum Button {
			LT = 0x2000,
			LB = 0x800,
			LS = 0x8000,
			RT = 0x4000,
			RB = 0x1000,
			RS = 0x10000,
			U = 0x8, // U+D, L+R are impossible with this scancode
			D = 0x10,
			L = 0x20,
			R = 0x40,
			Y= 0x100,
			A= 0x400,
			X= 0x80,
			B= 0x200
		};
		char pad1[48];
		DWORD pressed;		
		char pad2[204];
		DWORD mouseX;
		DWORD mouseY;
		char pad3[40];
		int lX;
		int lY;
		int rX;
		int rY;
		Math::Vec2<DWORD> GetMouseAbsCursor() {
			return Math::Vec2(mouseX, mouseY);
		}
		Math::Vec2<float> GetLeftJoystick() {
			return Math::Vec2(lX / 1000.0f, -lY / 1000.0f);
		}
		Math::Vec2<float> GetRightJoystick() {
			return Math::Vec2(rX / 1000.0f, -rY / 1000.0f);
		}
		bool IsPressed(Button button) {
			return (int)button & pressed;
		}
	};
	SIG_SCAN
	(
		sigGetInputState,
		0x1402AC960,
		"\x4C\x8B\x05\x00\x00\x00\x00\x4D\x85\xC0\x74\x35\x4C\x63\xC9\x48\xB8\x00\x00\x00\x00\x00\x00\x00\x00\x49\x8B\x48\x08\x49\x2B\x08\x48\xF7\xE9",
		"xxx????xxxxxxxxxx????????xxxxxxxxxx"
	)
	inline FUNCTION_PTR(DivaInputState*, __fastcall, GetInputState, sigGetInputStateAddr, int player);
}

inline bool HideAll = false;