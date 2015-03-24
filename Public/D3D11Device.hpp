#ifndef __SPACE_D3D11_DEVICE_HPP__
#define __SPACE_D3D11_DEVICE_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "D3D11Prerequisites.hpp"

namespace Space
{
	class D3D11Device : private Uncopyable
	{
	public:
		D3D11Device(D3D11Device &&param);
		D3D11Device& operator=(D3D11Device&& param);

		ID3D11Device* Get() const throw();
		ID3D11DeviceContext* GetImmediateContext() const throw();
		ID3D11Device* operator->() const throw();
		IDXGIFactory* GetDXGIFactory() const throw();

		bool IsValid() const throw();
		
		~D3D11Device();

		D3D11Device() throw();
	private:
		class Impl;
		std::unique_ptr<Impl> impl;

		friend class D3D11RenderSystem;
		friend class D3D11RenderSystemImpl;
	};
}

#endif