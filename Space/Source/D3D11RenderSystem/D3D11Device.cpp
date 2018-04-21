#include "D3D11RenderSystem/D3D11Device.hpp"

namespace Space {
	namespace Render {
		D3D11Device::D3D11Device()
		{
			auto feature_level_names = std::unordered_map<UINT, const char*>({
				{ D3D_FEATURE_LEVEL_9_1,"D3D_FEATURE_LEVEL_9_1" },
				{ D3D_FEATURE_LEVEL_9_2,"D3D_FEATURE_LEVEL_9_2" },
				{ D3D_FEATURE_LEVEL_9_3,"D3D_FEATURE_LEVEL_9_3" },
				{ D3D_FEATURE_LEVEL_10_0,"D3D_FEATURE_LEVEL_10_0" },
				{ D3D_FEATURE_LEVEL_10_1,"D3D_FEATURE_LEVEL_10_1" },
				{ D3D_FEATURE_LEVEL_11_0,"D3D_FEATURE_LEVEL_11_0" },
				{ D3D_FEATURE_LEVEL_11_1,"D3D_FEATURE_LEVEL_11_1" },
				{ D3D_FEATURE_LEVEL_12_0,"D3D_FEATURE_LEVEL_12_0" },
				{ D3D_FEATURE_LEVEL_12_1,"D3D_FEATURE_LEVEL_12_1" }
			});
			CComPtr<ID3D11Device> pDevice = nullptr;
			CComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
			HRESULT hr = S_OK;

			UINT createDeviceFlags = 0;
#ifdef _DEBUG
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			D3D_DRIVER_TYPE driverTypes[] =
			{
				D3D_DRIVER_TYPE_HARDWARE,
				D3D_DRIVER_TYPE_WARP,
				D3D_DRIVER_TYPE_REFERENCE,
			};
			UINT numDriverTypes = ARRAYSIZE(driverTypes);

			D3D_FEATURE_LEVEL featureLevels[] =
			{
				m_FeatureLevel
			};
			UINT numFeatureLevels = ARRAYSIZE(featureLevels);

			CComPtr<IDXGIFactory> pFactory = nullptr;
			hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));
			if (FAILED(hr))
			{
				throw std::exception("CreateDXGIFactory failed");
			}
			m_pDXGIFactory = pFactory;

			CComPtr<IDXGIAdapter> pAdapter = nullptr;
			std::vector<CComPtr<IDXGIAdapter>> pAdapters;
			uint32 i = 0;
			while (pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
			{
				pAdapters.push_back(pAdapter);
				++i;
				pAdapter = nullptr;
			}

			for (uint32 i = 0; i < pAdapters.size(); ++i)
			{
				for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
				{
					m_DriverType = driverTypes[driverTypeIndex];
					pDevice = nullptr;
					pDeviceContext = nullptr;
					hr = D3D11CreateDevice(
						nullptr,//pAdapters.at(i),
						m_DriverType,
						NULL,
						createDeviceFlags,
						featureLevels,
						numFeatureLevels,
						D3D11_SDK_VERSION,
						&pDevice,
						&m_FeatureLevel,
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
			if (m_FeatureLevel != D3D_FEATURE_LEVEL_11_0)
			{
				throw std::exception("FeatureLevel Not Supported");
			}

			Log("D3D11 Device Create Success. feature level:%s\n", feature_level_names.at(m_FeatureLevel));
			m_pDevice = pDevice;
			m_pImmediateContext = pDeviceContext;

#ifdef _DEBUG			
			hr = m_pDevice->QueryInterface(__uuidof(ID3D11Device), (void**)&m_pDebug);
			if (FAILED(hr))
			{
				throw std::exception("Not in debug mode");
			}
#endif

		}
		D3D11Device::~D3D11Device()
		{
			//#ifdef _DEBUG
			//		m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			//#endif
		}

	}
}