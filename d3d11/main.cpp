#include "Space.h"
#include <Windows.h>

#include <d3dx11.h>

using namespace space;
using namespace space::graphic;
using namespace space::math;

int main(){
	//UNREFERENCED_PARAMETER(hInst);
	return SetupRenderDevice<RS_D3D>(640, 480);
}

Mesh venusm;

void Init(){
	/*venusm.LoadFromFile("../../resourse/venusm.obj");
	venusm.Rescale(1);
	venusm.ComputeTexCoords();
	venusm.CompileModel(SP_TRIANGLES);*/
}

void display(){
	GetGame()->GetRenderDevice()->BeginScene();

	GetGame()->GetRenderDevice()->EndScene();
}