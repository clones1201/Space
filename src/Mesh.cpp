#include "Mesh.h"

namespace space{

	using namespace math;
	namespace graphic{

		bool Mesh::ObjFileLoader::Load(Mesh& mesh){
	
			ifstream in(sourcefilename, ios_base::in);
			if (! in.is_open()){
				return false;
			}
			char buf[256];
			float val[4];
			int idx[3][3];
			stringstream buffer;

			bool hasNormals, hasTexCoords;
			while (!in.eof()){
				buffer.clear();
				in.getline(buf, 256, '\n');
				buffer.str(buf);
				switch (buffer.get()){
				case '#':
					//skip line
					break;
				case 'v':// vertex
					switch (buffer.get()){
					case ' ': // position
						val[3] = 1.0f;
						buffer >> val[0] >> val[1] >> val[2] >> val[3];
						mesh._positions.push_back(val[0]);
						mesh._positions.push_back(val[1]);
						mesh._positions.push_back(val[2]);
						mesh._posSize = 3;
						break;
					case 'n': // normal
						buffer.get();
						buffer >> val[0] >> val[1] >> val[2];
						mesh._normals.push_back(val[0]);
						mesh._normals.push_back(val[1]);
						mesh._normals.push_back(val[2]);
						break;
					case 't': // texture coordinate
						buffer.get();
						val[2] = 0.0f;
						buffer >> val[0] >> val[1] >> val[2];
						mesh._texCoords.push_back(val[0]);
						mesh._texCoords.push_back(val[1]);
						mesh._tcSize = 2;
						break;
					}
					break;
				case 'f':// face
					//buffer.str("123//456 123//456 123//456 456//789 012//345 678//901 234//567");

					buffer.get(); //eat the space

					buffer >> idx[0][0];
					if (buffer.get() == '/'){
						if (buffer.get() == '/'){
							//vertices and normals
							buffer >> idx[0][2];

							idx[0][0] = (idx[0][0] > 0) ? (idx[0][0] - 1) : ((int)mesh._positions.size() - idx[0][0]);
							idx[0][2] = (idx[0][2] > 0) ? (idx[0][2] - 1) : ((int)mesh._normals.size() - idx[0][2]);

							buffer >> idx[1][0]; buffer.get(); buffer.get();
							buffer >> idx[1][2];

							idx[1][0] = (idx[1][0] > 0) ? (idx[1][0] - 1) : ((int)mesh._positions.size() - idx[1][0]);
							idx[1][2] = (idx[1][2] > 0) ? (idx[1][2] - 1) : ((int)mesh._normals.size() - idx[1][2]);

							while (!buffer.eof()){

								buffer >> idx[2][0]; buffer.get(); buffer.get();
								buffer >> idx[2][2];

								idx[2][0] = (idx[2][0] > 0) ? (idx[2][0] - 1) : ((int)mesh._positions.size() - idx[2][0]);
								idx[2][2] = (idx[2][2] > 0) ? (idx[2][2] - 1) : ((int)mesh._normals.size() - idx[2][2]);

								for (int ii = 0; ii < 3; ii++){
									mesh._pIndex.push_back(idx[ii][0]);
									mesh._tIndex.push_back(0);
									mesh._nIndex.push_back(idx[ii][2]);
								}
								idx[1][0] = idx[2][0];
								idx[1][2] = idx[2][2];
							}
							hasNormals = true;
							hasTexCoords = false;
						}
						else{
							buffer.unget();
							buffer >> idx[0][1];
							if (buffer.get() == '/'){
								//vertices, texCoords and normals
								buffer >> idx[0][2];

								idx[0][0] = (idx[0][0] > 0) ? (idx[0][0] - 1) : ((int)mesh._positions.size() - idx[0][0]);
								idx[0][1] = (idx[0][1] > 0) ? (idx[0][1] - 1) : ((int)mesh._texCoords.size() - idx[0][1]);
								idx[0][2] = (idx[0][2] > 0) ? (idx[0][2] - 1) : ((int)mesh._normals.size() - idx[0][2]);

								buffer >> idx[1][0]; buffer.get();
								buffer >> idx[1][1]; buffer.get();
								buffer >> idx[1][2];

								idx[1][0] = (idx[1][0] > 0) ? (idx[1][0] - 1) : ((int)mesh._positions.size() - idx[1][0]);
								idx[1][1] = (idx[1][1] > 0) ? (idx[1][1] - 1) : ((int)mesh._texCoords.size() - idx[1][1]);
								idx[1][2] = (idx[1][2] > 0) ? (idx[1][2] - 1) : ((int)mesh._normals.size() - idx[1][2]);
								while (!buffer.eof()){

									buffer >> idx[2][0]; buffer.get();
									buffer >> idx[2][1]; buffer.get();
									buffer >> idx[2][2];

									idx[2][0] = (idx[2][0] > 0) ? (idx[2][0] - 1) : ((int)mesh._positions.size() - idx[2][0]);
									idx[2][1] = (idx[2][1] > 0) ? (idx[2][1] - 1) : ((int)mesh._texCoords.size() - idx[2][1]);
									idx[2][2] = (idx[2][2] > 0) ? (idx[2][2] - 1) : ((int)mesh._normals.size() - idx[2][2]);

									for (int ii = 0; ii < 3; ii++){
										mesh._pIndex.push_back(idx[ii][0]);
										mesh._tIndex.push_back(idx[ii][1]);
										mesh._nIndex.push_back(idx[ii][2]);
									}
									idx[1][0] = idx[2][0];
									idx[1][1] = idx[2][1];
									idx[1][2] = idx[2][2];
								}
								hasNormals = true;
								hasTexCoords = true;
							}
							else{

								idx[0][0] = (idx[0][0] > 0) ? (idx[0][0] - 1) : ((int)mesh._positions.size() - idx[0][0]);
								idx[0][1] = (idx[0][1] > 0) ? (idx[0][1] - 1) : ((int)mesh._texCoords.size() - idx[0][1]);

								//vertices, texCoords
								buffer >> idx[1][0]; buffer.get();
								buffer >> idx[1][1];

								idx[1][0] = (idx[1][0] > 0) ? (idx[1][0] - 1) : ((int)mesh._positions.size() - idx[1][0]);
								idx[1][1] = (idx[1][1] > 0) ? (idx[1][1] - 1) : ((int)mesh._texCoords.size() - idx[1][1]);

								while (!buffer.eof()){

									buffer >> idx[2][0]; buffer.get();
									buffer >> idx[2][1];

									idx[2][0] = (idx[2][0] > 0) ? (idx[2][0] - 1) : ((int)mesh._positions.size() - idx[2][0]);
									idx[2][1] = (idx[2][1] > 0) ? (idx[2][1] - 1) : ((int)mesh._texCoords.size() - idx[2][1]);

									for (int ii = 0; ii < 3; ii++){
										mesh._pIndex.push_back(idx[ii][0]);
										mesh._tIndex.push_back(idx[ii][1]);
										mesh._nIndex.push_back(0);
									}
									idx[1][0] = idx[2][0];
									idx[1][1] = idx[2][1];
								}

								hasNormals = false;
								hasTexCoords = true;

							}
						}
					}
					else{
						//only have vertices
						idx[0][0] = (idx[0][0] > 0) ? (idx[0][0] - 1) : ((int)mesh._positions.size() - idx[0][0]);

						buffer >> idx[1][0];

						idx[1][0] = (idx[1][0] > 0) ? (idx[1][0] - 1) : ((int)mesh._positions.size() - idx[1][0]);

						while (!buffer.eof()){

							buffer >> idx[2][0];

							idx[2][0] = (idx[2][0] > 0) ? (idx[2][0] - 1) : ((int)mesh._positions.size() - idx[2][0]);

							for (int ii = 0; ii < 3; ii++){
								mesh._pIndex.push_back(idx[ii][0]);
								mesh._tIndex.push_back(0);
								mesh._nIndex.push_back(0);
							}
							idx[1][0] = idx[2][0];
						}

						hasNormals = false;
						hasTexCoords = false;

					}

					break;
				case 's':
				case 'g':
				case 'u':
				case 'o':
					//all presently ignored
				default:
					break;
				}
			}
			in.close();

			if (!hasNormals) mesh._nIndex.clear();
			if (!hasTexCoords) mesh._tIndex.clear();

			return true;
		}

