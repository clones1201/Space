#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Common/Log.h"
#include "Common/Utility.hpp"

#include "RenderSystem/DeviceResource.hpp"
#include "RenderSystem/RenderSystem.hpp"
#include "Space/Mesh.hpp"

#include <ppl.h>

namespace Space
{
	MeshPart::MeshPart()
	{
	}

	Mesh::Mesh()
	{
	}

	Mesh::Mesh(const tstring& filename)
		:m_FileName(filename)
	{
	}

	MeshPart* Mesh::CreatePart()
	{
		auto newPart = TypeTrait<MeshPart>::Ptr(new MeshPart());
		m_PartCollection.push_back(newPart);
		return newPart.get();
	}

	void Mesh::RemovePart(MeshPart* part)
	{
		for (
			auto iter = m_PartCollection.begin();
			iter != m_PartCollection.end();
			++iter)
		{
			if (iter->get() == part)
			{
				m_PartCollection.erase(iter);
				break;
			}
		}
	}
		
	void Mesh::_LoadAsync()
	{
		m_LoadAction = std::async(std::launch::async,
			[this] { this->_RealLoad(); });
	}

	void Mesh::_RealLoad()
	{
		tstring path = Core::GetInstance()->GetAssetsDir() + TEXT("/") + m_FileName;

		const aiScene* scene = aiImportFile(wstr2str(path).c_str(),
			aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded | aiProcess_TransformUVCoords);

		Float3 min, max;

		if (scene == nullptr || scene->mNumMeshes == 0)
		{
			aiReleaseImport(scene);
			throw std::exception("No Mesh in this file");
		}
		for (uint32 meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			MeshPart* part = CreatePart();
			aiMesh* pAiPart = scene->mMeshes[meshIndex];

			int32 numVertices = pAiPart->mNumVertices;
			int32 numFaces = pAiPart->mNumFaces;
			int32 numUVChannels = scene->mMeshes[meshIndex]->GetNumUVChannels();

			bool hasTangent = pAiPart->HasTangentsAndBitangents();
			if (!pAiPart->HasFaces()
				|| !pAiPart->HasNormals()
				|| !pAiPart->HasPositions()
			//	|| !pAiPart->HasTextureCoords(0)
			//	|| !pAiPart->HasTangentsAndBitangents()
				)
			{
				RemovePart(part);
				continue;
			};

			min = Float3((float*)&(pAiPart->mVertices[0]));
			max = Float3((float*)&(pAiPart->mVertices[0]));

			part->m_pInputLayout.reset(new Render::InputLayout{
				{Render::VertexElemType::Float3, Render::ElemSemantic::Position, 0, 0, Render::ElementClass::PerVertex, 0 },
				{Render::VertexElemType::Float3, Render::ElemSemantic::Normal, 0, 0, Render::ElementClass::PerVertex, 0 },
			});
			auto layout = part->m_pInputLayout;
			if (hasTangent)
			{
				layout->Insert(
					layout->End(),
					{Render::VertexElemType::Float3, Render::ElemSemantic::Tangent, 0, 0, Render::ElementClass::PerVertex, 0 });
			}
			for (int32 i = 0; i < numUVChannels; ++i)
				layout->Insert(layout->End(),
				{Render::VertexElemType::Float2, Render::ElemSemantic::TexCoord, (uint32)i, 0, Render::ElementClass::PerVertex, 0 });

			uint32 stride = layout->GetVertexStride(0);
			byte* vertices = new byte[stride * numVertices];
			uint16* indices = new uint16[3 * numFaces];

			auto elemPosition = layout->Begin();
			auto elemNormal = elemPosition + 1;
			auto elemTangent = elemNormal + 1;

#define Get(V,i,stride,offset) (V + stride * i + offset)
			for (int32 i = 0; i < numVertices; ++i)
			{
				*(Float3*)Get(vertices, i, stride, elemPosition->AlignedByteOffset) = Float3((float*)&(pAiPart->mVertices[i]));
				hasTangent ? (*(Float3*)Get(vertices, i, stride, elemTangent->AlignedByteOffset) = Float3((float*)&(pAiPart->mTangents[i]))) : Float3();
				*(Float3*)Get(vertices, i, stride, elemNormal->AlignedByteOffset) = Float3((float*)&(pAiPart->mNormals[i]));

				auto elemTexCoord = hasTangent ? elemTangent + 1 : elemNormal + 1;
				for (int32 j = 0; j < numUVChannels; ++j)
				{
					aiVector3D texcoord = pAiPart->mTextureCoords[j][i];
					*(Float2*)Get(vertices, i, stride, elemTexCoord->AlignedByteOffset) = Float2((float*)&(pAiPart->mTextureCoords[j][i]));
					++elemTexCoord;
				}
#undef GET(V, i, stride, offset)
#if	UV_MIRROR
				float t = dot(cross(scene->mMeshes[meshIndex]->mNormals[i], scene->mMeshes[meshIndex]->mTangents[i]), scene->mMeshes[meshIndex]->mBitangents[i]);
				if (t < 0)
					vertices[i].Tangent = float3(-vertices[i].Tangent.r, vertices[i].Tangent.g, vertices[i].Tangent.b);
#endif

				min.x = std::min(min.x, pAiPart->mVertices[i].x);
				min.y = std::min(min.y, pAiPart->mVertices[i].y);
				min.z = std::min(min.z, pAiPart->mVertices[i].z);
				max.x = std::max(max.x, pAiPart->mVertices[i].x);
				max.y = std::max(max.y, pAiPart->mVertices[i].y);
				max.z = std::max(max.z, pAiPart->mVertices[i].z);
			}
			part->m_Size = Float3(max.x - min.x, max.y - min.y, max.z - min.z);
			part->m_Center = Float3((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);

			for (long long i = 0; i < numFaces; ++i)
			{
				aiFace* face = &(scene->mMeshes[meshIndex]->mFaces[i]);
				indices[3 * i] = face->mIndices[0];
				indices[3 * i + 1] = face->mIndices[1];
				indices[3 * i + 2] = face->mIndices[2];
				assert(
					indices[3 * i] <= numVertices &&
					indices[3 * i + 1] <= numVertices &&
					indices[3 * i + 2] <= numVertices
				);
			}
			auto device = Render::Device::GetInstance();
			part->m_NumPrimitives = numFaces;
			part->m_pVertexBuffer.reset(new 
				Render::VertexBuffer(layout->GetVertexStride(0) * numVertices, layout->GetVertexStride(0), vertices));
			part->m_pIndexBuffer.reset(new
				Render::IndexBuffer(sizeof(uint16) * 3 * numFaces, Render::DataFormat::R16_UINT, (byte*)indices));

			delete [] vertices;
			delete [] indices;
		}

		aiReleaseImport(scene);
	}

	Mesh* Mesh::CreateFromFBX(tstring const& filename)
	{
		Mesh* ret = new Mesh(filename);
		ret->_RealLoad();
		return ret;
	}

	struct Vertex
	{
		Float3 position;
		Float3 normal;
		Float3 tangent;
		Float2 texcoord0;
		//	float2 texcoord1;
	};

	Mesh* Mesh::CreateCube(
		float width, float height, float depth)
	{
		HRESULT hr = S_OK;

		float hx = width / 2;
		float hy = height / 2;
		float hz = depth / 2;
		std::vector<Vertex> vertices =
		{
			//back
			{ { -hx, -hy, -hz },{ 0.0f,0.0f,-1.0f },{ -1.0f, 0.0f, 0.0f },{ 0.0f,0.0f } },
			{ { -hx,  hy, -hz },{ 0.0f,0.0f,-1.0f },{ -1.0f, 0.0f, 0.0f },{ 0.0f,1.0f } },
			{ { hx, -hy, -hz },{ 0.0f,0.0f,-1.0f },{ -1.0f, 0.0f, 0.0f },{ 1.0f,0.0f } },
			{ { hx,  hy, -hz },{ 0.0f,0.0f,-1.0f },{ -1.0f, 0.0f, 0.0f },{ 1.0f,1.0f } },
			//front
			{ { hx, -hy, hz },{ 0.0f,0.0f,1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f,0.0f } },
			{ { hx,  hy, hz },{ 0.0f,0.0f,1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f,1.0f } },
			{ { -hx, -hy, hz },{ 0.0f,0.0f,1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f,0.0f } },
			{ { -hx,  hy, hz },{ 0.0f,0.0f,1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f,1.0f } },
			//left
			{ { -hx, -hy, hz },{ -1.0f,0.0f,0.0f },{ 0.0f, -1.0f, 0.0f },{ 0.0f,0.0f } },
			{ { -hx,  hy, hz },{ -1.0f,0.0f,0.0f },{ 0.0f, -1.0f, 0.0f },{ 0.0f,1.0f } },
			{ { -hx, -hy, -hz },{ -1.0f,0.0f,0.0f },{ 0.0f, -1.0f, 0.0f },{ 1.0f,0.0f } },
			{ { -hx,  hy, -hz },{ -1.0f,0.0f,0.0f },{ 0.0f, -1.0f, 0.0f },{ 1.0f,1.0f } },
			//right
			{ { hx, -hy, -hz },{ 1.0f,0.0f,0.0f },{ 0.0f, -1.0f, 0.0f },{ 0.0f,0.0f } },
			{ { hx,  hy, -hz },{ 1.0f,0.0f,0.0f },{ 0.0f, -1.0f, 0.0f },{ 0.0f,1.0f } },
			{ { hx, -hy, hz },{ 1.0f,0.0f,0.0f },{ 0.0f, -1.0f, 0.0f },{ 1.0f,0.0f } },
			{ { hx,  hy, hz },{ 1.0f,0.0f,0.0f },{ 0.0f, -1.0f, 0.0f },{ 1.0f,1.0f } },
			//up
			{ { -hx, hy, -hz },{ 0.0f,1.0f,0.0f },{ 1.0f, 0.0f, 0.0f }	,{ 0.0f,0.0f } },
			{ { -hx, hy, hz },{ 0.0f,1.0f,0.0f },{ 1.0f, 0.0f, 0.0f }	,{ 0.0f,1.0f } },
			{ { hx, hy, -hz },{ 0.0f,1.0f,0.0f },{ 1.0f, 0.0f, 0.0f }	,{ 1.0f,0.0f } },
			{ { hx, hy, hz },{ 0.0f,1.0f,0.0f },{ 1.0f, 0.0f, 0.0f }	,{ 1.0f,1.0f } },
			//bottom
			{ { -hx, -hy, hz },{ 0.0f,-1.0f,0.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f,0.0f } },
			{ { -hx, -hy, -hz },{ 0.0f,-1.0f,0.0f },{ 1.0f,0.0f, 0.0f } ,{ 0.0f,1.0f } },
			{ { hx, -hy, hz },{ 0.0f,-1.0f,0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f,0.0f } },
			{ { hx, -hy, -hz },{ 0.0f,-1.0f,0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f,1.0f } },
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
			auto ret = new Mesh;
			MeshPart* part = ret->CreatePart();
			part->m_pInputLayout.reset(new Render::InputLayout{
				{ Render::VertexElemType::Float3, Render::ElemSemantic::Position, 0, 0, Render::ElementClass::PerVertex, 0 },
				{ Render::VertexElemType::Float3, Render::ElemSemantic::Normal, 0, 0, Render::ElementClass::PerVertex, 0 },
				{ Render::VertexElemType::Float3, Render::ElemSemantic::Tangent, 0, 0, Render::ElementClass::PerVertex, 0 },
				{ Render::VertexElemType::Float2, Render::ElemSemantic::TexCoord, 0, 0, Render::ElementClass::PerVertex, 0 }
			});
			auto layout = part->m_pInputLayout;
			part->m_Size = Float3(hx, hy, hz);
			part->m_Center = Float3(0.0f, 0.0f, 0.0f);

			size_t numVertices = vertices.size();
			size_t numFaces = indices.size() / 3;

			auto device = Render::Device::GetInstance();
			part->m_NumPrimitives = numFaces;
			part->m_pVertexBuffer.reset(new
				Render::VertexBuffer(layout->GetVertexStride(0) * numVertices, layout->GetVertexStride(0), reinterpret_cast<byte const*>(vertices.data())));
			part->m_pIndexBuffer.reset(new
				Render::IndexBuffer(sizeof(uint16) * 3 * numFaces, Render::DataFormat::R16_UINT, reinterpret_cast<byte const*>(indices.data())));

			return ret;
		}
		catch (std::exception)
		{
			return nullptr;
		}
	}

	Mesh* Mesh::CreateSphere(
		float radius, int32_t segments)
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
			auto ret = new Mesh;
			MeshPart* part = ret->CreatePart();
			part->m_pInputLayout.reset(new Render::InputLayout{
				{ Render::VertexElemType::Float3, Render::ElemSemantic::Position, 0, 0, Render::ElementClass::PerVertex, 0 },
				{ Render::VertexElemType::Float3, Render::ElemSemantic::Normal, 0, 0, Render::ElementClass::PerVertex, 0 },
				{ Render::VertexElemType::Float3, Render::ElemSemantic::Tangent, 0, 0, Render::ElementClass::PerVertex, 0 },
				{ Render::VertexElemType::Float2, Render::ElemSemantic::TexCoord, 0, 0, Render::ElementClass::PerVertex, 0 }
			});
			auto layout = part->m_pInputLayout;
			part->m_Size = Float3(radius * 2.0f, radius * 2.0f, radius * 2.0f);
			part->m_Center = Float3(0.0f, 0.0f, 0.0f);

			size_t numVertices = vertices.size();
			size_t numFaces = indices.size() / 3;

			auto device = Render::Device::GetInstance();
			part->m_NumPrimitives = numFaces;
			part->m_pVertexBuffer.reset(new
				Render::VertexBuffer(layout->GetVertexStride(0) * numVertices, layout->GetVertexStride(0), reinterpret_cast<byte const*>(vertices.data())));
			part->m_pIndexBuffer.reset(new
				Render::IndexBuffer(sizeof(uint16) * 3 * numFaces, Render::DataFormat::R16_UINT, reinterpret_cast<byte const*>(indices.data())));

			return ret;
		}
		catch (std::exception)
		{
			return nullptr;
		}
	}

	Mesh* Mesh::CreateCone(
		float radius, float height, int32_t segments)
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
			auto ret = new Mesh;
			MeshPart* part = ret->CreatePart();
			part->m_pInputLayout.reset(new Render::InputLayout{
				{ Render::VertexElemType::Float3, Render::ElemSemantic::Position, 0, 0, Render::ElementClass::PerVertex, 0 },
				{ Render::VertexElemType::Float3, Render::ElemSemantic::Normal, 0, 0, Render::ElementClass::PerVertex, 0 },
				{ Render::VertexElemType::Float3, Render::ElemSemantic::Tangent, 0, 0, Render::ElementClass::PerVertex, 0 },
				{ Render::VertexElemType::Float2, Render::ElemSemantic::TexCoord, 0, 0, Render::ElementClass::PerVertex, 0 }
			});
			auto layout = part->m_pInputLayout;
			part->m_Size = Float3(radius * 2.0f, height, radius * 2.0f);
			part->m_Center = Float3(0.0f, 0.0f, 0.0f);

			size_t numVertices = vertices.size();
			size_t numFaces = indices.size() / 3;

			auto device = Render::Device::GetInstance();
			part->m_NumPrimitives = numFaces;
			part->m_pVertexBuffer.reset(new
				Render::VertexBuffer(layout->GetVertexStride(0) * numVertices, layout->GetVertexStride(0), reinterpret_cast<byte const*>(vertices.data())));
			part->m_pIndexBuffer.reset(new
				Render::IndexBuffer(sizeof(uint16) * 3 * numFaces, Render::DataFormat::R16_UINT, reinterpret_cast<byte const*>(indices.data())));

			return ret;
		}
		catch (std::exception)
		{
			return nullptr;
		}
	}

	Mesh* Mesh::CreateQuad(
		float width, float height)
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
			auto ret = new Mesh;
			MeshPart* part = ret->CreatePart();
			part->m_pInputLayout.reset(new Render::InputLayout{
				{ Render::VertexElemType::Float3, Render::ElemSemantic::Position, 0, 0, Render::ElementClass::PerVertex, 0 },
				{ Render::VertexElemType::Float3, Render::ElemSemantic::Normal, 0, 0, Render::ElementClass::PerVertex, 0 },
				{ Render::VertexElemType::Float3, Render::ElemSemantic::Tangent, 0, 0, Render::ElementClass::PerVertex, 0 },
				{ Render::VertexElemType::Float2, Render::ElemSemantic::TexCoord, 0, 0, Render::ElementClass::PerVertex, 0 }
			});
			auto layout = part->m_pInputLayout;
			part->m_Size = Float3(width, height, 0.0f);
			part->m_Center = Float3(0.0f, 0.0f, 0.0f);

			size_t numVertices = vertices.size();
			size_t numFaces = indices.size() / 3;

			part->m_NumPrimitives = numFaces;
			part->m_pVertexBuffer.reset(new
				Render::VertexBuffer(layout->GetVertexStride(0) * numVertices, layout->GetVertexStride(0), reinterpret_cast<byte const*>(vertices.data())));
			part->m_pIndexBuffer.reset(new
				Render::IndexBuffer(sizeof(uint16) * 3 * numFaces, Render::DataFormat::R16_UINT, reinterpret_cast<byte const*>(indices.data())));

			return ret;
		}
		catch (std::exception)
		{
			return nullptr;
		}
	}

}