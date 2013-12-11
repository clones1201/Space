#ifndef __SPACE_H__
#define __SPACE_H__

#include "basic.h"
#include "global.h"
#include "WindowController.h"
#include "Camera.h"
#include "Mesh.h"
#include "Entity.h"

namespace space{
	using namespace graphic;

	class IGame : public Interface{
	public:
		virtual void Run() = 0;
		virtual RenderSystem* GetRenderDevice() = 0;
	};
	
	IGame* GetGame();

	template< RenderSystemType >
	int SetupRenderDevice();

	void DrawAxis();
}

#endif