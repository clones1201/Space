#include "RenderSystem.h"
#include "WindowController.h"

namespace space{
	namespace graphic{
		 
		// modified from "fghFillPFD" in *freeglut*
		static void spFillPFD(PIXELFORMATDESCRIPTOR *ppfd, HDC hdc)
		{
			ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
			ppfd->nVersion = 1;
			ppfd->dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

			ppfd->iPixelType = PFD_TYPE_RGBA;
			ppfd->cColorBits = 24;
			ppfd->cRedBits = 8;
			ppfd->cGreenBits = 8;
			ppfd->cBlueBits = 8;
			ppfd->cAlphaBits = 8;

			ppfd->cAccumBits = 1;
			ppfd->cAccumRedBits = 0;
			ppfd->cAccumGreenBits = 0;
			ppfd->cAccumBlueBits = 0;
			ppfd->cAccumAlphaBits = 0;

			/* Hmmm, or 32/0 instead of 24/8? */
			ppfd->cDepthBits = 24;
			ppfd->cStencilBits = 8;

			ppfd->iLayerType = PFD_MAIN_PLANE;
			ppfd->bReserved = 0;
			ppfd->dwLayerMask = 0;
			ppfd->dwVisibleMask = 0;
			ppfd->dwDamageMask = 0;

			ppfd->cColorBits = (BYTE)GetDeviceCaps(hdc, BITSPIXEL);
		}


		void RenderSystemOpenGL::Init(HWND hWnd){
			hDC = GetDC(hWnd);
			PIXELFORMATDESCRIPTOR pfd;
			memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
			spFillPFD(&pfd, hDC);

			int nPixelFormat = ChoosePixelFormat(hDC, &pfd);

			if (nPixelFormat == 0) return ;

			BOOL bResult = SetPixelFormat(hDC, nPixelFormat, &pfd);

			if (!bResult) return ;

			HGLRC tempContext = wglCreateContext(hDC);
			wglMakeCurrent(hDC, tempContext);

			GLenum err = glewInit();
			if (GLEW_OK != err){
				MessageBox(hWnd, L"GLEW is not initialized!", L"GLEW", 0);
			}

			int attribs[] =	{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
				WGL_CONTEXT_MINOR_VERSION_ARB, 1,
				WGL_CONTEXT_FLAGS_ARB, 0,
				0
			};

			if (wglewIsSupported("WGL_ARB_create_context") == 1){
				hGLRC = wglCreateContextAttribsARB(hDC, 0, attribs);
				wglMakeCurrent(NULL, NULL);
				wglDeleteContext(tempContext);
				wglMakeCurrent(hDC, hGLRC);
			}
			else{       //It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
				hGLRC = tempContext;
			}

			if (!hGLRC){
				MessageBox(hWnd, L"wglCreateContext() failed", L"Error", MB_ICONERROR | MB_OK);
				exit(1);
			}

			if (!wglMakeCurrent(hDC, hGLRC)){
				MessageBox(hWnd, L"wglMakeCurrent() failed", L"Error", MB_ICONERROR | MB_OK);
				exit(1);
			}
			
			glViewport(0, 0, 650, 650);
			glClearColor(0, 0, 1, 1);
		}
		
		void RenderSystemOpenGL::SwapBuffer(){
			SwapBuffers(hDC);
		}

		RenderSystemOpenGL::RenderSystemOpenGL(HWND hWnd){
			glewInit();

			Init(hWnd);
			camera = new PerspectiveCamera;
		
		}
		RenderSystemOpenGL::~RenderSystemOpenGL(){
			if (camera != NULL){
				delete camera;
			}
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(hGLRC);
			
			ReleaseDC(GetWindowController()->GetWndHandler(), hDC);
		}

