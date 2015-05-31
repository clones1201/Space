#ifndef __SPACE_RENDERING_HPP__
#define __SPACE_RENDERING_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	struct ViewPort
	{
		float TopLeftX;
		float TopLeftY;
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;
	};

	struct Rect
	{
		long left;
		long top;
		long right;
		long bottom;
	};

	enum class FillMode
	{
		WireFrame = 2,
		Solid = 3
	};

	enum class CullMode
	{
		None = 1,
		Front = 2,
		Back = 3
	};

	struct RasterizerDesc
	{
		FillMode FillMode;
		CullMode CullMode;
		bool FrontCounterClockwise;
		int32 DepthBias;
		float DepthBiasClamp;
		float SlopeScaledDepthBias;
		bool DepthClipEnable;
		bool ScissorEnable;
		bool MultisampleEnable;
		bool AntialiasedLineEnable;
	};

	enum class DepthWriteMask
	{
		Zero = 0,
		All = 1
	};

	enum class ComparisonFunc
	{
		Never = 1,
		Less = 2,
		Equal = 3,
		LessEqual = 4,
		Greater = 5,
		NotEqual = 6,
		GreaterEqual = 7,
		Always = 8
	};
	
	enum StencilOp
	{
		Keep = 1,
		Zero = 2,
		Replace = 3,
		IncrSat = 4,
		DecrSat = 5,
		Invert = 6,
		Incr = 7,
		Decr = 8
	};

	struct DepthStincilOpDesc
	{
		StencilOp StencilFailOp;
		StencilOp StencilDepthFailOp;
		StencilOp StencilPassOp;
		ComparisonFunc StencilFunc;
	};

	struct DepthStencilDesc
	{
		bool DepthEnable;
		DepthWriteMask DepthWriteMask;
		ComparisonFunc DepthFunc;
		bool StencilEnable;
		uint8 StencilReadMask;
		uint8 StencilWriteMask;
		DepthStincilOpDesc FrontFace;
		DepthStincilOpDesc BackFace;
	};

	enum class Blend
	{
		Zero = 1,
		One = 2,
		SrcColor = 3,
		InvSrcColor = 4,
		SrcAlpha = 5,
		InvSrcAlpha = 6,
		DestAlpha = 7,
		InvDestAlpha = 8,
		DestColor = 9,
		InvDestColor = 10,
		SrcAlphaSat = 11,
		BlendFactor = 14,
		InvBlendFactor = 15,
		Src1Color = 16,
		InvSrc1Color = 17,
		Src1Alpha = 18,
		InvSrc1Alpha = 19
	};

	enum class BlendOperator
	{
		Add = 1,
		Subtract = 2,
		RevSubtract = 3,
		Min = 4,
		Max = 5
	};
	
	struct RenderTargetBlendDesc
	{
		bool BlendEnable;
		Blend SrcBlend;
		Blend DestBlend;
		BlendOperator BlendOp;
		Blend SrcBlendAlpha;
		Blend DestBlendAlpha;
		BlendOperator BlendOpAlpha;
		uint8 RenderTargetWriteMask;
	};

	struct SampleDesc
	{
		uint Count;
		uint Quality;
	};

	struct BlendDesc
	{
		bool AlphaToCoverageEnable;
		bool IndependentBlendEnable;
		RenderTargetBlendDesc RenderTarget[8];
	};

	class CommandList;
	class PipelineState;
	class CommandQueue;

	class CommandList : virtual public Uncopyable
	{
	public:
		static CommandList* Create(RenderSystem* pRenderSys);
		virtual ~CommandList();
		
		void ClearDepth();
		void ClearStencil();
		void ClearRenderTargetView(RenderTarget* pTarget, Float4 clearColor);

		void Reset();
		void Close();
	
		void SetIndexBuffer(IndexBuffer* buffer);
		void SetVertexBuffers(uint startSlot,uint numBuffers,VertexBuffer** buffer);
		void SetRenderTargets(RenderTarget const* targets, uint32 numTargets, DepthStencilView* pDepth);
		void SetPipelineState(PipelineState const* state);
		
		void SetViewPorts(ViewPort const* pViewPorts,uint numViewPorts);
		void SetScissorRects(Rect const* rects, uint32 numRects);
		
		void DrawIndexed();
		void DrawInstanced();
		void DrawIndexedInstanced();
	protected:
		virtual void _ClearDepthStencilView() = 0;
		virtual void _ClearRenderTargetView(RenderTarget* pTarget, Float4 clearColor) = 0;
		virtual void _Reset() = 0;
		virtual void _Close() = 0;
		virtual void _SetIndexBuffer() = 0; 
		virtual void _SetViewPorts(ViewPort const* pViewPorts, uint numViewPorts) = 0;
		virtual void _SetScissorRects(Rect const* rects, uint32 numRects) = 0;
		virtual void _SetPipelineState(PipelineState const* state) = 0;
		virtual void _SetRenderTargets(RenderTarget const* targets, uint32 numTargets) = 0;
		virtual void _DrawIndexed() = 0;
		virtual void _DrawInstanced() = 0;
		virtual void _DrawIndexedInstanced() = 0;

		IndexBuffer* m_pIndexBuffer;
		VertexBuffer* m_pVertexBuffer;

	private:

	};
	 
	class PipelineState
	{
	public:
		static PipelineState* Create(RenderSystem* pRenderSys);

		virtual ~PipelineState();

		virtual void SetShaders(Shader* shader) = 0;
		virtual void SetInputLayout(InputLayout* input) = 0;
		void SetBlendState(BlendDesc desc);
		void SetRasterizerState(RasterizerDesc desc);
		void SetDepthStencilState(DepthStencilDesc desc);
		// seems that D3D12 use a unified multi-sample setting
		// while D3D11 set this in each texture2d (or texture3d)
		// this method takes no effect in D3D11
		void SetSample(SampleDesc desc);

		void SetBlendState(BlendDesc desc) const;
		void SetRasterizerState(RasterizerDesc desc);
		void SetDepthStencilState(DepthStencilDesc desc);
		// seems that D3D12 use a unified multi-sample setting
		// while D3D11 set this in each texture2d (or texture3d)
		// this method takes no effect in D3D11
		void SetSample(SampleDesc desc);
			
		void SetBlendFactor(Float4 factor);
		void SetSampleMask(uint32 mask);
		void SetStencilRef(uint32 ref);

		void ClearAllState();
	protected: 
		PipelineState();
		PipelineState(PipelineState const& other) = default;
		PipelineState& operator=(PipelineState const& other) = default;

		virtual void _ClearAllState() = 0;

		virtual void _SetBlendState() = 0;
		virtual void _SetRasterizerState() = 0;
		virtual void _SetDepthStencilState() = 0;
		virtual void _SetSample() = 0;

		BlendDesc m_BlendStateDesc;
		RasterizerDesc m_RasterizerDesc;
		DepthStencilDesc m_DepthStencilDesc;
		SampleDesc m_SampleDesc;
		PrimitiveTopology m_PrimTopology;
		
		Float4 m_BlendFactor = { 1.0f, 1.0f, 1.0f, 1.0f };
		uint32 m_SampleMask;
		uint32 m_StencilRef;
	private:


	};

}

#endif