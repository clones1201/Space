#ifndef __SPACE_H__
#define __SPACE_H__

#include "global.h"
#include "basic.h"
#include "WindowController.h"
#include "Camera.h"
#include "Mesh.h"
#include "Entity.h"

namespace space{
	class IGame : public Interface{
	public:
		virtual void Run() = 0;
		virtual graphic::RenderSystem* GetRenderDevice() = 0;
	};
	
	IGame* GetGame();

	template< graphic::RenderSystemType >
	int SetupRenderDevice(uint width,uint height);

	void DrawAxis();
}

#endif