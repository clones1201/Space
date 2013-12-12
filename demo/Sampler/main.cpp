#include "Space.h"
#include <Windows.h>

#include "RayTracing.h"

using namespace space;
using namespace space::graphic;
using namespace space::math;

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT){
	UNREFERENCED_PARAMETER(hInst);
	return SetupRenderDevice<RS_OGL>();
}

vector<Vector3> samples;

void Init(){

	Vector3 eye(2, 2, 2), lookat(0, 0.5, 0);
	PerspectiveCamera camera(eye, lookat - eye);
	GetGame()->GetRenderDevice()->SetView(camera);
	
	Vector3 o(0, 0, 0), n(0.5, 1, 0.5);

	samples.resize(1000);
	for (uint i = 0; i < 1000; i++){
		samples[i] = Sample::Instance()->HemiSphere(n, o, 0.5,30);
	}

	glClearColor(0, 0, 0, 1);
}


float dt = 1;
void display(){
	clock_t t1 = clock();

	GetGame()->GetRenderDevice()->RotateEye(50*dt, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation(0, 0, 0));
	glColor3f(0, 1, 0);
	glPointSize(2.0);
	glBegin(GL_POINTS);
	for (uint i = 0; i < 1000; i++){
		glVertex3fv((GLfloat*)&samples[i]);
	}
	glEnd();
	DrawAxis();
	GetGame()->GetRenderDevice()->Flush();

	clock_t t2 = clock();
	dt = (t2 - t1) / 1000.0;

	ostringstream strfps;
	strfps << "Space:Ray Trace " << "fps " << 1 / dt << "time " << dt << "s";

}