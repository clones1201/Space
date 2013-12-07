#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "basic.h"

namespace space{
	using namespace math;
	namespace graphic{
		
		typedef enum _LIGHTTYPE{
			POINTS = 1, SPOT = 2, DIRECTIONAL = 3
		}LightType;

		struct Light{
			LightType    Type;            /* Type of light source */
			Color   Diffuse;         /* Diffuse color of light */
			Color   Specular;        /* Specular color of light */
			Color   Ambient;         /* Ambient color of light */
			Vector3       Position;         /* Position in world space */
			Vector3       Direction;        /* Direction in world space */
			float           Range;            /* Cutoff range */
			float           Falloff;          /* Falloff */
			float           Attenuation0;     /* Constant attenuation */
			float           Attenuation1;     /* Linear attenuation */
			float           Attenuation2;     /* Quadratic attenuation */
			float           Theta;            /* Inner angle of spotlight cone */
			float           Phi;              /* Outer angle of spotlight cone */
			Light(LightType type = POINTS);
		};

		template <LightType T = POINTS>
		inline Light CreateLights();

		template <>
		inline Light CreateLights<POINTS>(){
			Light result;
			result.Type = POINTS;
			result.Position = Vector3(0, 0, 0);
			result.Range = 1000;
			result.Attenuation0 = 1;
			result.Attenuation1 = 0;
			result.Attenuation2 = 0;
			result.Specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
			result.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			result.Ambient = Color(0.1f, 0.1f, 0.1f, 1.0f);
			result.Direction = Vector3(1, 0, 0);
			result.Falloff = 1;
			result.Phi = 0;
			result.Theta = 0;
			return result;
		}
		template <>
		inline Light CreateLights<SPOT>(){
			Light result;
			result.Type = SPOT;
			result.Position = Vector3(0, 0, 0);
			result.Range = 1000;
			result.Attenuation0 = 1;
			result.Attenuation1 = 0;
			result.Attenuation2 = 0;
			result.Specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
			result.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			result.Ambient = Color(0.1f, 0.1f, 0.1f, 1.0f);
			result.Direction = Vector3(1, 0, 0);
			result.Falloff = 1;
			result.Phi = 0;
			result.Theta = 0;
			return result;
		}
		template <>
		inline Light CreateLights<DIRECTIONAL>(){
			Light result;
			result.Type = DIRECTIONAL;
			result.Position = Vector3(0, 0, 0);
			result.Range = 1000;
			result.Attenuation0 = 1;
			result.Attenuation1 = 0;
			result.Attenuation2 = 0;
			result.Specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
			result.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			result.Ambient = Color(0.1f, 0.1f, 0.1f, 1.0f);
			result.Direction = Vector3(1, 0, 0);
			result.Falloff = 1;
			result.Phi = 0;
			result.Theta = 0;
			return result;
		}

		class ILight :public Interface{
		public:
			int SetLight(int,const Light&);
			int TurnOn(int);
			int TurnOff(int);
		};
		
	}
}

#endif