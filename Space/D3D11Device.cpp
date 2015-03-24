#include "Log.h"
#include "D3D11Device.hpp"

namespace Space
{
	class D3D11Device::Impl 
	{
	private:
		CComPtr<ID3D11Device> m_pDevice;
		CComPtr<ID3D11DeviceContext> m_pImmediateContext;
		CComPtr<IDXGIFactory> m_pDXGIFactory;
	public:
		Impl()
		{
			CComPtr<ID3D11Device> pDevice = nullptr;
			CComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
			HRESULT hr = S_OK;

			UINT createDeviceFlags = 0;
#ifdef _DEBUG
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
			D3D_DRIVER_TYPE		driverType = D3D_DRIVER_TYPE_NULL;
			D3D_FEATURE_LEVEL	featureLevel = D3D_FEATURE_LEVEL_11_0;

			D3D_DRIVER_TYPE driverTypes[] =
			{
				D3D_DRIVER_TYPE_HARDWARE,
				D3D_DRIVER_TYPE_WARP,
				D3D_DRIVER_TYPE_REFERENCE,
			};
			UINT numDriverTypes = ARRAYSIZE(driverTypes);

			D3D_FEATURE_LEVEL featureLevels[] =
			{
				featureLevel
			};
			UINT numFeatureLevels = ARRAYSIZE(featureLevels);

			CComPtr<IDXGIFactory> pFactory = nullptr;
			hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));

			CComPtr<IDXGIAdapter> pAdapter = nullptr;
			std::vector<CComPtr<IDXGIAdapter>> pAdapters;
			uint i = 0;
			while (pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
			{
				pAdapters.push_back(pAdapter);
				++i;
				pAdapter = nullptr;
			}

			for (uint i = 0; i < pAdapters.size(); ++i)
			{
				for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
				{
					driverType = driverTypes[driverTypeIndex];
					pDevice = nullptr;
					pDeviceContext = nullptr;
					hr = D3D11CreateDevice(
						pAdapters.at(i),
						driverType,
						NULL,
						createDeviceFlags,
						featureLevels,
						numFeatureLevels,
						D3D11_SDK_VERSION,
						&pDevice,
						&featureLevel,
						&pDeviceContext);

					if (SUCCEEDED(hr))
						break;
				}
				if (SUCCEEDED(hr))
					break;
			}
			if (FAILED(hr))
			{
				throw std::exception("D3D11CreateDevice failed");
			}
			if (featureLevel != D3D_FEATURE_LEVEL_11_0)
			{
				throw std::exception("FeatureLevel Not Supported");
			}
			Log("D3D11 Device Create Success. feature level:%d\n", featureLevel);
			m_pDevice = pDevice;
			m_pImmediateContext = pDeviceContext;

			CComPtr<IDXGIFactory> pDXGIFactory = nullptr;
			hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&(pDXGIFactory));
			if (FAILED(hr)){
				throw std::exception("CreateDXGIFactory failed"); 
			}
			m_pDXGIFactory = pDXGIFactory;
		}
		~Impl()
		{
		}

		ID3D11Device* Get() const
		{
			return m_pDevice;
		}
		ID3D11DeviceContext* GetImmediateContext()const
		{
			return m_pImmediateContext;
		}
		IDXGIFactory* GetDXGIFactory() const
		{
			return m_pDXGIFactory;
		}
	};

	D3D11Device::D3D11Device()
	{
		try
		{
			impl.reset(new Impl());
		}
		catch (std::exception& e)
		{
			Log(e.what());
		}
	}
	D3D11Device::~D3D11Device()
	{
	}

	D3D11Device::D3D11Device(D3D11Device&& param)
	{
		this->impl = std::move(param.impl);
	}

	D3D11Device& D3D11Device::operator=(D3D11Device &&param)
	{
		this->impl = std::move(param.impl);
		return *this;
	}

	ID3D11Device* D3D11Device::Get() const throw()
	{
		return impl->Get();
	}
	ID3D11DeviceContext* D3D11Device::GetImmediateContext() const throw()
	{
		return impl->GetImmediateContext();
	}
	ID3D11Device* D3D11Device::operator->() const throw()
	{
		return impl->Get();
	}
	IDXGIFactory* D3D11Device::GetDXGIFactory() const throw()
	{
		return impl->GetDXGIFactory();
	}

	bool D3D11Device::IsValid() const throw()
	{
		return impl != nullptr;
	}

}