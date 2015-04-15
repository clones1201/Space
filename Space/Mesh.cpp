#include "Log.h"
#include "Utility.h"

#include "DeviceBuffer.hpp"
#include "Mesh.hpp"

#include "fbxsdk.h"
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp/postprocess.h"

namespace Space
{
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

//		for (int32 i = 0; i < scene->mNumMeshes; ++i)
//		{
//		int32 numVertices = scene->mMeshes[meshIndex]->mNumVertices;
//		int32 numFaces = scene->mMeshes[meshIndex]->mNumFaces;
//
//		min = Float3((float*)&(scene->mMeshes[meshIndex]->mVertices[0]));
//		max = Float3((float*)&(scene->mMeshes[meshIndex]->mVertices[0]));
//		if (!scene->mMeshes[meshIndex]->mVertices
//			|| !scene->mMeshes[meshIndex]->mNormals
//			|| !scene->mMeshes[meshIndex]->mTangents
//			|| !scene->mMeshes[meshIndex]->mTextureCoords[0])
//			return nullptr;
//
//		Vertex* vertices = new Vertex[numVertices];
//		uint32* indices = new uint32[3 * numFaces];
//
//		for (uint64 i = 0; i < numVertices; ++i)
//		{
//			vertices[i].Position = float3((float*)&(scene->mMeshes[meshIndex]->mVertices[i]));
//			vertices[i].Normal = float3((float*)&(scene->mMeshes[meshIndex]->mNormals[i]));
//			vertices[i].Tangent = float3((float*)&(scene->mMeshes[meshIndex]->mTangents[i]));
//			vertices[i].TexCoord = float2((float*)&(scene->mMeshes[meshIndex]->mTextureCoords[0][i]));
//			if (scene->mMeshes[meshIndex]->mTextureCoords[1])
//			{
//				vertices[i].TexCoord2 = float2((float*)&(scene->mMeshes[meshIndex]->mTextureCoords[1][i]));
//			}
//			else
//			{
//				vertices[i].TexCoord2 = float2(0, 0);
//			}
//			vertices[i].Color = float3(0, 0, 0);
//
//			float t = dot(cross(scene->mMeshes[meshIndex]->mNormals[i], scene->mMeshes[meshIndex]->mTangents[i]), scene->mMeshes[meshIndex]->mBitangents[i]);
//#if	UV_MIRROR
//			if (t < 0)
//				vertices[i].Tangent = float3(-vertices[i].Tangent.r, vertices[i].Tangent.g, vertices[i].Tangent.b);
//#endif
//
//			min.r = min(min.r, vertices[i].Position.r);
//			min.g = min(min.g, vertices[i].Position.g);
//			min.b = min(min.b, vertices[i].Position.b);
//			max.r = max(max.r, vertices[i].Position.r);
//			max.g = max(max.g, vertices[i].Position.g);
//			max.b = max(max.b, vertices[i].Position.b);
//		}
//		size = max(max.r - min.r, max(max.g - min.g, max.b - min.b));
//		center = float3((min.r + max.r) / 2.0f, (min.g + max.g) / 2.0f, (min.b + max.b) / 2.0f);
//
//		for (long long i = 0; i < numFaces; ++i)
//		{
//			aiFace* face = &(scene->mMeshes[meshIndex]->mFaces[i]);
//			indices[3 * i] = face->mIndices[0];
//			indices[3 * i + 1] = face->mIndices[1];
//			indices[3 * i + 2] = face->mIndices[2];
//		}
//#endif
//		 
//		VertexBuffer::Create(pRenderSys, vertices, sizeof(Vertex) * numVertices);
//		IndexBuffer::Create(pRenderSys, indices, sizeof(uint16) * 3 * numFaces);
//		 
//		//delete scene;
//		return hr;
//
//	failed:
//		aiReleaseImport(scene);
//		delete vertices;
//		delete indices;
		return nullptr;
	}
	

}