		// why the same data ordered in different way will lead to a totol different apperance?

		//////////////////////////////////////////////////////////////////////
		//
		// Local data structures
		//
		//////////////////////////////////////////////////////////////////////

		//
		//  Index gathering and ordering structure
		////////////////////////////////////////////////////////////
		struct IdxSet {
			GLuint pIndex;
			GLuint nIndex;
			GLuint tIndex;

			bool operator< (const IdxSet &rhs) const {
				if (pIndex < rhs.pIndex)
					return true;
				else if (pIndex == rhs.pIndex) {
					if (nIndex < rhs.nIndex)
						return true;
					else if (nIndex == rhs.nIndex) {
						if (tIndex < rhs.tIndex)
							return true;
					}
				}
				return false;
			}
		};

		//
		//  Edge connectivity structure 
		////////////////////////////////////////////////////////////
		struct Edge {
			GLuint pIndex[2]; //position indices

			bool operator< (const Edge &rhs) const {
				return (pIndex[0] == rhs.pIndex[0]) ? (pIndex[1] < rhs.pIndex[1]) : pIndex[0] < rhs.pIndex[0];
			}

			Edge(GLuint v0, GLuint v1) {
				pIndex[0] = min(v0, v1);
				pIndex[1] = max(v0, v1);
			}

