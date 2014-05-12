#ifndef __MESH_H__
#define __MESH_H__

#include "basic.h"
#include "Prerequisites.h"
#include "Renderable.h"

namespace space{

	namespace graphic{

		static const int NumPrimTypes = 3;

		typedef enum _PrimitiveType{
			SP_POINTS = 0x0000, SP_LINES = 0x0001,
			SP_TRIANGLES = 0x0004, SP_TRIANGLES_STRIP = 0x0005, SP_TRIANGLES_FAN = 0x0006
		}PrimitiveType;

		class Mesh {
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
				bool Load(Mesh& mesh){ return false; }
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

				bool Load(Mesh& mesh);
			};

		public:
			Mesh(){ isBoundingBoxComputed = false; }
			~Mesh(){}

			bool LoadFromFile(const string& filename){

				Loader = new ObjFileLoader(filename);
				return Loader->Load(*this);
			}

			bool CompileModel(PrimitiveType);
			void Rescale(float radius);

			void ComputeBoundingBox(math::Vector3 &min, math::Vector3 &max);
			void ComputeNormals();
			void ComputeTexCoords();

			bool hasNormals()const;
			bool hasTexCoords()const;

			uint GetPositionSize()const;
			uint GetNormalSize() const;
			uint GetTexCoordSize() const;

			// Intersect( in , out, out );
			// in model coords
			bool Intersect(const Ray &ray, math::Vector3 &normal, math::Vector3 &position);
			bool IntersectBBox(const Ray &ray);
			Triangle GetTriangle(uint i0, uint i1, uint i2) const;
			//=================
			// raw data access functions
			//=================

			const float* GetPositions() const;
			const float* GetNormals() const;
			const float* GetTexCoords() const;

			const uint* GetPositionIndices() const;
			const uint* GetNormalIndices() const;
			const uint* GetTexCoordIndices() const;

			uint GetPositionCount() const;
			uint GetNormalCount() const;
			uint GetTexCoordCount() const;

			uint GetIndexCount() const;

			//=================
			// compiled data access functions
			//=================

			const float* GetCompiledVertices() const;
			const uint* GetCompiledIndices(PrimitiveType type = SP_TRIANGLES) const;

			uint GetCompiledVertexSize() const;
			uint GetCompiledVertexCount() const;
			uint GetCompiledIndexCount(PrimitiveType type = SP_TRIANGLES) const;

			uint GetCompiledPositionOffset() const;
			uint GetCompiledNormalOffset() const;
			uint GetCompiledTexCoordOffset() const;
		};
	}
}

#endif