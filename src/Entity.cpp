#include "Entity.h"
#include "..\common\lodepng\lodepng.h"

namespace space{
	using namespace graphic;
	using namespace math;

	void Texture::LoadFromPNG(const string &filename){
		lodepng::decode(image, width, height, filename);
	}

	void Texture::Load(vector<float>& image, uint w, uint h){
		for (vector<float>::iterator pIter = image.begin();
			pIter < image.end(); pIter++){
			Texture::image.push_back((uchar)*pIter * 255);
		}
		width = w; height = h;
	}

	void Texture::Load(vector<uchar>& image, uint w, uint h){
		Texture::image = vector<uchar>(image);
		width = w; height = h;
	}

	uint Texture::GetTexId()const{
		return TexID;
	}

	void Texture::SetTexId(uint Id){
		TexID = Id;
	}

	Color Texture::GetTextureColor(float u, float v)const{
		/* Texture Wrap Repeat */

		u = (float)fmod(u, 1);
		if (u < 0.0f){
			u = 1.0f + u;
		}

		v = (float)fmod(v, 1);
		if (v < 0.0f){
			v = 1.0f + v;
		}

		uint w = u * width == width ? (uint)(u * width - 1) : (uint)u*width;
		uint h = v * height == height ? (uint)(v* height - 1) : (uint)v*width;

		Color result;
		if (4 == image.size() / (width*height)){
			result.r = image[4 * (w + h*width)] / 255.0f;
			result.g = image[4 * (w + h*width) + 1] / 255.0f;
			result.b = image[4 * (w + h*width) + 2] / 255.0f;
			result.a = image[4 * (w + h*width) + 3] / 255.0f;
		}
		else{
			result.r = image[3 * (w + h*width)] / 255.0f;
			result.g = image[3 * (w + h*width) + 1] / 255.0f;
			result.b = image[3 * (w + h*width) + 2] / 255.0f;
			result.a = 1.0f;
		}
		return result;
	}
	uint Texture::GetFormat() const{
		return (4 == image.size() / (width*height)) ? 4 : 3;
	}
	uint Texture::GetWidth() const{
		return width;
	}
	uint Texture::GetHeight() const{
		return height;
	}

	const vector<uchar>& Texture::GetTextureImage() const{
		return image;
	}

	//bool Texture::Setup(_Texture2D &tex, vector<uchar>& image, uint width, uint height){
	//	uint format = 4 == image.size() / (width*height) ?
	//	GL_RGBA : GL_RGB;

	//	glGenTextures(1, &tex.TexID);
	//	glBindTexture(GL_TEXTURE_2D, tex.TexID);
	//	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, format, GL_UNSIGNED_BYTE, image.data());
	//	//glTexImage2D(GL_TEXTURE_2D, 3	, format, width, height, 0, format, GL_UNSIGNED_BYTE, image.data());

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//	return true;
	//}

	/*void Entity::SetTexture(const _Texture2D& tex){
		Entity::tex = tex;
		}*/
}