		private:
			Edge() {} // disallow the default constructor
		};

		bool Mesh::CompileModel(PrimitiveType type){
			bool needsTriangles = false;
			bool needsEdges = false;
			bool needsPoints = false;

			if ((type & SP_POINTS) == SP_POINTS)
				needsPoints = true;

			if ((type & SP_TRIANGLES) == SP_TRIANGLES)
				needsTriangles = true;

			if ((type & SP_LINES) == SP_LINES) {
				needsTriangles = true;
				needsEdges = true;
			}

			//merge the points
			map<IdxSet, GLuint> pts;

			//find whether a position is unique
			set<GLuint> ptSet;

			{
				vector<GLuint>::iterator pit = _pIndex.begin();
				vector<GLuint>::iterator nit = _nIndex.begin();
				vector<GLuint>::iterator tit = _tIndex.begin();

				while (pit < _pIndex.end()) {
					IdxSet idx;
					idx.pIndex = *pit;

					if (_normals.size() > 0)
						idx.nIndex = *nit;
					else
						idx.nIndex = 0;

					if (_tIndex.size() > 0)
						idx.tIndex = *tit;
					else
						idx.tIndex = 0;

					map<IdxSet, GLuint>::iterator mit = pts.find(idx);

					if (mit == pts.end()) { //new index

						if (needsTriangles)  //Triangles Indices
							_indices[2].push_back((GLuint)pts.size());

						//since this one is a new vertex, check to see if this position is already referenced
						if (needsPoints && ptSet.find(idx.pIndex) != ptSet.end()) {
							ptSet.insert(idx.pIndex);
						}

						pts.insert(map<IdxSet, GLuint>::value_type(idx, (GLuint)pts.size()));

						//position
						_vertices.push_back(_positions[idx.pIndex*_posSize]);
						_vertices.push_back(_positions[idx.pIndex*_posSize + 1]);
						_vertices.push_back(_positions[idx.pIndex*_posSize + 2]);
						if (_posSize == 4)
							_vertices.push_back(_positions[idx.pIndex*_posSize + 3]);

						//normal
						if (_normals.size() > 0) {
							_vertices.push_back(_normals[idx.nIndex * 3]);
							_vertices.push_back(_normals[idx.nIndex * 3 + 1]);
							_vertices.push_back(_normals[idx.nIndex * 3 + 2]);
						}

						//texture coordinate
						if (_texCoords.size() > 0) {
							_vertices.push_back(_texCoords[idx.tIndex*_tcSize]);
							_vertices.push_back(_texCoords[idx.tIndex*_tcSize + 1]);
							if (_tcSize == 3)
								_vertices.push_back(_texCoords[idx.tIndex*_tcSize + 2]);
						}
					}
					else {
						if (needsTriangles)
							_indices[2].push_back(mit->second);
					}

					//advance the iterators if the components are present
					pit++;

					if (hasNormals())
						nit++;

					if (hasTexCoords())
						tit++;
				}
			}

			//create an edge list, if necessary
			if (needsEdges) {
				std::multimap<Edge, GLuint> edges;

				//edges are only based on positions only
				for (int ii = 0; ii < (int)_pIndex.size(); ii += 3) {
					for (int jj = 0; jj < 3; jj++) {
						Edge w(_pIndex[ii + jj], _pIndex[ii + (jj + 1) % 3]);
						std::multimap<Edge, GLuint>::iterator it = edges.find(w);

						//if we are storing edges, make sure we store only one copy
						if (needsEdges && it == edges.end()) {
							_indices[1].push_back(_indices[2][ii + jj]);
							_indices[1].push_back(_indices[2][ii + (jj + 1) % 3]);
						}
						edges.insert(std::multimap<Edge, GLuint>::value_type(w, ii / 3));
					}
				}
			}
			//set the offsets and vertex size
			_pOffset = 0; //always first
			_vtxSize = _posSize;
			if (hasNormals()) {
				_nOffset = _vtxSize;
				_vtxSize += 3;
			}
			else {
				_nOffset = -1;
			}
			if (hasTexCoords()) {
				_tcOffset = _vtxSize;
				_vtxSize += _tcSize;
			}
			else {
				_tcOffset = -1;
			}

			return true;
		}

