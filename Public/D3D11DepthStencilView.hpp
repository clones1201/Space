#ifndef __SPACE_D3D11_DEPTHSTENCIL_HPP__
#define __SPACE_D3D11_DEPTHSTENCIL_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "DepthStencilView.hpp"
#include "D3D11Prerequisites.hpp"

namespace Space
{
	class D3D11DepthStencilView : virtual public DepthStencilView
	{
	protected:
		D3D11DepthStencilView();
					 
	public:
		virtual ~D3D11DepthStencilView();		

		static D3D11DepthStencilView* Create(D3D11Device& device, ID3D11Texture2D* pBuffer);
		virtual ID3D11DepthStencilView* GetDepthStencilView() const = 0;
	};
}

#endif