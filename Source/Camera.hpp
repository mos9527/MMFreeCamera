#include "Common.h"

struct Camera {
	Math::Vec3<float> position;
	Math::Vec3<float> focus;
	float rotation;
	float unk1;
	float unk2;
	float verticalFov;
	float horizontalFov;
	float unk3;
	float unk4;
	Math::Vec3<float> ident; // is it really?
};

class CameraWindow : BaseWindow {
private:
public:	
	Camera* camera;
	float rotH;
	float rotV;
	float normalSpeed = 0.01;
	float acceleratedSpeed = 0.05;
	bool overrideCamera = false;	
	int prevPressed = 0;
	void OnFrame();
	void OnImGui();
	void OnInit();
} inline cameraWindow;