		void Mesh::Rescale(float radius){
			Vector3 max, min;
			ComputeBoundingBox(min, max);

			Vector3 r = (max - min) * 0.5f;
			Vector3 center = min + r;
			float oldRadius = max(r.x, max(r.y, r.z));
			float scale = radius / oldRadius;

			for (vector<float>::iterator iter = _positions.begin();
				iter < _positions.end();){
				Vector3 temp(*iter, *(iter + 1), *(iter + 2));
				Vector3 newPos = (temp - center) * scale;
				iter[0] = newPos.x;
				iter[1] = newPos.y;
				iter[2] = newPos.z;
				iter += 3;
			}

		}

		void Mesh::ComputeBoundingBox(Vector3 &min, Vector3 &max){
		 
			if (isBoundingBoxComputed){
				min = bmin, max = bmax;
				return;
			}

			min = Vector3(1e10f, 1e10f, 1e10f);
			max = -min;

			for (auto iter = _positions.begin(); iter < _positions.end();){
				Vector3 temp(*iter, *(iter + 1), *(iter + 2));
				if (temp.x < min.x) min.x = temp.x;
				if (temp.y < min.y) min.y = temp.y;
				if (temp.z < min.z) min.z = temp.z;

				if (temp.x > max.x) max.x = temp.x;
				if (temp.y > max.y) max.y = temp.y;
				if (temp.z > max.z) max.z = temp.z;
				iter += 3;
			}
			isBoundingBoxComputed = true;
			bmax = max; bmin = min;
		}
		void Mesh::ComputeNormals(){}

		//should run before compile the mesh
		void Mesh::ComputeTexCoords(){
			if (_texCoords.size() > 0) //already have texture coords
				return;

			_tcSize = 2;

			vector<uint>::iterator pIter = _pIndex.begin();
			while (pIter != _pIndex.end()){
				uint pIdx = *pIter;

				Vector3 position(_positions[pIdx * _posSize],
					_positions[pIdx * _posSize + 1],
					_positions[pIdx * _posSize + 2]);

				float u, v;
				u = atanf(position.x / position.z);
				v = Pi / 2 + atanf(sqrtf(position.x * position.x + position.z * position.z) / position.y);
				if (position.z < 0) u += Pi;
				u /= Pi;
				v /= 2 * Pi; //restrick the value to [0,1]

				_texCoords.push_back(u);
				_texCoords.push_back(v);

				_tIndex.push_back(_texCoords.size() / _tcSize - 1);

				pIter++;
			}
		}

		bool Mesh::hasNormals() const{ return _normals.size() > 0 ? true : false; }
		bool Mesh::hasTexCoords() const{ return _texCoords.size() > 0 ? true : false; }

		uint Mesh::GetPositionSize() const{ return _posSize; }
		uint Mesh::GetNormalSize() const{ return 3; }
		uint Mesh::GetTexCoordSize()const { return _tcSize; }

		Triangle Mesh::GetTriangle(uint i0, uint i1, uint i2) const{
			Triangle tri;
			tri.v0 = Vector3(_vertices[i0 * _vtxSize],
				_vertices[i0 * _vtxSize + 1],
				_vertices[i0 * _vtxSize + 2]);
			if (hasTexCoords()){
				tri.t0.x = _vertices[i0 * _vtxSize + _tcOffset];
				tri.t0.y = _vertices[i0 * _vtxSize + _tcOffset + 1];
			}
			if (hasNormals()){
				tri.n0 = Vector3(_vertices[i0 * _vtxSize + _nOffset],
					_vertices[i0 * _vtxSize + _nOffset + 1],
					_vertices[i0 * _vtxSize + _nOffset + 2]);
			}

			tri.v1 = Vector3(_vertices[i1 * _vtxSize],
				_vertices[i1 * _vtxSize + 1],
				_vertices[i1 * _vtxSize + 2]); 
			if (hasTexCoords()){
				tri.t1.x = _vertices[i1 * _vtxSize + _tcOffset];
				tri.t1.y = _vertices[i1 * _vtxSize + _tcOffset + 1];
			}
			if (hasNormals()){
				tri.n1 = Vector3(_vertices[i1 * _vtxSize + _nOffset],
					_vertices[i1 * _vtxSize + _nOffset + 1],
					_vertices[i1 * _vtxSize + _nOffset + 2]);
			}

			tri.v2 = Vector3(_vertices[i2 * _vtxSize],
				_vertices[i2 * _vtxSize + 1],
				_vertices[i2 * _vtxSize + 2]);
			if (hasTexCoords()){
				tri.t2.x = _vertices[i2 * _vtxSize + _tcOffset];
				tri.t2.y = _vertices[i2 * _vtxSize + _tcOffset + 1];
			}
			if (hasNormals()){
				tri.n2 = Vector3(_vertices[i2 * _vtxSize + _nOffset],
					_vertices[i2 * _vtxSize + _nOffset + 1],
					_vertices[i2 * _vtxSize + _nOffset + 2]);
			}
			return tri;
		}

