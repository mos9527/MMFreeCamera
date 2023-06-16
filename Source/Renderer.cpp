#include "Renderer.hpp"

namespace MegaMix {
	SIG_SCAN(
		sigRendererPasses,
		0x1404D87A0,
		"\x4C\x8B\xDC\x49\x89\x5B\x10\x49\x89\x73\x18\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x63\xDA",
		"xxxxxxxxxxxxxxx????xxx????xxx"
	)
	SIG_SCAN(
		sigRendererApplyDof,
		0x1404A5C00,
		"\x48\x8B\xC4\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\xA8\x0F\x29\x78\x98\x44\x0F\x29\x90\x00\x00\x00\x00",
		"xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxx????"
	)
	SIG_SCAN(
		sigRendererFinalizePass,
		0x1402C0570,
		"\x48\x8B\x01\x48\x8B\x08\x48\x8B\x51\x10\x48\x8D\x42\x04\x48\x39\x41\x08\x73\x09\x33\xC0\xC7\x00\x00\x00\x00\x00\xC3",
		"xxxxxxxxxxxxxxxxxxxxxxxx????x"
	)
	SIG_SCAN(
		sigRndPreProc,
		0x1404D84B0,
		"\x40\x53\x48\x83\xEC\x60\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x8B\x11\x48\x8B\xD9",
		"xxxxxxxxx????xxxxxxx?xxxxx"
	)
	SIG_SCAN(
		sigRndProc,
		0x1404D85F0,
		"\x40\x53\x48\x83\xEC\x60\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x8B\x11\x48\x8B\xD9\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00",
		"xxxxxxxxx????xxxxxxx?xxxxxxxxx?x????"
	)

	FUNCTION_PTR(void*, __fastcall, FinalizePass, sigRendererFinalizePassAddr, void *a1);	
	FUNCTION_PTR(void*, __fastcall, ShadowPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x13), void* a1, RendererParams* a2);
	FUNCTION_PTR(void*, __fastcall, SSSPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x29), void* a1, RendererParams* a2);
	FUNCTION_PTR(void*, __fastcall, ReflectionPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x35), void* a1, RendererParams* a2);
	FUNCTION_PTR(void*, __fastcall, RefractionPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x40), void* a1, RendererParams* a2);
	FUNCTION_PTR(void*, __fastcall, PreProcessPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x4B), void* a1, RendererParams* a2);
	FUNCTION_PTR(void*, __fastcall, ClearPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x56), void* a1, RendererParams* a2);
	FUNCTION_PTR(void*, __fastcall, SpritePass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x61), void* a1, RendererParams* a2);
	FUNCTION_PTR(void*, __fastcall, VectorPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x6C), void* a1, RendererParams* a2);
	FUNCTION_PTR(void*, __fastcall, PostPass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x77), void* a1, RendererParams* a2);
	FUNCTION_PTR(void*, __fastcall, SpritePass2, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x82), void* a1, RendererParams* a2);
	FUNCTION_PTR(void*, __fastcall, ScreenResizePass, readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0x8D), void* a1, RendererParams* a2);

	struct ShadowPassSettings {
		bool enabled = true;
	} shadowPassSettings;

	struct DOFSettings {
		bool enabled = false;
	} dofSettings;

	struct SSSSettings {
		bool enabled = true;
	} sssSettings;

	struct Pass3DSettings {
		bool enabled = true;
	} pass3DSettings;

	struct Pass2DSettings {
		bool* enabled = (bool*)(readLongLEAAddress((uint64_t)sigRendererPassesAddr + 0xDD) + 8 + 6);
	} pass2DSettings;
}

HOOK(void*, __fastcall, HookShadowPass, MegaMix::ShadowPass, void* a1, RendererParams* a2) {
	a2->byteF50 = MegaMix::shadowPassSettings.enabled;
	return originalHookShadowPass(a1, a2);
}
HOOK(void*, __fastcall, HookRendererDOF, MegaMix::sigRendererApplyDof(), void* a1, void* a2, void* a3, void* a4) {
	if (MegaMix::dofSettings.enabled) {
		return originalHookRendererDOF(a1, a2, a3, a4);
	}
	return 0;
}
HOOK(void*, __fastcall, HookSSSPass, MegaMix::SSSPass, void* a1, RendererParams* a2) {
	if (MegaMix::sssSettings.enabled) {
		return originalHookSSSPass(a1, a2);
	}
	return MegaMix::FinalizePass(a1);
}
HOOK(void*, __fastcall, HookPass1, MegaMix::sigRndPreProc(), void* a1, RendererParams* a2) {
	if (MegaMix::pass3DSettings.enabled)
		return originalHookPass1(a1,a2);
	return 0;
}
HOOK(void*, __fastcall, HookPass2, MegaMix::sigRndProc(), void* a1, RendererParams* a2) {
	if (MegaMix::pass3DSettings.enabled)
		return originalHookPass2(a1, a2);
	return 0;
}

void RendererWindow::OnInit() {
	INSTALL_HOOK(HookShadowPass);
	INSTALL_HOOK(HookRendererDOF);
	INSTALL_HOOK(HookSSSPass);
	INSTALL_HOOK(HookPass1);
	INSTALL_HOOK(HookPass2);
}

void RendererWindow::OnImGui() {
	if (HideAll) return;
	ImGui::Begin("Renderer");
	ImGui::SeparatorText("SHADOW");
	ImGui::Checkbox("SHADOWS ENABLED", &MegaMix::shadowPassSettings.enabled);
	ImGui::SeparatorText("DOF");
	ImGui::Checkbox("DOF ENABLED", &MegaMix::dofSettings.enabled);
	ImGui::SeparatorText("SSS");
	ImGui::Checkbox("SSS ENABLED", &MegaMix::sssSettings.enabled);
	ImGui::SeparatorText("3D");
	ImGui::Checkbox("3D ENABLED", &MegaMix::pass3DSettings.enabled);
	ImGui::SeparatorText("SPRITES");
	ImGui::Checkbox("SPRITES ENABLED", MegaMix::pass2DSettings.enabled);
	ImGui::End();
}