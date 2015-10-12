#ifndef __SPACE_RENDERSYSTEM_RENDERING_HPP__
#define __SPACE_RENDERSYSTEM_RENDERING_HPP__

#include "RenderSystem/Prerequisites.hpp"
#include "RenderSystem/Shared.hpp"

namespace Space
{
	struct SPACE_RENDERSYSTEM_API ViewPort
	{
		float TopLeftX;
		float TopLeftY;
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;
	};

	struct SPACE_RENDERSYSTEM_API Rect
	{
		long left;
		long top;
		long right;
		long bottom;
	};

	enum class SPACE_RENDERSYSTEM_API FillMode : uint8
	{
		WireFrame = 2,
		Solid = 3
	};

	enum class SPACE_RENDERSYSTEM_API CullMode : uint8
	{
		None = 1,
		Front = 2,
		Back = 3
	};

	struct SPACE_RENDERSYSTEM_API RasterizerDesc
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

	enum class SPACE_RENDERSYSTEM_API DepthWriteMask : uint8
	{
		Zero = 0,
		All = 1
	};

	enum class SPACE_RENDERSYSTEM_API ComparisonFunc : uint8
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
	
	enum class SPACE_RENDERSYSTEM_API StencilOp : uint8
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

	struct SPACE_RENDERSYSTEM_API DepthStincilOpDesc
	{
		StencilOp StencilFailOp;
		StencilOp StencilDepthFailOp;
		StencilOp StencilPassOp;
		ComparisonFunc StencilFunc;
	};

	struct SPACE_RENDERSYSTEM_API DepthStencilDesc
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

	enum class SPACE_RENDERSYSTEM_API Blend : uint8
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

	enum class SPACE_RENDERSYSTEM_API BlendOperator : uint8
	{
		Add = 1,
		Subtract = 2,
		RevSubtract = 3,
		Min = 4,
		Max = 5
	};
	
	struct SPACE_RENDERSYSTEM_API RenderTargetBlendDesc
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

	struct SPACE_RENDERSYSTEM_API SampleDesc
	{
		uint Count;
		uint Quality;
	};

	struct SPACE_RENDERSYSTEM_API BlendDesc
	{
		bool AlphaToCoverageEnable;
		bool IndependentBlendEnable;
		RenderTargetBlendDesc RenderTarget[8];
	};

	class CommandList;
	class PipelineState;
	class CommandQueue;

	class SPACE_RENDERSYSTEM_API CommandList : private Interface
	{
	public:
		static CommandList* Create(RenderSystem* pRenderSys);
		virtual ~CommandList();
		
		virtual void ClearDepth(DepthStencilView* pDepth, float depth) = 0;
		virtual void ClearStencil(DepthStencilView* pStencil, uint8 stencil) = 0;
		virtual void ClearRenderTargetView(RenderTarget* pTarget, Float4 clearColor) = 0;

		virtual void Reset() = 0;
		virtual void Close() = 0;
	
		virtual void SetIndexBuffer(IndexBuffer const* buffer) = 0;
		virtual void SetVertexBuffers(
			uint startSlot,uint numBuffers,VertexBuffer *const* buffer) = 0;
		virtual void SetRenderTargets(
			RenderTarget *const* targets, uint32 numTargets, DepthStencilView const* pDepth) = 0;
		virtual void SetPipelineState(PipelineState const* state) = 0;
		
		virtual void SetViewPorts(ViewPort const* pViewPorts,uint numViewPorts) = 0;
		virtual void SetScissorRects(Rect const* rects, uint32 numRects) = 0;
		
		virtual void DrawIndexed(uint startIndex,uint numPrimitive) = 0;
		virtual void DrawIndexedInstanced(
			uint startIndex,uint numPrimitive, uint startInstance, uint numInstance) = 0;
	protected:
		IndexBuffer const* m_pIndexBuffer = nullptr;
		std::vector<VertexBuffer const*> m_VertexBufferArray;
		
	private:

	};
	typedef std::shared_ptr<CommandList> CommandListPtr;
	 
	class SPACE_RENDERSYSTEM_API PipelineState : private Interface
	{
	public:
		static PipelineState* Create(RenderSystem* pRenderSys);

		virtual ~PipelineState();

		virtual void SetShaders(Shader* shader) = 0;
		virtual void SetInputLayout(InputLayout* input,
			byte const* pInputSignature, uint32 signatureSizeInBytes) = 0;
		inline void SetPrimitiveTopology(PrimitiveTopology topology)
		{
			m_PrimitiveTopology = topology;
		}
		inline void SetBlendState(BlendDesc desc)
		{
			m_BlendStateDesc = desc;
			_SetBlendState();
		}
		inline void SetRasterizerState(RasterizerDesc desc)
		{
			m_RasterizerDesc = desc;
			_SetRasterizerState();
		}
		inline void SetDepthStencilState(DepthStencilDesc desc)
		{
			m_DepthStencilDesc = desc;
			_SetDepthStencilState();
		}
	
		// seems that D3D12 use a unified multi-sample setting
		// while D3D11 set this in each texture2d (or texture3d)
		// this method takes no effect in D3D11
		inline void SetSample(SampleDesc desc)
		{
			m_SampleDesc = desc;
			_SetSample();
		}
			
		inline void SetBlendFactor(Float4 factor)
		{
			m_BlendFactor = factor;
		}
		inline void SetSampleMask(uint32 mask)
		{
			m_SampleMask = mask;
		}
		inline void SetStencilRef(uint32 ref)
		{
			m_StencilRef = ref;
		}

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

		InputLayout* m_pInput;
		byte const* m_InputSignature;
		uint32 m_LengthOfSignature;

		BlendDesc m_BlendStateDesc;
		RasterizerDesc m_RasterizerDesc;
		DepthStencilDesc m_DepthStencilDesc;
		SampleDesc m_SampleDesc;
		PrimitiveTopology m_PrimitiveTopology;
		
		Float4 m_BlendFactor = { 1.0f, 1.0f, 1.0f, 1.0f };
		uint32 m_SampleMask;
		uint32 m_StencilRef;
	private:


	};

	typedef std::shared_ptr<PipelineState> PipelineStatePtr;
}

#endif