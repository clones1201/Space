
//#include "nvModel.h"
#include "Space.h"
#include <Windows.h>

using namespace space;
using namespace space::graphic;
using namespace space::math; 

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT){
	UNREFERENCED_PARAMETER(hInst);
	return SetupRenderDevice<RS_OGL>(550,550);
}

float dt = 0.01f;

Mesh venusm;

Texture tex1,tex2; 

void initLights();

void Init(){ 
		
	tex1.LoadFromPNG("../../resourse/AS2_leather_01.png");
	tex2.LoadFromPNG("../../resourse/GRYCON3.png");
	 
	venusm.LoadFromFile("../../resourse/venusm.obj");
	venusm.Rescale(1);
	venusm.ComputeTexCoords();
	venusm.CompileModel(SP_TRIANGLES);
	 
	Vector3 eye(0, 1, 4), lookat(0, 0, 0);
	PerspectiveCamera camera(eye,lookat-eye);
	GetGame()->GetRenderDevice()->SetView(camera);

	GetGame()->GetRenderDevice()->SetTexture(&tex1);
	GetGame()->GetRenderDevice()->SetTexture(&tex2);
	initLights();
}
 

void display(){
	clock_t t1 = clock();

	GetGame()->GetRenderDevice()->BeginScene();

	GetGame()->GetRenderDevice()->RotateEye(dt * 90, 0);
	
	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation(2, 3, 2));
	initLights();

	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixIdentity());
	
	Material material;
	material.diffuse = gold;
	material.kd = 15.0f/128.0f;

	material.specular = white;
	GetGame()->GetRenderDevice()->SetMaterial(material);
	
	glBindTexture(GL_TEXTURE_2D, tex1.GetTexId());
	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation(2, 0, 0));
	GetGame()->GetRenderDevice()->DrawSolidMesh(venusm);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glBindTexture(GL_TEXTURE_2D, tex2.GetTexId());
	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation(-2, 0, 0));
	GetGame()->GetRenderDevice()->DrawSolidMesh(venusm);
	glBindTexture(GL_TEXTURE_2D, 0);

	GetGame()->GetRenderDevice()->EndScene();
	clock_t t2 = clock();
	dt =( t2 - t1 )/ 1000.0;
}

//=============================================================================
// initialize lights
//=============================================================================
void initLights()
{
	// set up light colors (ambient, diffuse, specular)
	GLfloat lightKa[] = { .2f, .2f, .2f, 1.0f };  // ambient light
	GLfloat lightKd[] = { .7f, .7f, .7f, 1.0f };  // diffuse light
	GLfloat lightKs[] = { .7f, .7f, .7f, 1.0f };           // specular light
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

	// position the light
	float lightPos[4] = { 0, 0, 0, 1 }; // positional light
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
}