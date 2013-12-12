// just for math function test

#include <iostream>
#include <vector>
#include <memory>
using namespace std;

#include <Space.h>
using namespace space::math;
using namespace space;

ostream& operator<<(ostream &out, const Matrix &mat){
	for (int i = 0; i < 16; i++){
		out << mat.m[i] << '\t';
		if (i % 4 == 3) out << endl;
	}
	return out;
}

ostream& operator<<(ostream &out, const Vector3 &vec){
	out << vec.x << '\t' << vec.y << '\t' << vec.z;
	return out;
}

ostream& operator<<(ostream &out, const Vector4 &vec){
	out << vec.x << '\t' << vec.y << '\t' << vec.z << '\t' << vec.w;
	return out;
}

int main(){
	return SetupRenderDevice<RS_OGL>();
}

shared_ptr<float> vtx;
shared_ptr<uint> idx;

float vertices[] = {
	-1, -1, 0, -1, -1, -2,
	-1, 1, 0, -1, 1, -2,
	1, -1, 0, 1, -1, -2,
	1, 1, 0, 1, 1, -2
};

float verticesR[] = {
	-1.20711, -0.707107, 0.207107, -2.20711, 0.707107, -0.792893, 
	-0.207107, 0.707107, 1.20711, -1.20711, 2.12132, 0.207107, 
	0.207107, -0.707107, -1.20711, -0.792893, 0.707107, -2.20711, 
	1.20711, 0.707107, -0.207107, 0.207107, 2.12132, -1.20711 
};

float verticesM[24];

uint indices[] = {
	0,1,	1,3,	3,2,	2,0,
	0,4,	1,5,	3,7,	2,6,
	4,5,	5,7,	7,6,	6,4
};

Mesh cube;

void Init(){

	cube.LoadFromFile("../../resourse/cube.obj");
	cube.Rescale(0.5);
	cube.CompileModel(SP_LINES);

	PerspectiveCamera view(Vector3(0, 2, 7), Vector3(0, -2, -7));
	view.SetPerspective(60, 1, 1, 1000);
	GetGame()->GetRenderDevice()->SetView(view);

	Matrix viewMat;

	PerspectiveCamera camera;
	camera.SetPerspective(60, 1, 1, 1000);
	//camera.Move(Vector3(1,1,1));
	camera.RotateL(0,-45);
	viewMat = camera.GetModelViewMatrix();

	Matrix matWorld;
	matWorld = MatrixTranslation(1, 1, 1);
	
	cout << viewMat << "=========\n";
	cout << matWorld << "=========\n";
	cout.precision(3);
	for (uint i = 0; i < 8; i++){
		cout << verticesR[i * 3 + 0] << "  " << verticesR[i * 3 + 1] << "  " << verticesR[i * 3 + 2] << ";";
		Vector4 p = Vec4Transform(MatrixMultiply(viewMat, matWorld), Vector4(verticesR[i * 3 + 0], verticesR[i * 3 + 1], verticesR[i * 3 + 2], 1));
		verticesM[i * 3 + 0] = p.x; verticesM[i * 3 + 1] = p.y; verticesM[i * 3 + 2] = p.z;
		cout << verticesM[i * 3 + 0] << "  " << verticesM[i * 3 + 1] << "  " << verticesM[i * 3 + 2] << endl;
	}
	 
}

void DrawLines(float *vtx, uint *idx){
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), vtx);
	glEnableClientState(GL_VERTEX_ARRAY);

	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, idx);

	glDisableClientState(GL_VERTEX_ARRAY);
}

float dt;
void display(){
	clock_t t1 = clock();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	glDepthFunc(GL_LEQUAL);

	GetGame()->GetRenderDevice()->RotateEye( 30 * dt , 0);
	
	DrawAxis();
	
	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW, MatrixTranslation(0, 0, 0));
	//DrawLines(vertices, indices);
	DrawLines(verticesR, indices);
	DrawLines(verticesM, indices);

	glFlush();
	Sleep(1);
	clock_t t2 = clock();
	dt = (t2 - t1) / 1000.0;
}