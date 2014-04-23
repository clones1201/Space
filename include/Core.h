#ifndef __SPACE_CORE_H__
#define __SPACE_CORE_H__

#include "basic.h"
#include "SceneManager.h"

namespace space {
	using namespace graphic;

	//typedef vector<IRenderSystem*> RenderSystemList;
	typedef map<string, SceneManager> SceneManagerMap;
	
	class Core : public Pattern::Singleton<Core>{
		friend class RenderSystem;
	protected:
		//RenderSystemList mRenderSystemList;
		SceneManagerMap mSceneManagerMap;
	public:
		void StartToRendering(void);

		void RenderOneFrame(void);

		void RenderOneFrame(float timeSinceLastFrame);

		void ShutDown(void);
		// maintain multiple rendersystem is not what i should do at the beginning
		//void AddRenderSystem(IRenderSystem *newRend);
		//const RenderSystemList& GetAvailableRenderers(void);
		//IRenderSystem* GetRenderSystemByName(const string& name);

		//Set the RenderSystem to be used
		void SetRenderSystem(RenderSystem* system);

		RenderSystem* GetRenderSystem(void);

		RenderSystem* Initialize(bool autoCreteWindow, const String& windowTitle = L"Space Render Window");

		RenderWindow* CreateRenderWindow();

		SceneManager* GetSceneManager(const String& name);

		SceneManager* CreateSceneManager(const String& name);

		void DestorySceneManager(SceneManager* sm);

		Core();
		~Core();
	private:

		RenderSystem* mActiveRenderSystem;
	};
}

#endif