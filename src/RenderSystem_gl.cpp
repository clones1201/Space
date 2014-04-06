#include "RenderSystem.h"
#include "WindowController.h"
#include <GL\freeglut.h>

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


		void RenderSystemOpenGL::Init(HWND hWnd, uint width, uint height){
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
			
			glViewport(0, 0, width, height);
			glClearColor(0, 0, 1, 1);
		}
		
		RenderSystemOpenGL::RenderSystemOpenGL(HWND hWnd, uint width, uint height) :RenderSystem(width, height){
			glewInit();

			Init(hWnd, width, height);
			camera = PerspectiveCamera_ptr(new PerspectiveCamera);
		
		}
		RenderSystemOpenGL::~RenderSystemOpenGL(){
			 
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(hGLRC);
			
			ReleaseDC(GetWindowController()->GetWndHandler(), hDC);
		}

		void RenderSystemOpenGL::Resize(int _width, int _height){
			RenderSystem::width = _width;
			RenderSystem::height = _height;
			glViewport(0, 0, _width, _height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			camera->SetAspect(_width / float(_height));
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
			camera = PerspectiveCamera_ptr(new PerspectiveCamera(c)); 
		}

		void RenderSystemOpenGL::SetTransform( TransformType type ,const math::Matrix& mat){
			switch (type){
				case SP_VIEW:
					glMatrixMode(GL_MODELVIEW);
					math::Matrix matView = camera->GetMatrix();
					matView = MatrixMultiply(matView, mat);
					glLoadMatrixf(math::MatrixTranspose(matView).m);
					break;
			}
		}

		void RenderSystemOpenGL::SetColor(const Color& color){
			glColor4fv((GLfloat*)&color);
		}

		void RenderSystemOpenGL::SetMaterial(const Material& material){
			float shininess = material.kd * 128;
			float diffuseColor[3] = { material.diffuse.r, material.diffuse.g, material.diffuse.b };
			float specularColor[4] = { material.specular.r, material.specular.g, material.specular.b, 1.0f };

			// set specular and shiniess using glMaterial (gold-yellow)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); // range 0 ~ 128
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

			// set ambient and diffuse color using glColorMaterial (gold-yellow)
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		}
		void RenderSystemOpenGL::SetTexture(Texture* tex){
			if (tex == nullptr){
				glBindTexture(GL_TEXTURE_2D, 0);
				return;
			}
			if (tex->GetTexId() != 0){
				glBindTexture(GL_TEXTURE_2D, tex->GetTexId());
			}
			else{
				uint format = 4 == tex->GetFormat() ? GL_RGBA : GL_RGB;

				uint Id;
				glGenTextures(1, &Id);
				tex->SetTexId(Id);
				glBindTexture(GL_TEXTURE_2D, tex->GetTexId());
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex->GetWidth(), tex->GetHeight(), format, GL_UNSIGNED_BYTE, tex->GetTextureImage().data());
				//glTexImage2D(GL_TEXTURE_2D, 3	, format, width, height, 0, format, GL_UNSIGNED_BYTE, image.data());

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		void RenderSystemOpenGL::DrawMesh(const Mesh& mesh){
			
			glVertexPointer(mesh.GetPositionSize(), GL_FLOAT, mesh.GetPositionSize() * sizeof(float), mesh.GetPositions());
			glNormalPointer(GL_FLOAT, mesh.GetNormalSize() * sizeof(float), mesh.GetNormals() );
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);

			glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, mesh.GetPositionIndices() );

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY); 
		}

		void RenderSystemOpenGL::DrawSolidMesh(const Mesh& mesh){
			
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

		void RenderSystemOpenGL::DrawSphere(float r){}

		const float cubevertices[] = {
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		};
		const uint wirecube_indices[] = {
			0, 1,	1, 2,	2, 3,	3, 0,
			0, 4,	1, 5,	2, 6,	3, 7,
			4, 5,	5, 6,	6, 7,	7, 4
		};

		void RenderSystemOpenGL::DrawCube(float a, float b, float c){
			float vertices[24];

			for (uint i = 0; i < 8; i++){
				vertices[i * 3] = cubevertices[i * 3] * a;
				vertices[i * 3 + 1] = cubevertices[i * 3 + 1] * b;
				vertices[i * 3 + 2] = cubevertices[i * 3 + 2] * c;
			}

			glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), vertices);
			
			glEnableClientState(GL_VERTEX_ARRAY);

			glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, wirecube_indices);

			glDisableClientState(GL_VERTEX_ARRAY);
		}

		void RenderSystemOpenGL::DrawPlane(math::Vector3 normal){}


		void RenderSystemOpenGL::BeginScene(){

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);
			glDepthFunc(GL_LEQUAL);

			glEnable(GL_TEXTURE_2D);

		}
		void RenderSystemOpenGL::EndScene(){ 
			glFlush();
			SwapBuffers(hDC); 
		}
	}
}
