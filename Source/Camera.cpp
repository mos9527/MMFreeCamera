#include "Camera.hpp"

namespace MegaMix {
	SIG_SCAN(
		sigInitCameraPPMaybe,
		0x1404D7BB0,
		"\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00",
		"xxxx?xxxx?xxxx????xxx????xxxxxxx????"
	)
		SIG_SCAN(
			sigUpdateCamera,
			0x1402FA9F0,
			"\x48\x8B\xC4\x55\x48\x8D\x68\xE8\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\xE8\x0F\x29\x78\xD8\x44\x0F\x29\x40\x00\x44\x0F\x29\x48\x00",
			"xxxxxxxxxxx????xxxxxxxxxxxx?xxxx?"
		);
	FUNCTION_PTR(Camera*, __fastcall, GetCamera, readRelCall16Address((uint64_t)sigInitCameraPPMaybeAddr + 0x58));
}

// These functions would be otherwise too small to be reliably sigscanned
#define HOOK_UPDATE(offset,value) HOOK(void, __fastcall, cameraUpdate##offset, readRelCall16Address((uint64_t)MegaMix::sigUpdateCameraAddr + offset), Camera* camera,value)

HOOK_UPDATE(0xA7, Math::Vec3<float> value) {
	// position
	if (!cameraWindow.overrideCamera)
		originalcameraUpdate0xA7(camera, value);
}
HOOK_UPDATE(0xCE, Math::Vec3<float> value) {
	// focus
	if (!cameraWindow.overrideCamera)
		return originalcameraUpdate0xCE(camera, value);
}
HOOK_UPDATE(0x103, Math::Vec3<float> value) {
	// rotation
	if (!cameraWindow.overrideCamera)
		return originalcameraUpdate0x103(camera, value);
}
HOOK_UPDATE(0x126, float value) {
	// vertical fov
	if (!cameraWindow.overrideCamera)
		return originalcameraUpdate0x126(camera, value);
}
HOOK_UPDATE(0x136, float value) {
	// horizontal fov
	if (!cameraWindow.overrideCamera)
		return originalcameraUpdate0x136(camera, value);
}

void CameraWindow::OnFrame() {
	MegaMix::DivaInputState* input = MegaMix::GetInputState(0);
	auto directionH = Math::Vec2(camera->focus.x, camera->focus.z) - Math::Vec2(camera->position.x, camera->position.z);
	auto directionV = Math::Vec2(camera->focus.y, camera->focus.z) - Math::Vec2(camera->position.y, camera->position.z);
	if (overrideCamera) {
		auto LS = input->GetLeftJoystick();
		auto RS = input->GetRightJoystick();
		// Position
		auto speed = !input->IsPressed(MegaMix::DivaInputState::LS) ? normalSpeed : acceleratedSpeed;
		speed *= LS.norm();
		if (speed > 0) {
			auto direction = camera->focus - camera->position;
			auto offset = direction.normal() * speed * LS.y;
			camera->position += offset;
			direction = camera->ident;
			offset = direction.normal() * speed * LS.x;
			camera->position += offset;
		}
		if (input->IsPressed(MegaMix::DivaInputState::LB))
			camera->position.y -= normalSpeed;
		if (input->IsPressed(MegaMix::DivaInputState::RB))
			camera->position.y += normalSpeed;
		// Focus
		speed = !input->IsPressed(MegaMix::DivaInputState::RS) ? normalSpeed : acceleratedSpeed;
		rotV += RS.y * speed;
		rotH += RS.x * speed;
		Math::Vec2 focus = Math::Vec2<float>::fromRad(rotH);
		camera->focus.x = camera->position.x + focus.x;
		camera->focus.z = camera->position.z + focus.y;
		camera->focus.y = camera->position.y + Math::Vec2<float>::fromRad(rotV).x;
		// FOV
		if (input->IsPressed(MegaMix::DivaInputState::LT))
			camera->verticalFov += normalSpeed;
		if (input->IsPressed(MegaMix::DivaInputState::RT))
			camera->verticalFov -= normalSpeed;
		// Rotation
		if (input->IsPressed(MegaMix::DivaInputState::L))
			camera->rotation += normalSpeed;
		if (input->IsPressed(MegaMix::DivaInputState::R))
			camera->rotation -= normalSpeed;
		// Clamping
		camera->verticalFov = std::clamp(camera->verticalFov, 0.0f, Math::Angle2Rad(90));
	}
	else {
		rotV = directionV.rad();
		rotH = directionH.rad();
	}
	// Hotkeys
	auto pressed = input->pressed;
#define KEYDOWN(key) if (!((prevPressed & (key)) == (key)) && ((pressed & (key)) == (key)))
#define KEYUP(key) if (((prevPressed & (key)) == (key)) && !((pressed & (key)) == (key)))
	KEYDOWN(MegaMix::DivaInputState::LS | MegaMix::DivaInputState::RS) {
		overrideCamera = !overrideCamera;
	}
	KEYDOWN(MegaMix::DivaInputState::RS | MegaMix::DivaInputState::U) {
		HideAll = !HideAll;
	}
	prevPressed = pressed;
}

