#include "Space.hpp"

#ifdef SPACE_GAME
using namespace Space;

int main(int argc, char* argv[])
{
	RenderSystem* pRenderSystem = Core::CreateD3DRenderSystem();

	std::wstring path = L"./Assets/Models/";
	path += L"0006_body.FBX";
	Mesh* pMesh = Mesh::CreateFromFBX(pRenderSystem, path);
}
#endif