		void RenderSystemOpenGL::Resize(int width, int height){
			glViewport(0, 0, width, height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			camera->SetAspect(width / float(height));
		}

		void RenderSystemOpenGL::LookAt(float eyex, float eyey, float eyez,
			float centrex, float centrey, float centrez,
			float upx, float upy, float upz){
			camera->SetEye(eyex, eyey, eyez);
			camera->SetLookAt(centrex, centrey, centrez);
		}
		void RenderSystemOpenGL::Perspective(float fovy, float aspect, float zNear, float zFar){
			camera->SetPerspective(fovy, aspect, zNear, zFar);
		}
		void RenderSystemOpenGL::RotateEye(float x, float y){
			camera->RotateE(x, y);
		}
		void RenderSystemOpenGL::RotateLook(float x, float y){
			camera->RotateL(x, y);
		}
		void RenderSystemOpenGL::SetView(const PerspectiveCamera & c){
			IPerspectiveCamera* pd = camera;
			camera = new PerspectiveCamera(c);
			delete pd; 
		}

		void RenderSystemOpenGL::SetTransform( TransformType type ,const Matrix& mat){
			switch (type){
				case SP_VIEW:
					glMatrixMode(GL_MODELVIEW);
					Matrix matView = camera->GetMatrix();
					matView = MatrixMultiply(matView, mat);
					glLoadMatrixf( MatrixTranspose( matView ).m);
					break;
			}
		}
		 
		void RenderSystemOpenGL::DrawMesh(const Mesh& mesh){
			float shininess = 15.0f;
			float diffuseColor[3] = { 0.929524f, 0.796542f, 0.178823f };
			float specularColor[4] = { 1.00000f, 0.980392f, 0.549020f, 1.0f };

			// set specular and shiniess using glMaterial (gold-yellow)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); // range 0 ~ 128
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

			// set ambient and diffuse color using glColorMaterial (gold-yellow)
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
			glColor3fv(diffuseColor); 

			glColor3f(1.0, 1.0, 1.0);
			glVertexPointer(mesh.GetPositionSize(), GL_FLOAT, mesh.GetPositionSize() * sizeof(float), mesh.GetPositions());
			glNormalPointer(GL_FLOAT, mesh.GetNormalSize() * sizeof(float), mesh.GetNormals() );
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);

			glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, mesh.GetPositionIndices() );

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY); 
		}

		void RenderSystemOpenGL::DrawSolidMesh(const Mesh& mesh){
			float shininess = 15.0f;
			float diffuseColor[3] = { 0.929524f, 0.796542f, 0.178823f };
			float specularColor[4] = { 1.00000f, 0.980392f, 0.549020f, 1.0f };

			// set specular and shiniess using glMaterial (gold-yellow)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); // range 0 ~ 128
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

			// set ambient and diffuse color using glColorMaterial (gold-yellow)
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
			glColor3fv(diffuseColor);

			glColor4f(1.0, 1.0, 1.0, 1.0);
			glVertexPointer(mesh.GetPositionSize(), GL_FLOAT, mesh.GetCompiledVertexSize() * sizeof(float), mesh.GetCompiledVertices());
			glEnableClientState(GL_VERTEX_ARRAY);
			if (mesh.hasNormals()){
				glEnableClientState(GL_NORMAL_ARRAY);
				GLfloat* normals = (GLfloat*)(mesh.GetCompiledVertices() + mesh.GetCompiledNormalOffset());
				glNormalPointer(GL_FLOAT, mesh.GetCompiledVertexSize() * sizeof(float), normals);
			}
			if (mesh.hasTexCoords()){
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				GLfloat *texcoords = (GLfloat*) (mesh.GetCompiledVertices() + mesh.GetCompiledTexCoordOffset());
				glTexCoordPointer(mesh.GetTexCoordSize(), GL_FLOAT, mesh.GetCompiledVertexSize() *sizeof(float), texcoords);
			}
			else{
				// should use glTexGen or somewhat, i don't know how
			}
			glDrawElements(SP_TRIANGLES, mesh.GetCompiledIndexCount(SP_TRIANGLES), GL_UNSIGNED_INT, mesh.GetCompiledIndices(SP_TRIANGLES));

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		void RenderSystemOpenGL::DrawWiredMesh(const Mesh& mesh){
			float shininess = 15.0f;
			float diffuseColor[3] = { 0.929524f, 0.796542f, 0.178823f };
			float specularColor[4] = { 1.00000f, 0.980392f, 0.549020f, 1.0f };

			// set specular and shiniess using glMaterial (gold-yellow)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); // range 0 ~ 128
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

			// set ambient and diffuse color using glColorMaterial (gold-yellow)
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
			glColor3fv(diffuseColor);

			glColor4f(1.0, 1.0, 1.0, 1.0);
			glVertexPointer(mesh.GetPositionSize(), GL_FLOAT, mesh.GetCompiledVertexSize() * sizeof(float), mesh.GetCompiledVertices());
			glNormalPointer(GL_FLOAT, mesh.GetCompiledVertexSize() * sizeof(float), mesh.GetCompiledVertices() + mesh.GetCompiledNormalOffset());
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);

			glDrawElements(SP_LINES, mesh.GetCompiledIndexCount(SP_LINES), GL_UNSIGNED_INT, mesh.GetCompiledIndices(SP_LINES));

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
		}

		void RenderSystemOpenGL::DrawScene( Scene& scene ){
			vector<Entity*>::iterator eIter = scene.GetEntitys().begin();
			for (; eIter != scene.GetEntitys().end();){ 

			}
		}
	}
}
