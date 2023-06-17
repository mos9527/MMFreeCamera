#include "Camera.hpp"
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
	auto directionH = Math::Vec2(MegaMix::camera->focus.x, MegaMix::camera->focus.z) - Math::Vec2(MegaMix::camera->position.x, MegaMix::camera->position.z);
	auto directionV = Math::Vec2(MegaMix::camera->focus.y, MegaMix::camera->focus.z) - Math::Vec2(MegaMix::camera->position.y, MegaMix::camera->position.z);
	if (overrideCamera) {
		auto LS = input->GetLeftJoystick();
		auto RS = input->GetRightJoystick();
		// Position
		auto speed = !input->IsPressed(MegaMix::DivaInputState::LS) ? normalSpeed : acceleratedSpeed;
		if (speed > 0) {
			auto direction = MegaMix::camera->focus - MegaMix::camera->position;
			auto offset = direction.normal() * speed * LS.y;
			MegaMix::camera->position += offset;
			direction = MegaMix::camera->ident;
			offset = direction.normal() * speed * LS.x;
			MegaMix::camera->position += offset;
		}
		if (input->IsPressed(MegaMix::DivaInputState::LB))
			MegaMix::camera->position.y -= speed;
		if (input->IsPressed(MegaMix::DivaInputState::RB))
			MegaMix::camera->position.y += speed;
		// FOV
		if (input->IsPressed(MegaMix::DivaInputState::LT))
			MegaMix::camera->verticalFov += normalSpeed;
		if (input->IsPressed(MegaMix::DivaInputState::RT))
			MegaMix::camera->verticalFov -= normalSpeed;
		// Focus
		speed = !input->IsPressed(MegaMix::DivaInputState::RS) ? normalSpeed : acceleratedSpeed;
		rotV += RS.y * speed;
		rotH += RS.x * speed;
		Math::Vec2 focus = Math::Vec2<float>::fromRad(rotH);
		MegaMix::camera->focus.x = MegaMix::camera->position.x + focus.x;
		MegaMix::camera->focus.z = MegaMix::camera->position.z + focus.y;
		MegaMix::camera->focus.y = MegaMix::camera->position.y + Math::Vec2<float>::fromRad(rotV).x;
		// Rotation
		if (input->IsPressed(MegaMix::DivaInputState::L))
			MegaMix::camera->rotation += normalSpeed;
		if (input->IsPressed(MegaMix::DivaInputState::R))
			MegaMix::camera->rotation -= normalSpeed;
		// Clamping
		MegaMix::camera->verticalFov = std::clamp(MegaMix::camera->verticalFov, 0.0f, Math::Angle2Rad(90));
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
		EXPAND3(MegaMix::camera->position),
		EXPAND3(MegaMix::camera->focus),
		EXPAND3(MegaMix::camera->ident),
		MegaMix::camera->rotation
	);
	ImGui::Text("rotH=%.3f rotV=%.3f", rotH, rotV);
	ImGui::SeparatorText("CAM SETTINGS");
	ImGui::Checkbox("OVERRIDE", &overrideCamera);
	ImGui::SliderAngle("Horizontal FOV", &MegaMix::camera->horizontalFov, 0, 90);
	ImGui::SliderAngle("Vertical   FOV", &MegaMix::camera->verticalFov, 0, 90);
#define SLIDER(v) ImGui::SliderFloat(#v, &MegaMix::camera->v, -5, 5);
	SLIDER(position.x);
	SLIDER(position.y);
	SLIDER(position.z);
	ImGui::SliderAngle("Rotation", &MegaMix::camera->rotation, -75, 75);
	ImGui::SliderAngle("Horizontal  Rotation", &rotH, 0, 360);
	ImGui::SliderAngle("Vertical    Rotation", &rotV, 0, 180);
	ImGui::SliderFloat("Normal      Speed", &normalSpeed, 0, 0.1);
	ImGui::SliderFloat("Accelerated Speed", &acceleratedSpeed, 0, 0.1);
	ImGui::End();
}

