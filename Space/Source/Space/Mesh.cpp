#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Common/Log.h"
#include "Common/Utility.hpp"

#include "RenderSystem/DeviceResource.hpp"
#include "Space/Mesh.hpp"

namespace Space
{
	MeshPart::MeshPart()
	{
	}

	Mesh::Mesh()
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
	
	Mesh* Mesh::CreateFromFBX(RenderSystem* pRenderSys, tstring const& filename)
	{ 
		try
		{
			tstring path = Core::GetInstance()->GetAssetsDir() + TEXT("/Models/") + filename;

			const aiScene* scene = aiImportFile(wstr2str(path).c_str(),
				aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded | aiProcess_TransformUVCoords);

			Float3 min, max;

			if (scene == nullptr || scene->mNumMeshes == 0)
				throw std::exception("No Mesh in this file");

			Mesh* ret = new Mesh();

			for (uint32 meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
			{
				MeshPart* part = ret->CreatePart();
				aiMesh* pAiPart = scene->mMeshes[meshIndex];

				int32 numVertices = pAiPart->mNumVertices;
				int32 numFaces = pAiPart->mNumFaces;
				int32 numUVChannels = scene->mMeshes[meshIndex]->GetNumUVChannels();

				if (!pAiPart->HasFaces()
					|| !pAiPart->HasNormals()
					|| !pAiPart->HasPositions()
					|| !pAiPart->HasTextureCoords(0)
					|| !pAiPart->HasTangentsAndBitangents())
				{
					ret->RemovePart(part);
					continue;
				};

				min = Float3((float*)&(pAiPart->mVertices[0]));
				max = Float3((float*)&(pAiPart->mVertices[0]));

				part->m_pInputLayout.reset(new InputLayout{
					{ VertexElemType::Float3, ElemSemantic::Position, 0, 0, ElementClass::PerVertex, 0 },
					{ VertexElemType::Float3, ElemSemantic::Normal, 0, 0, ElementClass::PerVertex, 0 },
					{ VertexElemType::Float3, ElemSemantic::Tangent, 0, 0, ElementClass::PerVertex, 0 },
				});
				auto layout = part->m_pInputLayout;
				for (int32 i = 0; i < numUVChannels; ++i)
					layout->Insert(layout->End(),
					{ VertexElemType::Float2, ElemSemantic::TexCoord, (uint32)i, 0, ElementClass::PerVertex, 0 });

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
					*(Float3*)Get(vertices, i, stride, elemTangent->AlignedByteOffset) = Float3((float*)&(pAiPart->mTangents[i]));
					*(Float3*)Get(vertices, i, stride, elemNormal->AlignedByteOffset) = Float3((float*)&(pAiPart->mNormals[i]));

					auto elemTexCoord = elemTangent + 1;
					for (int32 j = 0; j < numUVChannels; ++j)
					{
						aiVector3D texcoord = pAiPart->mTextureCoords[j][i];
						*(Float2*)Get(vertices, i, stride, elemTexCoord->AlignedByteOffset) = Float2((float*)&(pAiPart->mTextureCoords[j][i]));
						++elemTexCoord;
					}

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
				part->m_Size = std::max(max.x - min.x, std::max(max.y - min.y, max.z - min.z));
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

				part->m_NumPrimitives = numFaces;
				part->m_pVertexBuffer.reset(
					VertexBuffer::Create(pRenderSys, vertices, layout->GetVertexStride(0) * numVertices, layout->GetVertexStride(0)));
				part->m_pIndexBuffer.reset(
					IndexBuffer::Create(pRenderSys, (byte*)indices, sizeof(uint16) * 3 * numFaces, DataFormat::R16_UINT));

				delete vertices;
				delete indices;
			}

			aiReleaseImport(scene);
			return ret;
		}
		catch (std::exception& e)
		{
			Log(e.what());
			return nullptr;
		}
	}
	

}