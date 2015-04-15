#ifndef __SPACE_D3D11_INPUTLAYOUT_HPP__
#define __SPACE_D3D11_INPUTLAYOUT_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "D3D11Prerequisites.hpp"
#include "InputLayout.hpp"

namespace Space
{	
	class D3D11InputLayout : public InputLayout
	{
	private:
		CComPtr<ID3D11InputLayout> m_pD3D11InputLayout;

	public:
		ID3D11InputLayout* GetInputLayoutInterface(
			byte const* pInputSignature, uint32 lengthInBytes);
		
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DInputLayout() const;

	};

}

#endif