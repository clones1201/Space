#ifndef __SPACE_CORE_H__
#define __SPACE_CORE_H__

#include "basic.h"
#include "SceneManager.h"

namespace space {
	using namespace graphic;

	//typedef vector<IRenderSystem*> RenderSystemList;
	typedef map<string, SceneManager> SceneManagerMap;
	
	class Core : public Pattern::Singleton<Core>{
		friend class IRenderSystem;
	protected:
		//RenderSystemList mRenderSystemList;
		SceneManagerMap mSceneManagerMap;
	public:
		void StartToRendering(void);

		bool RenderOneFrame(void);

		bool RenderOneFrame(float timeSinceLastFrame);

		void ShutDown(void);
		// maintain multiple rendersystem is not what i should do at the beginning
		//void AddRenderSystem(IRenderSystem *newRend);
		//const RenderSystemList& GetAvailableRenderers(void);
		//IRenderSystem* GetRenderSystemByName(const string& name);

		//Set the RenderSystem to be used
		void SetRenderSystem(IRenderSystem* system);

		IRenderSystem* GetRenderSystem(void);

		IRenderSystem* Initialize(bool autoCreteWindow, const string& windowTitle = "Space Render Window");

		IRenderSystem* CreateRenderWindow();

		SceneManager* GetSceneManager(const string& name);

		SceneManager* CreateSceneManager(const string& name);

		void DestorySceneManager(SceneManager* sm);

		Core();
		~Core();
	private:

		IRenderSystem* mActiveRenderSystem;
	};
}

#endif