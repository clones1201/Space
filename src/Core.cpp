#include "Core.h"
#include "Renderable.h"
#include "RenderTarget.h"
#include "RenderWindow.h"
#include "RenderSystem.h"

namespace space{
	using namespace graphic;

	Core::Core(){
		//should start many factory here
		//but right now we don't have any factory at hand

	}

	Core::~Core(){

	}

	void Core::StartToRendering(void){}

	void Core::RenderOneFrame(void){}

	void Core::RenderOneFrame(float timeSinceLastFrame){}

	void Core::ShutDown(void){}
	// maintain multiple rendersystem is not what i should do at the beginning
	//void AddRenderSystem(IRenderSystem *newRend);
	//const RenderSystemList& GetAvailableRenderers(void);
	//IRenderSystem* GetRenderSystemByName(const string& name);

	//Set the RenderSystem to be used
	void Core::SetRenderSystem(IRenderSystem* system){}

	IRenderSystem* Core::GetRenderSystem(void){
		return mActiveRenderSystem;
	}

	IRenderSystem* Core::Initialize(bool autoCreteWindow, const String& windowTitle){
		auto autoWin = mActiveRenderSystem->_Initialize(autoCreteWindow);
		return mActiveRenderSystem;
	}

	IRenderWindow* Core::CreateRenderWindow(){
		if (!mActiveRenderSystem){
			//no renderer
		}
		IRenderWindow* win = mActiveRenderSystem->_createRenderWindow(L"Space Window", 500, 500, false);

		return win;
	}

	SceneManager* Core::GetSceneManager(const String& name){
		return nullptr;
	}

	SceneManager* Core::CreateSceneManager(const String& name){
		return nullptr;
	}

	void Core::DestorySceneManager(SceneManager* sm){}

}