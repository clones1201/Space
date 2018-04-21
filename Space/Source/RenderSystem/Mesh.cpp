#include "RenderSystem/Mesh.hpp"
#include "RenderSystem/Rendering.hpp"
#include "RenderSystem/DeviceResource.hpp"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace Space
{
	namespace Render
	{
		struct Vertex
		{
			Float3 position;
			Float3 normal;
			Float3 tangent;
			Float2 texcoord0;
			//	float2 texcoord1;
		};

		std::shared_ptr<Mesh> Mesh::CreateCube(
			Device* pDevice, float width, float height, float depth)
		{
			HRESULT hr = S_OK;
			
			float hx = width / 2;
			float hy = height / 2;
			float hz = depth / 2;
			std::vector<Vertex> vertices =
			{
				//back
				{ { -hx, -hy, -hz },{ 0.0f,0.0f,-1.0f },{ -1.0f, 0.0f, 0.0f },{0.0f,0.0f}},
				{ { -hx,  hy, -hz },{ 0.0f,0.0f,-1.0f },{ -1.0f, 0.0f, 0.0f },{0.0f,1.0f}},
				{ {  hx, -hy, -hz },{ 0.0f,0.0f,-1.0f },{ -1.0f, 0.0f, 0.0f },{1.0f,0.0f}},
				{ {  hx,  hy, -hz },{ 0.0f,0.0f,-1.0f },{ -1.0f, 0.0f, 0.0f },{1.0f,1.0f}},
				//front
				{ { hx, -hy, hz },{ 0.0f,0.0f,1.0f },	{ 1.0f, 0.0f, 0.0f },{ 0.0f,0.0f } },
				{ { hx,  hy, hz },{ 0.0f,0.0f,1.0f },	{ 1.0f, 0.0f, 0.0f },{ 0.0f,1.0f } },
				{ { -hx, -hy, hz },{ 0.0f,0.0f,1.0f },	{ 1.0f, 0.0f, 0.0f },{ 1.0f,0.0f } },
				{ { -hx,  hy, hz },{ 0.0f,0.0f,1.0f },	{ 1.0f, 0.0f, 0.0f },{ 1.0f,1.0f } },
				//left
				{ { -hx, -hy, hz },{ -1.0f,0.0f,0.0f },	{ 0.0f, -1.0f, 0.0f },{ 0.0f,0.0f } },
				{ { -hx,  hy, hz },{ -1.0f,0.0f,0.0f },	{ 0.0f, -1.0f, 0.0f },{ 0.0f,1.0f } },
				{ { -hx, -hy, -hz },{ -1.0f,0.0f,0.0f },{ 0.0f, -1.0f, 0.0f },{ 1.0f,0.0f } },
				{ { -hx,  hy, -hz },{ -1.0f,0.0f,0.0f },{ 0.0f, -1.0f, 0.0f },{ 1.0f,1.0f } },
				//right
				{ { hx, -hy, -hz },{ 1.0f,0.0f,0.0f }, { 0.0f, -1.0f, 0.0f },{ 0.0f,0.0f } },
				{ { hx,  hy, -hz },{ 1.0f,0.0f,0.0f }, { 0.0f, -1.0f, 0.0f },{ 0.0f,1.0f } },
				{ { hx, -hy, hz }, { 1.0f,0.0f,0.0f }, { 0.0f, -1.0f, 0.0f },{ 1.0f,0.0f } },
				{ { hx,  hy, hz }, { 1.0f,0.0f,0.0f }, { 0.0f, -1.0f, 0.0f },{ 1.0f,1.0f } },
				//up
				{ { -hx, hy, -hz },{ 0.0f,1.0f,0.0f },	{ 1.0f, 0.0f, 0.0f }	,{ 0.0f,0.0f } },
				{ { -hx, hy, hz },{ 0.0f,1.0f,0.0f },	{ 1.0f, 0.0f, 0.0f }	,{ 0.0f,1.0f } },
				{ { hx, hy, -hz },{ 0.0f,1.0f,0.0f },	{ 1.0f, 0.0f, 0.0f }	,{ 1.0f,0.0f } },
				{ { hx, hy, hz },{ 0.0f,1.0f,0.0f },	{  1.0f, 0.0f, 0.0f }	,{ 1.0f,1.0f } },
				//bottom
				{ { -hx, -hy, hz },{ 0.0f,-1.0f,0.0f },	{ 1.0f, 0.0f, 0.0f },{ 0.0f,0.0f } },
				{ { -hx, -hy, -hz },{ 0.0f,-1.0f,0.0f },{ 1.0f,0.0f, 0.0f } ,{ 0.0f,1.0f } },
				{ { hx, -hy, hz },{ 0.0f,-1.0f,0.0f },	{ 1.0f, 0.0f, 0.0f },{ 1.0f,0.0f } },
				{ { hx, -hy, -hz },{ 0.0f,-1.0f,0.0f },	{ 1.0f, 0.0f, 0.0f },{ 1.0f,1.0f } },
			};

			std::vector<uint16_t> indices =
			{
				0,1,2,2,1,3,
				4,5,6,6,5,7,
				8,9,10,10,9,11,
				12,13,14,14,13,15,
				16,17,18,18,17,19,
				20,21,22,22,21,23,
			};

			try
			{
				return std::make_shared<Mesh>(pDevice,
					vertices.size(), sizeof(Vertex), (byte*)vertices.data(),
					indices.size(), sizeof(uint16_t), (byte*)indices.data());
			}
			catch (std::exception)
			{
				return std::shared_ptr<Mesh>();
			}
		}

		std::shared_ptr<Mesh> Mesh::CreateSphere(
			Device* pDevice, float radius, int32_t segments)
		{
			HRESULT hr = S_OK;
			
			std::vector<Vertex> vertices;
			vertices.reserve((segments + 1) * segments);
			std::vector<uint16_t> indices;
			indices.reserve(3 * 2 * segments * segments);

			auto CreateSphereVertex = [radius, segments](int i, int j) -> Vertex
			{
				i = std::min(segments, std::max(0, i));
				j = std::min(segments, std::max(0, j));
				float theta = j * (XM_PI / segments);
				float phi = i * (2 * XM_PI / segments);

				XMVECTOR position = XMVectorSet(
					radius * cos(phi) * sin(theta),
					radius * cos(theta),
					radius * sin(phi) * sin(theta),
					1.0f);

				XMVECTOR normal = XMVector3Normalize(position);
				XMVECTOR bitangent = XMVectorSet(
					cos(phi) * sin(theta),
					0.0f,
					sin(phi) * sin(theta),
					0.0f);
				XMVECTOR tangent = XMVector3Cross(bitangent, normal);

				Vertex ret;
				XMStoreFloat3(&ret.position, position);
				XMStoreFloat3(&ret.normal, normal);
				XMStoreFloat3(&ret.tangent, tangent);
				ret.texcoord0 = { i * 1.0f / segments, j * 1.0f / segments };
				return ret;
			};

			for (int i = 0; i < segments; i++)
			{
				vertices.push_back(CreateSphereVertex(i, 0));
			}
			vertices.push_back(CreateSphereVertex(0, 1));
			for (int i = 0; i < segments; i++)
			{
				vertices.push_back(CreateSphereVertex(1 + i, 1));
				indices.push_back(i);
				indices.push_back(i + 1 + segments);
				indices.push_back(i + segments);
			}

			for (int j = 2; j < segments; j++)
			{
				vertices.push_back(CreateSphereVertex(0, j));
				for (int i = 0; i < segments; i++)
				{
					vertices.push_back(CreateSphereVertex(1 + i, j));
					indices.push_back(i + (j - 1) * segments + (j - 2));
					indices.push_back(i + 1 + j * segments + (j - 1));
					indices.push_back(i + j * segments + (j - 1));

					indices.push_back(i + (j - 1) * segments + (j - 2));
					indices.push_back(i + 1 + (j - 1) * segments + (j - 2));
					indices.push_back(i + 1 + j * segments + j - 1);
				}
			}

			for (int i = 0; i < segments; i++)
			{
				vertices.push_back(CreateSphereVertex(i, segments));
				indices.push_back(i + (segments - 1) * segments + (segments - 2));
				indices.push_back(i + 1 + (segments - 1) * segments + (segments - 2));
				indices.push_back(i + segments * segments + (segments - 1));
			}
			try
			{
				return std::make_shared<Mesh>(pDevice,
					vertices.size(), sizeof(Vertex), (byte*)vertices.data(),
					indices.size(), sizeof(uint16_t), (byte*)indices.data());
			}
			catch (std::exception)
			{
				return std::shared_ptr<Mesh>();
			}
		}

		std::shared_ptr<Mesh> Mesh::CreateCone(
			Device* pDevice, float radius, float height, int32_t segments)
		{
			HRESULT hr = S_OK;
			
			if (segments <= 3) {
				return nullptr;
			}

			std::vector<Vertex> vertices;
			vertices.reserve(3 * segments + 1);
			std::vector<uint16_t> indices;
			indices.reserve(3 * 2 * segments);

			for (int i = 0; i < segments; i++)
			{
				float phi = ((float)i + 0.5f) * 2 * XM_PI / segments;
				Vertex v;
				auto Normal = XMVector3Normalize(XMVectorSet(cos(phi), radius / height, sin(phi), 0.0f));
				auto Bitangent = XMVector3Normalize(XMVectorSet(cos(phi), 0.0f, sin(phi), 0.0f));
				auto Tangent = XMVector3Normalize(XMVector3Cross(Bitangent, Normal));
				v.position = { 0.0f, height, 0.0f };
				XMStoreFloat3(&v.normal, Normal);
				XMStoreFloat3(&v.tangent, Tangent);
				v.texcoord0 = { i * (1.0f / segments), 0.0f };
				vertices.push_back(v);
			}

			Vertex v;
			auto Normal = XMVector3Normalize(XMVectorSet(1.0f, radius / height, 0.0f, 0.0f));
			auto Bitangent = XMVector3Normalize(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
			auto Tangent = XMVector3Cross(Bitangent, Normal);
			v.position = { radius, 0.0f, 0.0f };
			XMStoreFloat3(&v.normal, Normal);
			XMStoreFloat3(&v.tangent, Tangent);
			v.texcoord0 = { 0.0f, 0.0f };
			vertices.push_back(v);
			for (int i = 0; i < segments; i++)
			{
				float phi = (i + 1) * 2 * XM_PI / segments;
				Vertex v;
				auto Normal = XMVector3Normalize(XMVectorSet(cos(phi), radius / height, sin(phi), 0.0f));
				auto Bitangent = XMVector3Normalize(XMVectorSet(cos(phi), 0.0f, sin(phi), 0.0f));
				auto Tangent = XMVector3Cross(Bitangent, Normal);
				v.position = { radius * cos(phi), 0.0f, radius * sin(phi) };
				XMStoreFloat3(&v.normal, Normal);
				XMStoreFloat3(&v.tangent, Tangent);
				v.texcoord0 = { i * (1.0f / segments), 0.0f };
				vertices.push_back(v);

				indices.push_back(i);
				indices.push_back(i + 1 + segments);
				indices.push_back(i + segments);
			}

			for (int i = 0; i < segments; i++)
			{
				float phi = ((float)i + 0.5f) * 2 * XM_PI / segments;
				Vertex v;
				auto Normal = XMVector3Normalize(XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));
				auto Bitangent = XMVector3Normalize(XMVectorSet(-cos(phi), 0.0f, -sin(phi), 0.0f));
				auto Tangent = XMVector3Cross(Bitangent, Normal);
				v.position = { 0.0f, 0.0f, 0.0f };
				XMStoreFloat3(&v.normal, Normal);
				XMStoreFloat3(&v.tangent, Tangent);
				v.texcoord0 = { 0.0f, 0.0f };
				vertices.push_back(v);
				indices.push_back(i + segments);
				indices.push_back(i + 1 + segments);
				indices.push_back(i + 1 + 2 * segments);
			}

			try
			{
				return std::make_shared<Mesh>(pDevice,
					vertices.size(), sizeof(Vertex), (byte*)vertices.data(),
					indices.size(), sizeof(uint16_t), (byte*)indices.data());
			}
			catch (std::exception)
			{
				return std::shared_ptr<Mesh>();
			}
		}

		std::shared_ptr<Mesh> Mesh::CreateQuad(
			Device* device, float width, float height)
		{
			HRESULT hr = S_OK;

			float hx = width / 2.0f;
			float hy = height / 2.0f;

			std::vector<Vertex> vertices =
			{
				{ { -hx, -hy, 0.0f },{ 0.0f,0.0f,-1.0f },{ -1.0f, 0.0f, 0.0f },{ 0.0f,0.0f } },
				{ { -hx,  hy, 0.0f },{ 0.0f,0.0f,-1.0f },{ -1.0f, 0.0f, 0.0f },{ 0.0f,1.0f } },
				{ { hx, -hy, 0.0f },{ 0.0f,0.0f,-1.0f },{ -1.0f, 0.0f, 0.0f },{ 1.0f,0.0f } },
				{ { hx,  hy, 0.0f },{ 0.0f,0.0f,-1.0f },{ -1.0f, 0.0f, 0.0f },{ 1.0f,1.0f } }
			};
			std::vector<uint16_t> indices =
			{
				0,1,2,2,1,3,
			};

			try
			{
				return std::make_shared<Mesh>(device,
					vertices.size(), sizeof(Vertex), (byte*)vertices.data(),
					indices.size(), sizeof(uint16_t), (byte*)indices.data());
			}
			catch (std::exception)
			{
				return std::shared_ptr<Mesh>();
			}
		}
		std::shared_ptr<Mesh> Mesh::CreateFromFile(
			Device* device, char const* filename)
		{
			HRESULT hr = S_OK;

			const aiScene* scene = aiImportFile(filename, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
			if (scene == nullptr)
				return std::shared_ptr<Mesh>();

			aiMesh* mesh = scene->mMeshes[0];

			int numVertices = mesh->mNumVertices;
			int numFaces = mesh->mNumFaces;

			std::vector<Vertex> vertices;
			vertices.reserve(numVertices);
			std::vector<uint16_t> indices;
			indices.reserve(3 * numFaces);

			for (int i = 0; i < numVertices; ++i)
			{
				Vertex v = {
					{ mesh->mVertices[i].x, mesh->mVertices[i].y,  mesh->mVertices[i].z },
					{ mesh->mNormals[i].x,  mesh->mNormals[i].y,   mesh->mNormals[i].z },
					{ mesh->mTangents[i].x, mesh->mTangents[i].y,  mesh->mTangents[i].z},
					{ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y }
				};
				vertices.push_back(v);
			}

			for (int i = 0; i < numFaces; ++i)
			{
				aiFace* face = &(mesh->mFaces[i]);
				indices.push_back(face->mIndices[0]);
				indices.push_back(face->mIndices[1]);
				indices.push_back(face->mIndices[2]);
			}

			aiReleaseImport(scene);
			try
			{
				return std::make_shared<Mesh>(device,
					vertices.size(), sizeof(Vertex), (byte*)vertices.data(),
					indices.size(), sizeof(uint16_t), (byte*)indices.data());
			}
			catch (std::exception)
			{
				return std::shared_ptr<Mesh>();
			}
		}

		Mesh::Mesh(Device* device,
			size_t vertexCount, size_t sizeOfVertex, byte const* pVerticesInitialData,
			size_t indexCount, size_t sizeOfIndex, byte const* pInticesInitialData
		)
			:_IndexCount(indexCount), _VertexCount(vertexCount)
		{
			HRESULT hr = S_OK;

			UINT lengthOfVertices = UINT(vertexCount * sizeOfVertex);
			_pVertexBuffer = std::make_unique<VertexBuffer>(
				device->GetDevice(), lengthOfVertices, sizeOfVertex, pVerticesInitialData);
			
			UINT lengthOfIndices = UINT(indexCount * sizeOfIndex);
			_pIndexBuffer = std::make_unique<IndexBuffer>(
				device->GetDevice(), lengthOfIndices, DataFormat::R16_UINT, pInticesInitialData);

		}

		Mesh::~Mesh()
		{
		}

		void Mesh::Render(CommandList* pList)
		{
			pList->SetVertexBuffers(0, 
			{ _pVertexBuffer->GetBuffer(), }, { (int32)_pVertexBuffer->GetStride(), }, {0,});			
			pList->SetIndexBuffer(_pIndexBuffer->GetBuffer(), _pIndexBuffer->GetFormat(), 0);
			pList->DrawIndexed(0, (UINT)_IndexCount / 3);
		}

	}
}