		// see: http://geomalgorithms.com/a06-_intersect-2.html
		bool Mesh::Intersect(const Ray& ray, Vector3 & normal, Vector3 &position){
			bool result = false;
			float tmin = INFINITY;
			if (IntersectBBox(ray)){

				auto Idxit = _indices[2].begin();
				while (Idxit < _indices[2].end()){
					Triangle tri = GetTriangle((*Idxit), (*Idxit + 1), (*Idxit + 2));
					float t;
					tri.Intersect(ray, t);
					if (t < tmin){
						tmin = t;
						position = ray.ori + t * ray.dir;
					}
					result = true;
					Idxit += 3;
				}
				return result;
			}
			return false;
		}
		 
		bool Mesh::IntersectBBox(const Ray&ray){
			BBox box;
			ComputeBoundingBox(box.bmin, box.bmax);
			float tmin, tmax;
			return box.Intersect(ray,tmin,tmax);
		}

		//=================
		// raw data access functions
		//=================

		const float* Mesh::GetPositions() const{ return Mesh::_positions.data(); }
		const float* Mesh::GetNormals() const{ return Mesh::_normals.data(); }
		const float* Mesh::GetTexCoords() const{ return Mesh::_texCoords.data(); }

		const uint* Mesh::GetPositionIndices() const{ return Mesh::_pIndex.data(); }
		const uint* Mesh::GetNormalIndices() const{ return Mesh::_nIndex.data(); }
		const uint* Mesh::GetTexCoordIndices() const{ return Mesh::_tIndex.data(); }

		uint Mesh::GetPositionCount() const{ return Mesh::_positions.size() / _posSize; }
		uint Mesh::GetNormalCount() const{ return Mesh::_normals.size() / 3; }
		uint Mesh::GetTexCoordCount() const{ return Mesh::_texCoords.size() / _tcSize; }

		uint Mesh::GetIndexCount() const{ return Mesh::_nIndex.size(); }

		//=================
		// compiled data access functions
		//=================

		const float* Mesh::GetCompiledVertices() const{ return Mesh::_vertices.data(); }
		const uint* Mesh::GetCompiledIndices(PrimitiveType type) const{
			switch (type) {
			case SP_POINTS:
				return (_indices[0].size() > 0) ? _indices[0].data() : 0;
			case SP_LINES:
				return (_indices[1].size() > 0) ? _indices[1].data() : 0;
			case SP_TRIANGLES:
				return (_indices[2].size() > 0) ? _indices[2].data() : 0;
			default:
				return (_indices[2].size() > 0) ? _indices[2].data() : 0;
			}
		}

		uint Mesh::GetCompiledVertexSize() const{
			return _vtxSize;
		}
		uint Mesh::GetCompiledVertexCount() const{
			return _vertices.size();
		}
		uint Mesh::GetCompiledIndexCount(PrimitiveType type) const{
			switch (type) {
			case SP_POINTS:
				return (uint)_indices[0].size();
			case SP_LINES:
				return (uint)_indices[1].size();
			case SP_TRIANGLES:
				return (uint)_indices[2].size();
			}
		}

		uint Mesh::GetCompiledPositionOffset() const{
			return Mesh::_pOffset;
		}
		uint Mesh::GetCompiledNormalOffset() const{
			return Mesh::_nOffset;
		}
		uint Mesh::GetCompiledTexCoordOffset() const{
			return Mesh::_tcOffset;
		}
	}
}
