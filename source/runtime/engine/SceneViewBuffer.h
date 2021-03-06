#pragma once
#include "EngineMininal.h"
#include "SceneInterface.h"
#include "ShowFlags.h"
#include "SceneTypes.h"
#include "Classes/Engine/GameViewportClient.h"
#include "SceneManagement.h"
#include "RHIDefinitions.h"
#include "ConvexVolume.h"
#include "scene.h"
#include "ConstantBuffer.h"
#include "ShaderParameters.h"
#include "DebugViewModeHelpers.h"
#include <set>

namespace Air
{
#define VIEW_CONSTANT_BUFFER_MEMBER(type, identifier)	\
	SHADER_PARAMETER(type, identifier)

#define VIEW_CONSTANT_BUFFER_MEMBER_EX(type, identifier, precision)	\
	SHADER_PARAMETER_EX(type, identifier, precision)

#define VIEW_CONSTANT_BUFFER_MEMBER_ARRAY(type, identifier, dimension) \
	SHADER_PARAMETER_ARRAY(type, identifier, dimension)

#define VIEW_CONSTANT_BUFFER_MEMBER_TABLE	\
	VIEW_CONSTANT_BUFFER_MEMBER(Matrix, TranslatedWorldToClip)\
	VIEW_CONSTANT_BUFFER_MEMBER(Matrix, WorldToClip)	\
	VIEW_CONSTANT_BUFFER_MEMBER(Matrix, TranslatedWorldToView)\
	VIEW_CONSTANT_BUFFER_MEMBER(Matrix, ViewToTranslatedWorld)\
	VIEW_CONSTANT_BUFFER_MEMBER(Matrix, TranslatedWorldToCameraView)\
	VIEW_CONSTANT_BUFFER_MEMBER(Matrix, CameraViewToTranslatedWorld)\
	VIEW_CONSTANT_BUFFER_MEMBER(Matrix, ViewToClip)\
	VIEW_CONSTANT_BUFFER_MEMBER(Matrix, ClipToView)\
	VIEW_CONSTANT_BUFFER_MEMBER(Matrix, ClipToTranslatedWorld)\
	VIEW_CONSTANT_BUFFER_MEMBER(Matrix, SVPositionToTranslatedWorld)\
	VIEW_CONSTANT_BUFFER_MEMBER(Matrix, ScreenToWorld)\
	VIEW_CONSTANT_BUFFER_MEMBER(Matrix, ScreenToTranslatedWorld)\
	VIEW_CONSTANT_BUFFER_MEMBER_EX(float3, ViewForward, EShaderPrecisionModifier::Half)\
	VIEW_CONSTANT_BUFFER_MEMBER_EX(float3, ViewUp, EShaderPrecisionModifier::Half)\
	VIEW_CONSTANT_BUFFER_MEMBER_EX(float3, ViewRight, EShaderPrecisionModifier::Half)\
	VIEW_CONSTANT_BUFFER_MEMBER_EX(float3, HMDViewNoRollUp, EShaderPrecisionModifier::Half)\
	VIEW_CONSTANT_BUFFER_MEMBER_EX(float3, HMDViewNoRollRight, EShaderPrecisionModifier::Half)\
	VIEW_CONSTANT_BUFFER_MEMBER(float4, InvDeviceZToWorldZTransform)\
	VIEW_CONSTANT_BUFFER_MEMBER(float3, WorldCameraOrigin)\
	VIEW_CONSTANT_BUFFER_MEMBER(float3, TranslatedWorldCameraOrigin)\
	VIEW_CONSTANT_BUFFER_MEMBER(float3, WorldViewOrigin)\
	VIEW_CONSTANT_BUFFER_MEMBER(float3, PreViewTranslation)\
	VIEW_CONSTANT_BUFFER_MEMBER_EX(LinearColor, DirectionalLightColor, EShaderPrecisionModifier::Half)\
	VIEW_CONSTANT_BUFFER_MEMBER_EX(float3, DirectionalLightDirection, EShaderPrecisionModifier::Half)\
	VIEW_CONSTANT_BUFFER_MEMBER_EX(float4, ViewRectMin, EShaderPrecisionModifier::Half)\
	VIEW_CONSTANT_BUFFER_MEMBER_EX(float4, ScreenPositionScaleBias, EShaderPrecisionModifier::Half)\
	VIEW_CONSTANT_BUFFER_MEMBER(float4, ViewSizeAndInvSize)\
	VIEW_CONSTANT_BUFFER_MEMBER(float4, BufferSizeAndInvSize)\
	VIEW_CONSTANT_BUFFER_MEMBER(float4, SceneTextureMinMax)\
	VIEW_CONSTANT_BUFFER_MEMBER(LinearColor, SkyLightColor)\
	VIEW_CONSTANT_BUFFER_MEMBER(float, SkyLightParameters) \
	VIEW_CONSTANT_BUFFER_MEMBER_ARRAY(float4, SkyIrradianceEnvironmentMap, [7])\
	VIEW_CONSTANT_BUFFER_MEMBER(uint32, Random)\
	VIEW_CONSTANT_BUFFER_MEMBER(float, RealTime)\
	VIEW_CONSTANT_BUFFER_MEMBER(float, GameTime)\
	VIEW_CONSTANT_BUFFER_MEMBER(float, MotionBlurNormalizedToPixel)

	BEGIN_GLOBAL_SHADER_PARAMETER_STRUCT_WITH_CONSTRUCTOR(InstancedViewConstantShaderParameters, ENGINE_API)
		VIEW_CONSTANT_BUFFER_MEMBER_TABLE
	END_GLOBAL_SHADER_PARAMETER_STRUCT()


}