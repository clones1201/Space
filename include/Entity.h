#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Renderable.h"
#include "Mesh.h"
#include "Light.h"
#include "Camera.h"

namespace space{
	
	typedef struct _Texture2D{
	private:
		vector<uchar> image;
		uint width, height;
		uint TexID;
	public:
		_Texture2D(){}
		~_Texture2D(){}
		void Load(vector<float>&,uint,uint);
		void Load(vector<uchar>&,uint,uint);
		void LoadFromPNG(const string&);

		uint GetTexId()const;
		void SetTexId(uint Id);
		const vector<uchar>& GetTextureImage() const;
		uint GetFormat() const;
		uint GetWidth() const;
		uint GetHeight() const;
		graphic::Color GetTextureColor(float u, float v)const;
	}Texture2D,Texture;
	typedef shared_ptr<Texture> Texture_ptr;

	class Entity : public Renderable{
	public:
		Entity(const graphic::Mesh*);

		math::Vector3 GetPosition()const;

		void Move(const math::Vector3& move);
		void Rotate(float angleX,float angleY,float angleZ);

		void SetTexture(const Texture&);
	
	private:
		graphic::Mesh* mesh;
		Entity* sub;

		math::Matrix matPos;
		math::Matrix matRotate;

		Texture tex;
	};

	class Scene{
		DEFINE_GET_SET(vector<graphic::Light*>, Lights); //multi light
		DEFINE_GET_SET(vector<Camera*>, Cameras); // multi camera
		DEFINE_GET_SET(vector<Entity*>, Entitys); // multi object
	};
}


#endif