#pragma once

#include "Prerequisites.hpp"

namespace Space
{
	namespace Render
	{
		class Mesh: public Uncopyable
		{
		public:
			static std::shared_ptr<Mesh> CreateCube(
				Device* pDevice, float width, float height, float depth);
			static std::shared_ptr<Mesh> CreateSphere(
				Device* pDevice, float radius, int32_t segments);
			static std::shared_ptr<Mesh> CreateCone(
				Device* pDevice, float radius, float height, int32_t segments);
			static std::shared_ptr<Mesh> CreateQuad(
				Device* pDevice, float width, float height);
			std::shared_ptr<Mesh> CreateFromFile(
				Device* device, char const* filename);

			void Render(CommandList* pList);
			
			Mesh(Device* device,
				size_t vertexCount, size_t sizeOfVertex, byte const* pVerticesInitialData,
				size_t indexCount, size_t sizeOfIndex, byte const* pInticesInitialData);
			
			virtual ~Mesh();
		private:
			std::unique_ptr<VertexBuffer> _pVertexBuffer;
			std::unique_ptr<IndexBuffer> _pIndexBuffer;
			size_t _VertexCount = 0;
			size_t _IndexCount = 0;
		};

	}
}
