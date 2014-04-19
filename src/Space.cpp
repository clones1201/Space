#include "Space.h"

#include "Prerequisites.h"

extern void display();
extern void Init();

namespace space{

	template < graphic::RenderSystemType >
	class Game :public IGame {
	private:
	public:
		Game(uint width, uint height);

		~Game(){
		}

		void Run(){
			GetWindowController()->Loop();
		}

		graphic::RenderSystem* GetRenderDevice(){
			return GetWindowController()->GetRenderSystem();
		}
	};

	template <>
	Game<graphic::RS_OGL>::Game(uint width, uint height){
		SetWindowController(new WindowController(graphic::RS_OGL, width, height));
		GetWindowController()->SetDisplayFunc(&display);
	} 

	template <>
	Game<graphic::RS_RT>::Game(uint width, uint height){
		SetWindowController(new WindowController(graphic::RS_RT, width, height));
		GetWindowController()->SetDisplayFunc(&display);
	}

	template <>
	Game<graphic::RS_D3D>::Game(uint width, uint height){
		SetWindowController(new WindowController(graphic::RS_D3D, width, height));
		GetWindowController()->SetDisplayFunc(&display);
	}

	IGame* game;

	IGame* GetGame(){
		return game;
	}

	template <>
	int SetupRenderDevice<graphic::RS_OGL>(uint width, uint height){
		game = new Game<graphic::RS_OGL>(width, height);
		Init();
		game->Run();
		delete game;
		return 0;
	}

	template <>
	int SetupRenderDevice<graphic::RS_RT>(uint width, uint height){
		game = new Game<graphic::RS_RT>(width, height);
		Init();
		game->Run();
		delete game;
		return 0;
	}

	template <>
	int SetupRenderDevice<graphic::RS_D3D>(uint width, uint height){
		game = new Game<graphic::RS_D3D>(width, height);
		Init();
		game->Run();
		delete game;
		return 0;
	}


	void DrawAxis(){
		GetGame()->GetRenderDevice()->SetTransform(graphic::SP_VIEW, math::MatrixTranslation(0, 0, 0));
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