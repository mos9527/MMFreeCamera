#include "Common.h"

struct RendererParams
{
	_BYTE gap0[24];
	_QWORD qword18;
	_BYTE gap20[16];
	_QWORD qword30;
	_BYTE gap38[3864];
	_BYTE byteF50;
};


namespace MegaMix {
	SIG_SCAN(
		sigRendererPasses,
		0x1404D87E0,
		"\x4C\x8B\xDC\x49\x89\x5B\x10\x49\x89\x73\x18\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x63\xDA\x48\x89\x44\x24\x00\x33\xD2\x48\x89\x54\x24\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x44\x24\x00\x48\x8D\x05\x00\x00\x00\x00\x49\x89\x43\x88\x48\x8D\x05\x00\x00\x00\x00\x49\x89\x43\x90\x48\x8D\x05\x00\x00\x00\x00\x49\x89\x43\x98\x48\x8D\x05\x00\x00\x00\x00\x49\x89\x43\xA0\x48\x8D\x05\x00\x00\x00\x00\x49\x89\x43\xA8\x48\x8D\x05\x00\x00\x00\x00\x49\x89\x43\xC8\x48\x8D\x05\x00\x00\x00\x00\x49\x89\x43\xD0\x48\x8D\x05\x00\x00\x00\x00\x49\x89\x43\xD8\x48\x8D\x05\x00\x00\x00\x00\x49\x89\x43\xE0",
		"xxxxxxxxxxxxxxx????xxx????xxxxxxx?xxxxxx?xxx????xxxx?xxx????xxxxxxx????xxxxxxx????xxxxxxx????xxxxxxx????xxxxxxx????xxxxxxx????xxxxxxx????xxxxxxx????xxxx"
	)
		SIG_SCAN(
			sigRendererApplyDof,
			0x1404A5C40,
			"\x48\x8B\xC4\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\xA8\x0F\x29\x78\x98\x44\x0F\x29\x90\x00\x00\x00\x00\x44\x0F\x29\x98\x00\x00\x00\x00\x44\x0F\x29\xA0\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x49\x8B\xD8\x4C\x89\x4C\x24\x00\x48\x8B\xF9\x48\x89\x5C\x24\x00\x49\x8B\xC9\x48\x8B\xEA\xE8\x00\x00\x00\x00\x44\x0F\x28\xD0\xE8\x00\x00\x00\x00\x44\x0F\x28\xD8\xE8\x00\x00\x00\x00\x48\x8B\x87\x00\x00\x00\x00\x48\x8D\xB7\x00\x00\x00\x00\x4C\x8B\x7F\x30\x44\x0F\x28\xE0\x4C\x8B\x6F\x50\x49\x83\xC7\x28\x8B\x97\x00\x00\x00\x00\x49\x83\xC5\x28\xF3\x0F\x10\xB8\x00\x00\x00\x00\x8B\xCA\x0F\x28\xCF\xE8\x00\x00\x00\x00",
			"xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxx????xxxx????xxxx????xxx????xxxxxxx????xxxxxxx?xxxxxxx?xxxxxxx????xxxxx????xxxxx????xxx????xxx????xxxxxxxxxxxxxxxxxx????xxxxxxxx????xxxxxx????"
		)
		SIG_SCAN(
			sigRendererFinalizePass,
			0x1402C0590,
			"\x48\x8B\x01\x48\x8B\x08\x48\x8B\x51\x10\x48\x8D\x42\x04\x48\x39\x41\x08\x73\x09\x33\xC0\xC7\x00\x00\x00\x00\x00\xC3",
			"xxxxxxxxxxxxxxxxxxxxxxxx????x"
		)
		SIG_SCAN(
			sigrndpass_render_all_pass_execute_pre3d,
			0x1404D84F0,
			"\x40\x53\x48\x83\xEC\x60\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x8B\x11\x48\x8B\xD9\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\xC8\xE8\x00\x00\x00\x00\x83\xF8\x02\x0F\x84\x00\x00\x00\x00",
			"xxxxxxxxx????xxxxxxx?xxxxxxxxx?x????xxx????xxxx?x????x????xxxx????xxxxx????"
		)
		SIG_SCAN(
			sigrndpass_render_all_pass_execute_3d,
			0x1404D8630,
			"\x40\x53\x48\x83\xEC\x60\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x8B\x11\x48\x8B\xD9\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\xC8\xE8\x00\x00\x00\x00\x83\xF8\x02\x74\x5A\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x8B\x05\x00\x00\x00\x00",
			"xxxxxxxxx????xxxxxxx?xxxxxxxxx?x????xxx????xxxx?x????x????xxxx????xxxxxxxxx?x????xx????"
		)
		inline FUNCTION_PTR(void*, __fastcall, FinalizePass, sigRendererFinalizePassAddr, void* a1);
		inline FUNCTION_PTR(void*, __fastcall, ShadowPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x13), void* a1, RendererParams* a2);
		inline FUNCTION_PTR(void*, __fastcall, SSSPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x29), void* a1, RendererParams* a2);
		inline FUNCTION_PTR(void*, __fastcall, ReflectionPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x35), void* a1, RendererParams* a2);
		inline FUNCTION_PTR(void*, __fastcall, RefractionPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x40), void* a1, RendererParams* a2);
		inline FUNCTION_PTR(void*, __fastcall, PreProcessPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x4B), void* a1, RendererParams* a2);
		inline FUNCTION_PTR(void*, __fastcall, ClearPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x56), void* a1, RendererParams* a2);
		inline FUNCTION_PTR(void*, __fastcall, SpritePass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x61), void* a1, RendererParams* a2);
		inline FUNCTION_PTR(void*, __fastcall, VectorPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x6C), void* a1, RendererParams* a2);
		inline FUNCTION_PTR(void*, __fastcall, PostPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x77), void* a1, RendererParams* a2);
		inline FUNCTION_PTR(void*, __fastcall, SpritePass2, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x82), void* a1, RendererParams* a2);
		inline FUNCTION_PTR(void*, __fastcall, ScreenResizePass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x8D), void* a1, RendererParams* a2);

	struct ShadowPassSettings {
		bool enabled = true;
	} inline shadowPassSettings;

	struct DOFSettings {
		bool enabled = false;
	} inline dofSettings;

	struct SSSSettings {
		bool enabled = true;
	} inline sssSettings;

	struct Pass3DSettings {
		bool enabled = true;
	} inline pass3DSettings;

	struct Pass2DSettings {
		bool* enabled = (bool*)(readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0xDD) + 8 + 6);
	} inline pass2DSettings;
}

class RendererWindow : BaseWindow {

public:
	void OnImGui();
	void OnInit();
} inline rendererWindow;
