#include "Space.h"
#include <Windows.h>
#include <iostream>

#include "RayTracing.h"

using namespace space;
using namespace space::util;
using namespace space::math;
using namespace space::graphic;
using namespace space::graphic::raytrace;

class BTreeNode : public BinaryTreeNode<uint>{
public:
	BTreeNode(uint _nelem) :BinaryTreeNode<uint>(_nelem){}
};

class BTreeLeaf : public BinaryTreeLeaf<uint>{
public:
	float leafelem;
	BTreeLeaf(uint _nelm, float _lfe) :BinaryTreeLeaf<uint>(_nelm){
		leafelem = _lfe;
	}
};

void BuildTree(BTreeNode::Ptr &node, uint depth){
	if (depth == 0){
		node = (BTreeNode::Ptr)(new BTreeLeaf(depth, 0.1f));
		return;
	}

	node = BTreeNode::Ptr(new BTreeNode(depth));

	BTreeNode::Ptr left = nullptr, right = nullptr;
	BuildTree(left, depth - 1);
	BuildTree(right, depth - 1);
	node->SetChildren(left, right);
}

void TravelTree(BTreeNode::Ptr node){

	if (!node->isLeaf()){
		std::cout << node->GetElem() << " ";
		TravelTree((node->GetLeft()));
		TravelTree((node->GetRight()));
	}
	else{
		std::cout << node->GetElem() << " ";
		std::cout << ((BTreeLeaf*)node.get())->leafelem << " " << endl;
	}
}

void TravelBSPTree(BSPNode::Ptr node){
	graphic::BBox box;

	((BSPNode*)(node.get()))->CalculateBoundsBox(box.bmax, box.bmin);

	GetGame()->GetRenderDevice()->SetTransform(SP_VIEW,
		MatrixTranslation(box.bmin.x, box.bmin.y, box.bmin.z)
		);
	GetGame()->GetRenderDevice()->DrawCube(box.bmax.x - box.bmin.x, box.bmax.y - box.bmin.y, box.bmax.z - box.bmin.z);

	if (!node->isLeaf()){
		TravelBSPTree((node->GetLeft()));
		TravelBSPTree((node->GetRight()));
	}
}

int main(){
	//BTreeNode::Ptr root = nullptr;
	////for (uint i = 0; i < 1000;i++)	
	//	BuildTree(root, 3);

	//TravelTree(root);
	SetupRenderDevice<graphic::RS_OGL>(640, 480);

	std::cout << "press any key to continue..." << endl;
	std::cin.get();
	return 0;
}

Mesh venusm;
Mesh cube;

BSPNode::Ptr root;


void Init(){
	venusm.LoadFromFile("../../resourse/venusm.obj");
	venusm.Rescale(1.0f);
	venusm.CompileModel(SP_TRIANGLES);

	cube.LoadFromFile("../../resourse/cubeuv.obj");
	cube.Rescale(0.5);
	cube.CompileModel(SP_TRIANGLES);

	vector<Primitive_ptr> prims;

	CreatePrimitives(prims, venusm);

	vector<Primitive_ptr> bsp;
	root = BuildBSPTree(bsp,prims,16);

	Vector3 eye(0, 1, 2), lookat(0, 0, 0);
	PerspectiveCamera camera(eye, lookat - eye);
	GetGame()->GetRenderDevice()->SetView(camera);
}

float dt;
void initLights();

void display(){

	clock_t t1 = clock();

	GetGame()->GetRenderDevice()->RotateEye(dt * 90, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glDepthFunc(GL_LEQUAL);
	initLights();


	Material material;
	material.diffuse = gold;
	material.kd = 15.0f / 128.0f;
	GetGame()->GetRenderDevice()->SetMaterial(material);
	GetGame()->GetRenderDevice()->SetColor(graphic::white);
	GetGame()->GetRenderDevice()->DrawSolidMesh(venusm);

	glDisable(GL_LIGHTING);
	GetGame()->GetRenderDevice()->SetColor(graphic::green);
	TravelBSPTree(root);

	DrawAxis();

	GetGame()->GetRenderDevice()->Flush();

	clock_t t2 = clock();
	dt = (t2 - t1) / 1000.0;
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