#pragma once

#include "RenderCommon.hpp"
#include "D3D11Prerequisites.hpp"

namespace Space 
{
namespace Render 
{
	class SPACE_D3D11_API D3D11Device : public Uncopyable
	{
	public:
		inline ID3D11Device* Get() const
		{
			return m_pDevice;
		}
		inline ID3D11DeviceContext* GetImmediateContext() const
		{
			return m_pImmediateContext;
		}
		inline ID3D11Device* operator->() const
		{
			return m_pDevice;
		}
		inline IDXGIFactory* GetDXGIFactory() const
		{
			return m_pDXGIFactory;
		}

		~D3D11Device();

		D3D11Device();

	private:
		static std::unordered_map<UINT, const char*> _feature_level_names;

		CComPtr<ID3D11Device> m_pDevice = nullptr;
		CComPtr<ID3D11DeviceContext> m_pImmediateContext = nullptr;
		CComPtr<IDXGIFactory> m_pDXGIFactory = nullptr;

#ifdef _DEBUG
		CComPtr<ID3D11Debug> m_pDebug = nullptr;
#endif

		D3D_DRIVER_TYPE m_DriverType = D3D_DRIVER_TYPE_UNKNOWN;
		D3D_FEATURE_LEVEL m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	};
}
}
