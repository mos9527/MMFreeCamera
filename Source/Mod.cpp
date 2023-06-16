#include "Common.h"

HOOK(void*, __fastcall, GetInputState, MegaMix::sigGetInputState(), int player) {
	void* result = originalGetInputState(player);
	if (result && (ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantCaptureMouse))
	{
		return originalGetInputState(1);
	}
	return result;
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer) {
		g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
		pBackBuffer->Release();
	}
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;
	switch (uMsg) {
	case WM_KEYDOWN:
		break;
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return CallWindowProc(originalWndProc, hWnd, uMsg, wParam, lParam);
}

#include "Camera.hpp"
#include "Renderer.hpp"
extern "C"
{
	void __declspec(dllexport) D3DInit(IDXGISwapChain* swapChain,
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext) {
		g_pSwapChain = swapChain;
		g_pd3dDevice = device;
		g_pd3dDeviceContext = deviceContext;

		DXGI_SWAP_CHAIN_DESC m_ChainDesc;
		swapChain->GetDesc(&m_ChainDesc);
		CreateRenderTarget();
		window = m_ChainDesc.OutputWindow;
		originalWndProc = (WNDPROC)SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);		
	}
	void __declspec(dllexport) Init() {
		INSTALL_HOOK(GetInputState);
		ImGui::CreateContext();
		cameraWindow.OnInit();
		rendererWindow.OnInit();
	}
	void __declspec(dllexport) OnFrame(IDXGISwapChain* m_pSwapChain) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::GetIO().MouseDrawCursor = !HideAll;
		ImGui::NewFrame();
		if (!HideAll) {
			ImGui::Begin("Help");
			ImGui::SeparatorText("CONTROLS");
			ImGui::BulletText("Hotkeys");
			ImGui::Text("- Press both LS + RS to toggle Camera Control / Normal Gameplay");
			ImGui::Text("- Dpad Up + Press RS to toggle ImGui");
			ImGui::BulletText("Camera Controls");
			ImGui::Text("- [ Left  Stick ] Moves camera in viewing direction");
			ImGui::Text("                  Press LS to move faster");
			ImGui::Text("- [ Right Stick ] Pans camera viewport");
			ImGui::Text("                  Press RS to move faster");
			ImGui::Text("- [    LB/RB    ] Moves camera up/down");
			ImGui::Text("- [    LT/RT    ] Decreases/increases FOV (Zooms in/out)");
			ImGui::Text("- [  Dpad L/R   ] Rotates viewport counter-clockwise/clockwise");			
			ImGui::SeparatorText("ACKNOWLEDGEMENTS");
			ImGui::BulletText("Original TLAC Camera by samyuu");
			ImGui::TextColored(ImVec4(0, 0.5f, 1.0f, 1.0f), "github.com/samyuu/TotallyLegitArcadeController");
			ImGui::BulletText("Modified TLAC CameraController to work with MM+ by BroGamer4256");
			ImGui::TextColored(ImVec4(0, 0.5f, 1.0f, 1.0f), "github.com/BroGamer4256/camera");
			ImGui::SeparatorText("NOTES");
			ImGui::BulletText("[Camera]   Objects are known to flicker / disapper when not seen in game camera");
			// ...these are supposed to be TODOs
			ImGui::End();
		}
		cameraWindow.OnImGui();
		rendererWindow.OnImGui();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}