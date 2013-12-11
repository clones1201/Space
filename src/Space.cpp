#include "Space.h"

#include "global.h"

extern void display();
extern void Init();

namespace space{

	template <RenderSystemType>
	class Game :public IGame {
	private:
	public:
		Game();

		~Game(){
		}

		void Run(){
			GetWindowController()->Loop();
		}

		RenderSystem* GetRenderDevice(){
			return GetWindowController()->GetRenderSystem();
		}
	};

	template <>
	Game<RS_OGL>::Game(){
		SetWindowController(new WindowController(RS_OGL));
		GetWindowController()->SetDisplayFunc(&display);
	} 

	template <>
	Game<RS_RT>::Game(){
		SetWindowController(new WindowController(RS_RT));
		GetWindowController()->SetDisplayFunc(&display);
	}

	template <>
	Game<RS_D3D>::Game(){
		SetWindowController(new WindowController(RS_D3D));
		GetWindowController()->SetDisplayFunc(&display);
	}

	IGame* game;

	IGame* GetGame(){
		return game;
	}

	template <>
	int SetupRenderDevice<RS_OGL>(){
		game = new Game<RS_OGL>();
		Init();
		game->Run();
		delete game;
		return 0;
	}

	template <>
	int SetupRenderDevice<RS_RT>(){
		game = new Game<RS_RT>();
		Init();
		game->Run();
		delete game;
		return 0;
	}

	template <>
	int SetupRenderDevice<RS_D3D>(){
		game = new Game<RS_D3D>();
		Init();
		game->Run();
		delete game;
		return 0;
	}


	void DrawAxis(){
		GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation(0, 0, 0));
		glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0); glVertex3f(1, 0, 0);
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0); glVertex3f(0, 1, 0);
		glColor3f(0, 1, 1);
		glVertex3f(0, 0, 0); glVertex3f(0, 0, 1);
		glEnd();
	}
}