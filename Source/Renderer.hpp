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


class RendererWindow : BaseWindow {

public:
	void OnImGui();
	void OnInit();
} inline rendererWindow;
