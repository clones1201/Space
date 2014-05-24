#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "basic.h"

namespace space{
	namespace graphic{
		
		typedef enum _LIGHTTYPE{
			POINTS = 1, SPOT = 2, DIRECTIONAL = 3
		}LightType;

		class Light{
		public:
			template <LightType T = POINTS>
			LightPtr Create();
		protected:
			LightType    Type;            /* Type of light source */
			Color   Diffuse;         /* Diffuse color of light */
			Color   Specular;        /* Specular color of light */
			Color   Ambient;         /* Ambient color of light */
			math::Vector3       Position;         /* Position in world space */
			math::Vector3       Direction;        /* Direction in world space */
			float           Range;            /* Cutoff range */
			float           Falloff;          /* Falloff */
			float           Attenuation0;     /* Constant attenuation */
			float           Attenuation1;     /* Linear attenuation */
			float           Attenuation2;     /* Quadratic attenuation */
			float           Theta;            /* Inner angle of spotlight cone */
			float           Phi;              /* Outer angle of spotlight cone */
			Light(LightType type = POINTS);
		};

		inline Light CreateLights();

		template <>
		inline LightPtr Light::Create<POINTS>(){
			auto ret = LightPtr(new Light);
			ret->Type = POINTS;
			ret->Position = math::Vector3(0, 0, 0);
			ret->Range = 1000;
			ret->Attenuation0 = 1;
			ret->Attenuation1 = 0;
			ret->Attenuation2 = 0;
			ret->Specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
			ret->Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			ret->Ambient = Color(0.1f, 0.1f, 0.1f, 1.0f);
			ret->Direction = math::Vector3(1, 0, 0);
			ret->Falloff = 1;
			ret->Phi = 0;
			ret->Theta = 0;
			return ret;
		}
		template <>
		inline LightPtr Light::Create<SPOT>(){
			auto ret = LightPtr(new Light);
			ret->Type = SPOT;
			ret->Position = math::Vector3(0, 0, 0);
			ret->Range = 1000;
			ret->Attenuation0 = 1;
			ret->Attenuation1 = 0;
			ret->Attenuation2 = 0;
			ret->Specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
			ret->Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			ret->Ambient = Color(0.1f, 0.1f, 0.1f, 1.0f);
			ret->Direction = math::Vector3(1, 0, 0);
			ret->Falloff = 1;
			ret->Phi = 0;
			ret->Theta = 0;
			return ret;
		}
		template <>
		inline LightPtr Light::Create<DIRECTIONAL>(){
			auto ret = LightPtr(new Light);
			ret->Type = DIRECTIONAL;
			ret->Position = math::Vector3(0, 0, 0);
			ret->Range = 1000;
			ret->Attenuation0 = 1;
			ret->Attenuation1 = 0;
			ret->Attenuation2 = 0;
			ret->Specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
			ret->Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			ret->Ambient = Color(0.1f, 0.1f, 0.1f, 1.0f);
			ret->Direction = math::Vector3(1, 0, 0);
			ret->Falloff = 1;
			ret->Phi = 0;
			ret->Theta = 0;
			return ret;
		}

		
	}
}

#endif