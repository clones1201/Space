#ifndef __MESH_H__
#define __MESH_H__

#include "basic.h"
#include "Prerequisites.h"

namespace space{

	namespace graphic{

		//=========================
		// Based on NVIDIA SDK SAMPLE
		// nvModel.h nvModel.cpp nvModelObj.cpp nvModelQuery.cpp
		//
		// I don't know whether it's illegal to use the SDK code...please don't sue me...
		//=========================
		typedef enum _PrimitiveType{
			SP_POINTS = 0x0000, SP_LINES = 0x0001, 
			SP_TRIANGLES = 0x0004, SP_TRIANGLES_STRIP = 0x0005, SP_TRIANGLES_FAN = 0x0006
		}PrimitiveType;

		static const int NumPrimTypes = 3;

		class IMesh : public Interface{
		public:
			virtual bool LoadFromFile(const string &filename) = 0;
			virtual bool CompileModel(PrimitiveType) = 0;
			virtual void Rescale(float scale) = 0;

			virtual void ComputeBoundingBox(math::Vector3 &min, math::Vector3 &max) = 0;
			virtual void ComputeNormals() = 0;
			virtual void ComputeTexCoords() = 0;

			virtual bool hasNormals() const = 0;
			virtual bool hasTexCoords() const = 0;

			virtual uint GetPositionSize()const = 0;
			virtual uint GetNormalSize() const = 0;
			virtual uint GetTexCoordSize() const = 0;

			// Intersect( in , out, out );
			// in model coords
			virtual bool Intersect(const Ray &ray, math::Vector3 &normal, math::Vector3 &position) = 0;
			virtual bool IntersectBBox(const Ray &ray) = 0;

			//=================
			// raw data access functions
			//=================

			virtual const float* GetPositions() const = 0;
			virtual const float* GetNormals() const = 0;
			virtual const float* GetTexCoords() const = 0;

			virtual const uint* GetPositionIndices() const = 0;
			virtual const uint* GetNormalIndices() const = 0;
			virtual const uint* GetTexCoordIndices() const = 0;

			virtual uint GetPositionCount() const = 0;
			virtual uint GetNormalCount() const = 0;
			virtual uint GetTexCoordCount() const = 0;

			virtual uint GetIndexCount() const = 0;

			//=================
			// compiled data access functions
			//=================

			virtual const float* GetCompiledVertices() const = 0;
			virtual const uint* GetCompiledIndices(PrimitiveType type = SP_TRIANGLES) const = 0;

			virtual uint GetCompiledVertexSize() const = 0;
			virtual uint GetCompiledVertexCount() const = 0;
			virtual uint GetCompiledIndexCount(PrimitiveType type = SP_TRIANGLES) const = 0;

			virtual uint GetCompiledPositionOffset() const = 0;
			virtual uint GetCompiledNormalOffset() const = 0;
			virtual uint GetCompiledTexCoordOffset() const = 0;
		};

		class Mesh : public IMesh{
		private:
			std::vector<float> _positions;
			std::vector<float> _normals;
			std::vector<float> _texCoords;
			int _posSize;
			int _tcSize;

			std::vector<uint> _pIndex;
			std::vector<uint> _nIndex;
			std::vector<uint> _tIndex;

			//data structures optimized for rendering, compiled model
			std::vector<uint> _indices[NumPrimTypes];
			std::vector<float> _vertices;
			int _pOffset;
			int _nOffset;
			int _tcOffset;
			int _vtxSize;

			math::Vector3 bmax, bmin;
			bool isBoundingBoxComputed;
			
			class IModelLoader :public Interface{
			public:
				virtual bool Load(Mesh& mesh) = 0;
			};
			class ModelLoader : public Interface{
			public:
				virtual bool Load(Mesh& mesh){ return false; }
			};
			ModelLoader* Loader;

			// specific loader
			class ObjFileLoader : public ModelLoader{
			private:
				string sourcefilename;
			public:
				ObjFileLoader(const string &filename){
					sourcefilename = filename;
				}
				~ObjFileLoader(){}

				bool Load(Mesh& mesh) final;
			};

		public:
			Mesh(){ isBoundingBoxComputed = false; }
			~Mesh(){}

			virtual bool LoadFromFile(const string& filename){

				Loader = new ObjFileLoader(filename);
				return Loader->Load(*this);
			}

			virtual bool CompileModel(PrimitiveType) final;
			virtual void Rescale(float radius)final;

			virtual void ComputeBoundingBox(math::Vector3 &min, math::Vector3 &max) final;
			virtual void ComputeNormals()final;
			virtual void ComputeTexCoords()final;

			virtual bool hasNormals()const final;
			virtual bool hasTexCoords()const final;

			virtual uint GetPositionSize()const;
			virtual uint GetNormalSize() const;
			virtual uint GetTexCoordSize() const;

			// Intersect( in , out, out );
			// in model coords
			virtual bool Intersect(const Ray &ray, math::Vector3 &normal, math::Vector3 &position);
			virtual bool IntersectBBox(const Ray &ray);
			Triangle GetTriangle(uint i0, uint i1, uint i2) const;
			//=================
			// raw data access functions
			//=================

			virtual const float* GetPositions() const final;
			virtual const float* GetNormals() const final;
			virtual const float* GetTexCoords() const final;

			virtual const uint* GetPositionIndices() const final;
			virtual const uint* GetNormalIndices() const final;
			virtual const uint* GetTexCoordIndices() const final;

			virtual uint GetPositionCount() const final;
			virtual uint GetNormalCount() const final;
			virtual uint GetTexCoordCount() const final;

			virtual uint GetIndexCount() const final;

			//=================
			// compiled data access functions
			//=================

			virtual const float* GetCompiledVertices() const final;
			virtual const uint* GetCompiledIndices(PrimitiveType type = SP_TRIANGLES) const final;

			virtual uint GetCompiledVertexSize() const final;
			virtual uint GetCompiledVertexCount() const final;
			virtual uint GetCompiledIndexCount(PrimitiveType type = SP_TRIANGLES) const final;

			virtual uint GetCompiledPositionOffset() const final;
			virtual uint GetCompiledNormalOffset() const final;
			virtual uint GetCompiledTexCoordOffset() const final;
		};
	}
}

#endif