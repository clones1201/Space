#include "Space.h"
#include <Windows.h>

#include "RayTracing.h"

using namespace space;
using namespace space::graphic;
using namespace space::math;
using namespace space::graphic::raytrace;

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT){
	UNREFERENCED_PARAMETER(hInst);
	return SetupRenderDevice<RS_RT>();
}

Mesh cube;
 

void Init(){
	vector<uint> image;

	Vector3 eye(0, 1, 10), lookat(0, 0, 0);
	PerspectiveCamera camera(eye, lookat - eye);
	GetGame()->GetRenderDevice()->SetView(camera);
	cube.LoadFromFile("../../resourse/cube.obj");
	cube.Rescale(1);
	cube.CompileModel(SP_TRIANGLES);
}

float dt = 1;
void display(){	
	clock_t t1 = clock();
	
	//GetGame()->GetRenderDevice()->RotateEye(5, 0);
	GetGame()->GetRenderDevice()->DrawSolidMesh(cube);
	
	RenderSystemRayTrace* tracer = (RenderSystemRayTrace*)(GetGame()->GetRenderDevice());

	tracer->Flush();

	clock_t t2 = clock();
	dt = (t2 - t1) / 1000.0;

	ostringstream strfps;
	strfps << "Space:Ray Trace " << "fps " << 1 / dt << "time " << dt<<"s";
	
}