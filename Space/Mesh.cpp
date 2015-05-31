#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Log.h"
#include "Utility.hpp"

#include "DeviceBuffer.hpp"
#include "InputLayout.hpp"
#include "Mesh.hpp"

namespace Space
{
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

	Mesh* Mesh::CreateFromFBX(RenderSystem* pRenderSys, std::wstring const& filename)
	{
		return CreateFromFBX(pRenderSys,wstr2str(filename));
	}

	Mesh* Mesh::CreateFromFBX(RenderSystem* pRenderSys, std::string const& filename)
	{ 
		const aiScene* scene = aiImportFile(filename.c_str(), 
			aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded | aiProcess_TransformUVCoords);

		Float3 min, max;

		if (scene == nullptr || scene->mNumMeshes)
			return nullptr;

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
						
			part->m_pInputLayout.reset(InputLayout::Create(pRenderSys));
			auto layout = part->m_pInputLayout;

			layout->AddElem(VET_Float3, ES_Position);
			layout->AddElem(VET_Float3, ES_Normal);
			layout->AddElem(VET_Float3, ES_Tangent);
			for (int32 i = 0; i < numUVChannels; ++i)
				layout->AddElem(VET_Float2, ES_TexCoord);
			
			uint32 stride = layout->GetVertexSize();
			byte* vertices = new byte[stride * numVertices];
			uint16* indices = new uint16[3 * numFaces];

#define Get(V,i,stride,offset) (V[stride * i] + offset)
			for (int32 i = 0; i < numVertices; ++i)
			{
				*(Float3*)Get(vertices, i, stride, layout->GetOffsetByIndex(0)) = Float3((float*)&(pAiPart->mVertices[i]));
				*(Float3*)Get(vertices, i, stride, layout->GetOffsetByIndex(1)) = Float3((float*)&(pAiPart->mTangents[i]));
				*(Float3*)Get(vertices, i, stride, layout->GetOffsetByIndex(2)) = Float3((float*)&(pAiPart->mNormals[i]));
				
				for (int32 j = 0; j < numUVChannels; ++j)
				{
					aiVector3D texcoord = pAiPart->mTextureCoords[j][i];
					*(Float2*)Get(vertices, i, stride, layout->GetOffsetByIndex(3)) = Float2((float*)&(pAiPart->mTextureCoords[j][i]));
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
			}
		 
			part->m_pVertexBuffer.reset(VertexBuffer::Create(pRenderSys, vertices, layout->GetVertexSize() * numVertices));
			part->m_pIndexBuffer.reset(IndexBuffer::Create(pRenderSys, (byte*)indices, sizeof(uint16) * 3 * numFaces));

			delete vertices;
			delete indices;
		}
		 
		aiReleaseImport(scene); 
	}
	

}