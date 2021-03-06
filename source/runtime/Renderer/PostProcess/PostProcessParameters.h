#pragma once
#include "CoreMinimal.h"
#include "RHIStaticStates.h"
#include "ShaderCore.h"
#include "ShaderParameters.h"
namespace Air
{
	enum EPassOutputId
	{
		ePId_Output0,
		ePId_Output1,
		ePId_Output2,
		ePId_Output3,
		ePId_Output4,
		ePId_Output5,
		ePId_Output6,
		ePId_Output7,

	};

	enum EPassInputId
	{
		ePId_Input0,
		ePId_Input1,
		ePId_Input2,
		ePId_Input3,
		ePId_Input4,
		ePId_Input5,
		ePId_Input6,
		ePId_Input_Max,

	};

	enum EFallbackColor
	{
		eFC_0000,
		eFC_1111,
		eFC_0001
	};
	struct RenderingCompositePassContext;
	struct PostProcessPassParameters
	{
		void bind(const ShaderParameterMap& parameterMap);

		void setPS(RHIPixelShader* shaderRHI, const RenderingCompositePassContext& context, RHISamplerState* filter = TStaticSamplerState<>::getRHI(), EFallbackColor fallbackColor = eFC_0000, RHISamplerState** filterOverrideArray = 0);

		template<typename TRHICmdList>
		void setCS(RHIComputeShader* shaderRHI, const RenderingCompositePassContext& context, TRHICmdList& RHICmdList, RHISamplerState* filter = TStaticSamplerState<>::getRHI(), EFallbackColor fallbackColor = eFC_0000, RHISamplerState** filterOverrideArray = nullptr);

		void setVS(RHIVertexShader* shaderRHI, const RenderingCompositePassContext& context, RHISamplerState* filter = TStaticSamplerState<>::getRHI(), EFallbackColor fallbackColor = eFC_0000, RHISamplerState** filterOverrideArray = nullptr);

		friend Archive& operator << (Archive& ar, PostProcessPassParameters& p);


		template<typename ShaderRHIParamRef, typename TRHICmdList>
		void set(ShaderRHIParamRef shaderRHI, const RenderingCompositePassContext& context, TRHICmdList& RHICmdList, RHISamplerState* filter, EFallbackColor fallbackColor, RHISamplerState** filterOverrideArray = nullptr);
	private:
		ShaderParameter mViewportSize;
		ShaderParameter mViewportRect;
		ShaderResourceParameter mPostprocessInputParameter[ePId_Input_Max];
		ShaderResourceParameter mPostprocessInputParameterSampler[ePId_Input_Max];
		ShaderParameter mPostprocessInputSizeParameter[ePId_Input_Max];
		ShaderParameter mPostprocessInputMinMaxParameter[ePId_Input_Max];
		ShaderParameter mScreenPosToPixel;
		ShaderResourceParameter mBilinearTextureSampler0;
		ShaderResourceParameter mBilinearTextureSampler1;

	};
}