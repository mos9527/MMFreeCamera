#include "Renderer.hpp"

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
HOOK(void*, __fastcall, HookPass1, MegaMix::sigrndpass_render_all_pass_execute_pre3d(), void* a1, RendererParams* a2) {
	if (MegaMix::pass3DSettings.enabled)
		return originalHookPass1(a1,a2);
	return 0;
}
HOOK(void*, __fastcall, HookPass2, MegaMix::sigrndpass_render_all_pass_execute_3d(), void* a1, RendererParams* a2) {
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