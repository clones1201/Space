#include "Space.hpp"
#include "Shader.hpp"
#ifdef SPACE_GAME
using namespace Space;

int main(int argc, char* argv[])
{
	{
		auto renderSys = std::shared_ptr<RenderSystem>(Core::CreateD3DRenderSystem());

		auto pMaterial = std::unique_ptr<Material>(Material::Create(renderSys.get(), "default"));
	}
	_CrtDumpMemoryLeaks();
	return 0;
}
#endif