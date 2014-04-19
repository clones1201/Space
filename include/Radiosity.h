#ifndef __SPACE_RADIOSITY_H__
#define __SPACE_RADIOSITY_H__

#include "Prerequisites.h"
#include "basic.h"
#include "Entity.h"



namespace space{
	namespace graphic{
		namespace radiosity{

			struct Patch{
				Color radiosity;
				math::Vector3 position;
				math::Vector3 normal;
			};
			
			//very slow, venusm.obj took me 1 hours to calculate a sh*t-like shadow map...
			vector<uchar> SetupShadow(Mesh& mesh, uint width, uint height);


			void DrawTextureMap(Mesh& mesh, uint width, uint height);

			void GeneratePatch(Mesh& mesh);


		}
	}
}

#endif