#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Mesh.h"
#include "Light.h"
#include "Camera.h"

namespace space{
	using namespace graphic;
	using namespace math;

	typedef struct _Material{
		Color diffuse;
		Color specular;
		float shininess;

		_Material(Color _diffuse, Color _specular, float _shininess) :
			diffuse(_diffuse), specular(_specular), shininess(_shininess){}
	}Material;

	typedef struct _Texture{
		uint TexID;
	private:
		class TextureLoader :public Interface{
		protected:
			bool Setup(_Texture&,vector<uchar>&,uint,uint);
		public:
			virtual bool Load(_Texture&) = 0;
		};
		class PNGLoader : public TextureLoader{
		private:
			string filename;
		public:
			PNGLoader(const string&);
			virtual bool Load(_Texture&);
		};
		class RGBLoader : public TextureLoader{
		private:
			uint width, height;
			vector<uchar> *image;
		public:
			RGBLoader(vector<uchar>&,uint,uint);
			virtual bool Load(_Texture&);
		};
		class ColorValueLoader : public TextureLoader{
		private:
			uint width, height;
			vector<float> *image;
		public:
			ColorValueLoader(vector<float>&,uint,uint);
			virtual bool Load(_Texture&);
		};
		TextureLoader* loader;
	public:
		_Texture(){}
		~_Texture(){}
		void Load(vector<float>&,uint,uint);
		void Load(vector<uchar>&,uint,uint);
		void LoadFromPNG(const string&);
	}Texture;

	class Entity : public Object{
	public:
		Entity(const Mesh*);

		Vector3 GetPosition()const;

		void Move(const Vector3& move);
		void Rotate(float angleX,float angleY,float angleZ);

		void SetTexture(const Texture&);
	
	private:
		Mesh* mesh;
		Entity* sub;

		Matrix matPos;
		Matrix matRotate;

		Texture tex;
	};

	class Scene{
		DEFINE_GET_SET(vector<Light*>, Lights); //multi light
		DEFINE_GET_SET(vector<Camera*>, Cameras); // multi camera
		DEFINE_GET_SET(vector<Entity*>, Entitys); // multi object
	};
}


#endif