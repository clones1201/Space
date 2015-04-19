#include "Log.h"
#include "Utility.hpp"
#include "D3D11Shared.hpp"
#include "D3D11Device.hpp"
#include "D3D11InputLayout.hpp"

namespace Space
{

	class D3D11InputLayoutImpl : public D3D11InputLayout
	{
	public: 
		D3D11InputLayoutImpl(D3D11Device& device)
			:D3D11InputLayout(device)
		{
		}

		virtual void Complete(byte const* pInputSignature, uint32 lengthInBytes)
		{
			m_D3D11ElemVector.reserve(m_LayoutVector.size());
			for (auto layout = m_LayoutVector.begin();
				layout != m_LayoutVector.end();
				++layout)
			{
				D3D11_INPUT_ELEMENT_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.AlignedByteOffset = layout->offset;
				desc.SemanticName = GetSemanticName(layout->semantic);
				desc.SemanticIndex = layout->semanticId;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.Format = GetElemDXGIFormat(layout->type);
				m_D3D11ElemVector.push_back(desc);
			}

			ID3D11InputLayout* pInputLayout = nullptr;
			HRESULT hr = mDevice->CreateInputLayout(
				m_D3D11ElemVector.data(), m_D3D11ElemVector.size(),
				pInputSignature, lengthInBytes,
				&pInputLayout);
			if (FAILED(hr))
			{
				Log(TEXT("CreateInputLayout failed"));
				return;
			}
			m_pD3D11InputLayout = pInputLayout;
			_Complete();
		}

		virtual ID3D11InputLayout* GetInputLayoutInterface() const
		{
			return IsCompleted() ? m_pD3D11InputLayout.p : nullptr;
		}
	};

	D3D11InputLayout::D3D11InputLayout(D3D11Device& device)
		:mDevice(device)
	{}

	D3D11InputLayout::~D3D11InputLayout()
	{}

	D3D11InputLayout* D3D11InputLayout::Create(D3D11Device& device)
	{
		try
		{
			return new D3D11InputLayoutImpl(device);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}
}