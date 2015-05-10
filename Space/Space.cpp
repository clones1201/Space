#include "Space.hpp"
#include "Shader.hpp"
#ifdef SPACE_GAME
using namespace Space;

int main(int argc, char* argv[])
{
	{
		auto renderSys = TypeTrait<RenderSystem>::Ptr(Core::CreateD3DRenderSystem());

		std::unique_ptr<Material> pMaterial;
		pMaterial.reset(Material::Create(renderSys.get(), "default"));
	}
	_CrtDumpMemoryLeaks();
	return 0;
}
#endif