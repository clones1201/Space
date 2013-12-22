#include "Space.h"
#include <Windows.h>

#include "RayTracing.h"

using namespace space;
using namespace space::graphic;
using namespace space::math;
using namespace space::graphic::raytrace;

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT){
	UNREFERENCED_PARAMETER(hInst);
	return SetupRenderDevice<RS_RT>(640,320);
}

Mesh venusm;
Mesh cube;
Texture tex;

void Init(){
	vector<uchar> chess;
	chess.resize(400 * 4);
	for (uint i = 0; i < 20; i++){
		for (uint j = 0; j < 20; j++){
			if (i < 10 && j >= 10 || i >= 10 && j < 10){
				chess[4 * (i * 20 + j)] = 0;
				chess[4 * (i * 20 + j) + 1] = 0;
				chess[4 * (i * 20 + j) + 2] = 0;
				chess[4 * (i * 20 + j) + 3] = 255;
			}
			else{
				chess[4 * (i * 20 + j)] = 255;
				chess[4 * (i * 20 + j) + 1] = 255;
				chess[4 * (i * 20 + j) + 2] = 255;
				chess[4 * (i * 20 + j) + 3] = 255;
			}
		}
	}
	tex.Load(chess, 20, 20);

	Vector3 eye(1,1,-2), lookat(0, 0, 0);
	PerspectiveCamera camera(eye, lookat - eye);
	GetGame()->GetRenderDevice()->SetView(camera);

	cube.LoadFromFile("../../resourse/cubeuv.obj");
	cube.Rescale(0.5);
	cube.CompileModel(SP_TRIANGLES); 
	
	venusm.LoadFromFile("../../resourse/venusm.obj");
	venusm.Rescale(0.5f);
	venusm.CompileModel(SP_TRIANGLES);
}

float dt = 1;
void display(){	
	clock_t t1 = clock();

	GetGame()->GetRenderDevice()->RotateEye(10,0);

	Material material;
	material.specular = white;
	material.ks = 0.4;
	material.n = 20;
	material.diffuse = white;
	material.kd = 0.6;
	material.ambient = white;
	material.ka = 0.1;

	/*material.reflect = 0.5f;
	material.refract = 0.0f;
	material.greflect = 0.0f;
	GetGame()->GetRenderDevice()->SetMaterial(material);
	GetGame()->GetRenderDevice()->SetTexture(&tex);

	GetGame()->GetRenderDevice()->SetColor(gray);
	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation(0, -0.5, 0));
	GetGame()->GetRenderDevice()->DrawPlane(Vector3(0, 1, 0));
	GetGame()->GetRenderDevice()->SetTexture(nullptr);
*/
	material.reflect = 0.5f;
	material.refract = 0.0f;
	material.greflect = 0.0f;
	GetGame()->GetRenderDevice()->SetMaterial(material);
	GetGame()->GetRenderDevice()->SetColor(yellow);
	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation(0.5, 0, 1.3));
	GetGame()->GetRenderDevice()->DrawSolidMesh(cube);

	/*material.reflect = 0.5f;
	material.refract = 0.5f;
	material.greflect = 0.0f;
	GetGame()->GetRenderDevice()->SetMaterial(material);
	GetGame()->GetRenderDevice()->SetColor(white);
	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation(-0.7, 0, 0.7));
	GetGame()->GetRenderDevice()->DrawSolidMesh(venusm);
*/
	material.reflect = 0.3f;
	material.refract = 0.8f;
	material.greflect = 0.0f;
	GetGame()->GetRenderDevice()->SetMaterial(material);
	GetGame()->GetRenderDevice()->SetColor(white);
	for (uint i = 0; i < 100; i++){
		GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation( rand()/float(RAND_MAX), 0, rand()/float(RAND_MAX)));
		GetGame()->GetRenderDevice()->DrawSphere(0.2);
	}
	material.reflect = 0.5f;
	material.refract = 0.0f;
	material.greflect = 0.0f;
	GetGame()->GetRenderDevice()->SetMaterial(material);
	GetGame()->GetRenderDevice()->SetColor(green);
	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation(0.0, -0.2, -0.5));
	GetGame()->GetRenderDevice()->DrawSphere(0.3);

	material.reflect = 0.3f;
	material.refract = 0.0f;
	material.greflect = 0.0f;
	GetGame()->GetRenderDevice()->SetMaterial(material);
	GetGame()->GetRenderDevice()->SetTexture(&tex);
	GetGame()->GetRenderDevice()->SetColor(white);
	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixScale(4,1,4) * MatrixTranslation(0, -1, 0));
	GetGame()->GetRenderDevice()->DrawSolidMesh(cube);
	GetGame()->GetRenderDevice()->SetTexture(nullptr);
	GetGame()->GetRenderDevice()->Flush();

	clock_t t2 = clock();
	dt = (t2 - t1) / 1000.0;

	wostringstream strfps;
	static uint fcount = 0;
	strfps.precision(4);
	strfps << "Space:Ray Trace " << " time:" << dt << "s" << 1 / dt << " fps ";
	
	GetWindowController()->SetWindowsTitle(strfps.str());
	fcount++;
}