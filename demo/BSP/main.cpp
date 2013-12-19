#include "Space.h"
#include <Windows.h>
#include <iostream>

#include "RayTracing.h"

using namespace space;
using namespace util;

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

void BuildTree(BTreeNode **node,uint depth){
	if (depth == 0){
		*node = (BTreeNode*)(new BTreeLeaf(depth, 0.1f));
		return;
	}

	*node = new BTreeNode(depth);

	BTreeNode *left = nullptr, *right = nullptr;
	BuildTree(&left,depth-1);
	BuildTree(&right,depth-1);
	(*node)->SetChildren(*left,*right);

}

void TravelTree(BTreeNode* node){
	

	if (!node->isLeaf()){
		std::cout << node->GetElem() << " ";
		TravelTree((BTreeNode*)node->GetLeft());
		TravelTree((BTreeNode*)node->GetRight());
	}
	else{
		std::cout << node->GetElem() << " ";
		std::cout << ((BTreeLeaf*)node)->leafelem << " " << endl;
	}
}

int main(){

	BTreeNode* root = nullptr;

	BuildTree(&root, 3);

	TravelTree(root);

	std::cout << "press any key to continue..." << endl;
	std::cin.get();
}