#define INSTALL_HOOK_UPDATE(v) INSTALL_HOOK(cameraUpdate##v);
void CameraWindow::OnInit() {
	camera = MegaMix::GetCamera();
	INSTALL_HOOK_UPDATE(0xA7)
		INSTALL_HOOK_UPDATE(0xCE)
		INSTALL_HOOK_UPDATE(0x103)
		INSTALL_HOOK_UPDATE(0x126)
		INSTALL_HOOK_UPDATE(0x136)
}

void CameraWindow::OnImGui() {
	OnFrame();
	if (HideAll) return;
	MegaMix::DivaInputState* input = MegaMix::GetInputState(0);
	ImGui::Begin("Inputs");
	// I shouldn't have done this
#define P ImGui::Button("##",ImVec2(32,16));ImGui::SameLine();
#define K(v) ImGui::Button(input->IsPressed(MegaMix::DivaInputState::v) ? #v : "##",ImVec2(32,16));ImGui::SameLine();
	K(LB); K(LT); P;    P;  P;    K(RT); K(RB); ImGui::NewLine();
	P;    K(U);  P;    P;  P;    K(Y);  P;  ImGui::NewLine();
	K(L); K(LS); K(R); P;  K(X); K(RS); K(B); ImGui::NewLine();
	P;    K(D);  P;    P;  P;    K(A);  P;  ImGui::NewLine();
	ImGui::Text("LS=(%.3f,%.3f)", EXPAND2(input->GetLeftJoystick()));
	ImGui::Text("RS=(%.3f,%.3f)", EXPAND2(input->GetRightJoystick()));
	ImGui::End();
	ImGui::Begin("Camera");
	ImGui::Text("Camera Stats");
	ImGui::Text(
		"pos=(%.3f,%.3f,%.3f) focus=(%.3f,%.3f,%.3f) ident=(%.3f,%.3f,%.3f) rotation=%.3f",
		EXPAND3(camera->position),
		EXPAND3(camera->focus),
		EXPAND3(camera->ident),
		camera->rotation
	);
	ImGui::Text("rotH=%.3f rotV=%.3f", rotH, rotV);
	ImGui::SeparatorText("CAM SETTINGS");
	ImGui::Checkbox("OVERRIDE", &overrideCamera);
	ImGui::SliderAngle("Horizontal FOV", &camera->horizontalFov, 0, 90);
	ImGui::SliderAngle("Vertical   FOV", &camera->verticalFov, 0, 90);
#define SLIDER(v) ImGui::SliderFloat(#v, &camera->v, -5, 5);
	SLIDER(position.x);
	SLIDER(position.y);
	SLIDER(position.z);
	SLIDER(rotation);
	ImGui::SliderAngle("Horizontal  Rotation", &rotH, 0, 360);
	ImGui::SliderAngle("Vertical    Rotation", &rotV, 0, 180);
	ImGui::SliderFloat("Normal      Speed", &normalSpeed, 0, 0.1);
	ImGui::SliderFloat("Accelerated Speed", &acceleratedSpeed, 0, 0.1);
	ImGui::End();
}
