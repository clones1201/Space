#include "Space.h"
#include <Windows.h>

#include "RayTracing.h"

using namespace space;
using namespace space::graphic;
using namespace space::math;
using namespace space::graphic::raytrace;

Mesh cube;

void Init(){
	vector<uint> image;

	Vector3 eye(-3, 1, 0), lookat(0, 0, 0);
	PerspectiveCamera camera(eye, lookat - eye);
	GetGame()->GetRenderDevice()->SetView(camera);

	cube.LoadFromFile("../../resourse/cubeuv.obj");
	cube.Rescale(0.5);
	cube.CompileModel(SP_TRIANGLES);
}

float dt = 1;
void display(){
	clock_t t1 = clock();

	GetGame()->GetRenderDevice()->RotateEye(10, 0);

	Material material;
	material.specular = white;
	material.ks = 0.4;
	material.n = 20;

	material.diffuse = white;
	material.kd = 0.6;

	material.ambient = white;
	material.ka = 0.1;

	material.reflect = 0.3f;
	GetGame()->GetRenderDevice()->SetMaterial(material);

	GetGame()->GetRenderDevice()->SetColor(gray);
	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation(0, -0.5, 0));
	GetGame()->GetRenderDevice()->DrawPlane(Vector3(0, 1, 0));

	material.reflect = 0.6f;
	GetGame()->GetRenderDevice()->SetMaterial(material);

	GetGame()->GetRenderDevice()->SetColor(white);
	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation(0.5, 0, 1.3));
	GetGame()->GetRenderDevice()->DrawSolidMesh(cube);

	material.reflect = 0.2f;
	GetGame()->GetRenderDevice()->SetMaterial(material);

	GetGame()->GetRenderDevice()->SetColor(red);
	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation(-0.7, 0, 0.7));
	GetGame()->GetRenderDevice()->DrawSolidMesh(cube);

	material.reflect = 0.3f;
	GetGame()->GetRenderDevice()->SetMaterial(material);
	GetGame()->GetRenderDevice()->SetColor(yellow);
	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation(1.0, 0, 0));
	GetGame()->GetRenderDevice()->DrawSphere(0.5);

	GetGame()->GetRenderDevice()->Flush();

	clock_t t2 = clock();
	dt = (t2 - t1) / 1000.0;

	wostringstream strfps;
	static uint fcount = 0;
	strfps << "Space:Ray Trace " << "fc:" << fcount << "fps " << 1 / dt << "time " << dt << "s";

	GetWindowController()->SetWindowsTitle(strfps.str());
	fcount++;
}

int main(){
	Init();

	display();

	return 0;
}