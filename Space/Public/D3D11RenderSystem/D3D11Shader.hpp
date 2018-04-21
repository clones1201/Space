#pragma once

#include "D3D11RenderSystem/D3D11Prerequisites.hpp" 
#include "D3D11RenderSystem/D3D11Device.hpp"

namespace Space {
	namespace Render {

		class ShaderSource;
		class ShaderMacro;

		class SPACE_D3D11_API D3D11VertexShader:
			public SharedPtrObject<D3D11VertexShader>
		{
		public:
			CComPtr<ID3D11VertexShader> GetShader() const;

			static std::vector<byte> Compile(ShaderSource const& source);

			D3D11VertexShader(D3D11Device* device,
				std::vector<byte> byteCodes);
			virtual ~D3D11VertexShader();
		private:
			CComPtr<ID3D11VertexShader> m_pShader = nullptr;
			D3D11Device* m_Device = nullptr;
		};


		class SPACE_D3D11_API D3D11PixelShader : 
			public SharedPtrObject<D3D11PixelShader>
		{
		public:
			CComPtr<ID3D11PixelShader> GetShader() const;

			static std::vector<byte> Compile(ShaderSource const& source);

			D3D11PixelShader(D3D11Device* device, 
				std::vector<byte> byteCodes);
			virtual ~D3D11PixelShader();
		protected:

		private:
			CComPtr<ID3D11PixelShader> m_pShader = nullptr;
			D3D11Device* m_Device = nullptr;
		};

	}
}