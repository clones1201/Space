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
	public:
		static D3D11InputLayout* Create(D3D11Device& device);

		virtual ID3D11InputLayout* GetInputLayoutInterface() const = 0;
		virtual void Complete(byte const* pInputSignature, uint32 lengthInBytes) = 0;

		virtual ~D3D11InputLayout();
	protected:
		D3D11InputLayout(D3D11Device& device);

		D3D11Device& mDevice;

		CComPtr<ID3D11InputLayout> m_pD3D11InputLayout;

		std::vector<D3D11_INPUT_ELEMENT_DESC> m_D3D11ElemVector;
	};

}

#endif