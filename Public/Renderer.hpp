#ifndef __SPACE_RENDERER_HPP__
#define __SPACE_RENDERER_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	class Renderer : virtual public Uncopyable
	{
	public:
		void SetDestTexture2D(TypeTrait<DeviceTexture2D>::Ptr pTexture);
	};

	class MaterialRenderer : public Renderer
	{
	public:
		
		void SetMaterial();
	};

	class MeshMaterialRenderer : public MaterialRenderer
	{
	public:
		TypeTrait<Mesh>::Ptr GetMesh() const;
		void SetMesh(TypeTrait<Mesh>::Ptr pMesh);
	};


}

#endif