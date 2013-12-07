#include "Entity.h"
#include "..\common\lodepng\lodepng.h"

namespace space{
	using namespace graphic;
	using namespace math;

	void Texture::LoadFromPNG(const string &filename){
		loader = new PNGLoader(filename);

		loader->Load(*this);

		if (loader != nullptr) delete loader;
	}

	void Texture::Load(vector<float>& image, uint w, uint h){
		loader = new ColorValueLoader(image, w, h);

		loader->Load(*this);

		if (loader != nullptr) delete loader;
	}

	void Texture::Load(vector<uchar>& image, uint w, uint h){
		loader = new RGBLoader(image, w, h);

		loader->Load(*this);

		if (loader != nullptr) delete loader;
	}

	bool Texture::TextureLoader::Setup(_Texture &tex, vector<uchar>& image, uint width, uint height){
		uint format = 4 == image.size() / (width*height) ?
		GL_RGBA : GL_RGB;

		glGenTextures(1, &tex.TexID);
		glBindTexture(GL_TEXTURE_2D, tex.TexID);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, format, GL_UNSIGNED_BYTE, image.data());
		//glTexImage2D(GL_TEXTURE_2D, 3	, format, width, height, 0, format, GL_UNSIGNED_BYTE, image.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
		glBindTexture(GL_TEXTURE_2D, 0);
		return true;
	}

	Texture::PNGLoader::PNGLoader(const string& filename) :filename(filename){}
	Texture::RGBLoader::RGBLoader(vector<uchar>& array, uint width, uint height) : image(&array), width(width), height(height){}
	Texture::ColorValueLoader::ColorValueLoader(vector<float>& array, uint width, uint height) : image(&array), width(width), height(height){}

	bool Texture::PNGLoader::Load(_Texture &tex){
		std::vector<uchar> image;
		uint width, height;
		uint error = lodepng::decode(image, width, height, PNGLoader::filename);

		if (error != 0) return false;
		return TextureLoader::Setup(tex, image, width, height);
	}

	bool Texture::ColorValueLoader::Load(Texture &tex){
		std::vector<uchar> image_uchar;
		for (vector<float>::iterator pIter = image->begin();
			pIter < image->end(); pIter++){
			image_uchar.push_back(*pIter * 255);
		}
		return TextureLoader::Setup(tex, image_uchar, width, height);
	}

	bool Texture::RGBLoader::Load(Texture &tex){
		return TextureLoader::Setup(tex, *image, width, height);
	}

	void Entity::SetTexture(const Texture& tex){
		Entity::tex = tex;
	}
}