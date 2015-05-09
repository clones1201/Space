#include "Space.hpp"
#include "Shader.hpp"

#ifdef SPACE_GAME
using namespace Space;

// 1. the return type (bool) is only valid if T is an integral type:
template <class T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
is_odd(T i) { return bool(i % 2); }

// 2. the second template argument is only valid if T is an integral type:
template < class T,
class = typename std::enable_if<std::is_integral<T>::value>::type>
	bool is_even(T i) { return !bool(i % 2); }

int main(int argc, char* argv[])
{
	{
		/*std::fstream tempFile("C:/Code/GitHub/Space/Assets/Material/default/0.hlsl.ps.cso", std::ios_base::in | std::ios_base::binary);
		if (!tempFile.is_open())
		{
			throw std::exception("Missing Template File");
		}
		std::istreambuf_iterator<char> beg(tempFile), end;
		std::vector<char> code(beg, end);*/

		auto renderSys = TypeTrait<RenderSystem>::Ptr(Core::CreateD3DRenderSystem());

		std::unique_ptr<Material> pMaterial;
		pMaterial.reset(Material::Create(renderSys.get(), "default"));
	}
	_CrtDumpMemoryLeaks();
	return 0;
}
#endif