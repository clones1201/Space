#include "Light.h"
#include "Prerequisites.h"

namespace space{

	namespace graphic{

		Light::Light(LightType type){
			switch (type){
			case POINTS:
				*this = CreateLights<POINTS>();
				break;
			case SPOT:
				*this = CreateLights<SPOT>();
				break;
			case DIRECTIONAL:
				*this = CreateLights<DIRECTIONAL>();
				break;
			}
		}

		template <LightType T>
		inline void SetLightHelp(int lenum, const Light& l);

		template <>
		inline void SetLightHelp<POINTS>(int lenum, const Light& l){
			
		}
		template <>
		inline void SetLightHelp<SPOT>(int lenum, const Light& l){

		}
		template <>
		inline void SetLightHelp<DIRECTIONAL>(int lenum, const Light& l){

		}

		int ILight::SetLight(int lenum, const Light& l){
			switch (l.Type){
			case POINTS:
				SetLightHelp<POINTS>(lenum, l);
				break;
			case SPOT:
				SetLightHelp<SPOT>(lenum, l);
				break;
			case DIRECTIONAL:
				SetLightHelp<DIRECTIONAL>(lenum, l);
				break;
			}
			return lenum;
		}
		int ILight::TurnOn(int lenum){
			return lenum;
		}
		int ILight::TurnOff(int lenum){
			return lenum;
		} 
	}
}