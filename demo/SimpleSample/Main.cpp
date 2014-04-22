#include "Core.h"
#include "D3D11RenderSystem.h"
#include "D3D11RenderWindow.h"

int main(){
	HINSTANCE hInst = (HINSTANCE)GetModuleHandle(NULL);
	space::IRenderSystem* renderer = new space::D3D11RenderSystem(hInst);

	space::Core::Instance()->SetRenderSystem(renderer);
	
	space::Core::Instance()->Initialize(false);

	space::SceneManager* sceneMgr = space::Core::Instance()->CreateSceneManager(L"Sample SceneManager");

	space::IRenderWindow* win = space::Core::Instance()->CreateRenderWindow();

	space::Core::Instance()->StartToRendering();

	return 0;
}