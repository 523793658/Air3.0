#pragma once
#include "CoreMinimal.h"
#include "RHIConfig.h"
#include "RHIDefinitions.h"
#include "PixelFormat.h"
#include "Math/Color.h"
#include "Math/Math.h"
#include "Containers/ResourceArray.h"
#include "Containers/EnumAsByte.h"
#include "Containers/ContainerAllocationPolicies.h"
#include "Containers/StaticArray.h"
namespace Air
{
	
#define CONSTANT_BUFFER_STRUCT_ALIGNMENT	16

	extern RHI_API void RHIInit(bool bHasEditorToken);

	extern RHI_API void RHIPostInit();

	extern RHI_API void RHIExit();

	RHI_API bool RHISupportsTessellation(const EShaderPlatform platform);

	extern RHI_API bool GRHISupportsRHIThread;

	extern RHI_API bool GUsingNullRHI;

	extern RHI_API bool GRHINeedsExtraDeletionLatency;

	extern RHI_API bool GSupportsDepthFetchDuringDepthTest;

	extern RHI_API int64 GTexturePoolSize;
	extern RHI_API volatile int32 GCurrentTextureMemorySize;
	extern RHI_API volatile int32 GCurrentRendertargetMemorySize;
	extern RHI_API int32 GPoolSizeVRAMPercentage;
	
	extern RHI_API ERHIFeatureLevel::Type GMaxRHIFeatureLevel;



	extern RHI_API wstring GRHIAdapterName;
	extern RHI_API wstring GRHIAdapterInternalDriverVersion;
	extern RHI_API wstring GRHIAdapterUserDriverVersion;
	extern RHI_API wstring GRHIAdapterDriverDate;
	extern RHI_API uint32 GRHIDeviceId;
	extern RHI_API uint32 GRHIDeviceRevision;

	extern RHI_API uint32 GRHIVendorId;
	extern RHI_API uint32 GRHIDeviceId;

	extern RHI_API float GProjectionSignY;

	extern RHI_API float GMinClipZ;

	extern RHI_API int32 GPoolSizeVRAMPercentage;

	extern RHI_API bool GRHISupportsAsyncTextureCreation;

	extern RHI_API bool GRHIDeviceIsAMDPreGCNArchitecture;

	extern RHI_API bool GRHISupportsHDROutputs;

	extern RHI_API bool GRHISupportsTextureStreaming;

	extern RHI_API bool GRHISupportsFirstInstance;

	extern RHI_API bool GRHISupportsParallelRHIExecute;

	extern RHI_API bool GSupportsRenderTargetFormat_PF_FloatRGBA;

	extern RHI_API bool GSupportsSeparateRenderTargetBlendState;

	// Set the RHI initialized flag.
	extern RHI_API bool GIsRHIInitialized;

	extern RHI_API EPixelFormat GRHIHDRDisplayOutputFormat;

	extern RHI_API EShaderPlatform GShaderPlatformForFeatureLevel[ERHIFeatureLevel::Num];

	extern RHI_API ERHIFeatureLevel::Type GMaxRHIFeatureLevel;

	extern RHI_API bool GSupportsDepthBoundsTest;

	extern RHI_API bool GSupportsRenderTargetWriteMask;

	extern RHI_API int32 GMaxTexture2DDemensions;

	extern RHI_API int32 GMaxTextureDepth;

	extern RHI_API int32 GMaxTextureCubeDemensions;

	extern RHI_API bool GRHISupportsMSAADepthSampleAccess;

	extern RHI_API bool GSupportsTimestampRenderQueries;

	extern RHI_API bool GSupportsResolveCubemapFaces;

	extern RHI_API int32 GMaxTextureMipCount;

	extern RHI_API int32 GMaxShadowDepthBufferSizeX;

	extern RHI_API int32 GMaxShadowDepthBufferSizeY;

	extern RHI_API EShaderPlatform GMaxRHIShaderPlatform;


	FORCEINLINE uint32 getMax2DTextureDemension()
	{
		return GMaxTexture2DDemensions;
	}

	extern RHI_API int32 GMaxTextureArrayLayers;
	FORCEINLINE uint32 getMaxTextureArrayLayers()
	{
		return GMaxTextureArrayLayers;
	}

	RHI_API bool isRHIDeviceIntel();

	RHI_API bool isRHIDeviceAMD();

	RHI_API bool isRHIDeviceNVIDIA();

	extern RHI_API void RHIPrivateBeginFrame();

	RHI_API wstring legacyShaderPlatformToShaderFormat(EShaderPlatform platform);

	extern RHI_API void getFeatureLevelName(ERHIFeatureLevel::Type inFeatureLevel, wstring& outName);

#if PLATFORM_DESKTOP
#define WITH_SLI	(1)
	extern RHI_API int32 GNumActiveGPUsForRendering;
#else
#define WITH_SLI	(0)
#define GNumActiveGPUsForRendering (1)
#endif

	enum class EResourceTransitionAccess
	{
		EReadable, //transition from write-> read
		EWritable, //transition from read -> write	
		ERWBarrier, // Mostly for UAVs.  Transition to read/write state and always insert a resource barrier.
		ERWNoBarrier, //Mostly UAVs.  Indicates we want R/W access and do not require synchronization for the duration of the RW state.  The initial transition from writable->RWNoBarrier and readable->RWNoBarrier still requires a sync
		ERWSubResBarrier, //For special cases where read/write happens to different subresources of the same resource in the same call.  Inserts a barrier, but read validation will pass.  Temporary until we pass full subresource info to all transition calls.
		EMetaData,		  // For transitioning texture meta data, for example for making readable in shaders
		EMaxAccess,
	};


	enum class EResourceTransitionPipeline
	{
		EGfxToCompute,
		EComputeToGfx,
		EGfxToGfx,
		EComputeToCompute
	};

	struct ResolveRect
	{
		int32 X1;
		int32 Y1;
		int32 X2;
		int32 Y2;
		FORCEINLINE ResolveRect(int32 inX1 = -1, int32 inY1 = -1, int32 inX2 = -1, int32 inY2 = -1)
			:X1(inX1),
			Y1(inY1),
			X2(inX2),
			Y2(inY2)
		{
		}

		FORCEINLINE ResolveRect(const ResolveRect& other)
			:X1(other.X1)
			,Y1(other.Y1)
			,X2(other.X2)
			,Y2(other.Y2)
		{}

		bool isValid() const
		{
			return X1 >= 0 && Y1 >= 0 && X2 - X1 > 0 && Y2 - Y1 > 0;
		}
	};

	struct ResolveParams
	{
		ECubeFace mCubeFace;
		ResolveRect mRect;
		int32 mMipIndex;
		int32 mSourceArrayIndex;
		int32 mDestArrayIndex;

		ResolveParams(const ResolveRect& inRect = ResolveRect(), ECubeFace inCubeFace = CubeFace_PosX, int32 inMipIndex = 0, int32 InSourceArrayIndex = 0, int32 inDestArrayIndex = 0)
			:mCubeFace(inCubeFace)
			,mRect(inRect)
			,mMipIndex(inMipIndex)
			,mSourceArrayIndex(InSourceArrayIndex)
			,mDestArrayIndex(mDestArrayIndex)
		{}

		FORCEINLINE ResolveParams(const ResolveParams& other)
			:mCubeFace(other.mCubeFace)
			,mRect(other.mRect)
			,mMipIndex(other.mMipIndex)
			,mSourceArrayIndex(other.mSourceArrayIndex)
			,mDestArrayIndex(other.mDestArrayIndex)
		{}
	};

	enum class EClearBinding
	{
		ENoneBound,
		EColorBound,
		EDepthStencilBound
	};

	struct ClearValueBinding
	{
		struct DSValue
		{
			float depth;
			uint32 stencil;
		};


		ClearValueBinding()
			:mColorBinding(EClearBinding::EColorBound)
		{
			Value.mColor[0] = 0.0f;
			Value.mColor[1] = 0.0f;
			Value.mColor[2] = 0.0f;
			Value.mColor[3] = 0.0f;
		}
		ClearValueBinding(EClearBinding NoBinding)
			:mColorBinding(NoBinding)
		{
			BOOST_ASSERT(mColorBinding == EClearBinding::ENoneBound);
		}

		explicit ClearValueBinding(const LinearColor& inClearColor)
			:mColorBinding(EClearBinding::EColorBound)
		{
			Value.mColor[0] = inClearColor.R;
			Value.mColor[1] = inClearColor.G;
			Value.mColor[2] = inClearColor.B;
			Value.mColor[3] = inClearColor.A;
		}
		explicit ClearValueBinding(float depthClearValue, uint32 StencilClearValue = 0)
			: mColorBinding(EClearBinding::EDepthStencilBound)
		{
			Value.mDSValue.depth = depthClearValue;
			Value.mDSValue.stencil = StencilClearValue;
		}

		LinearColor getClearColor() const
		{
			BOOST_ASSERT(mColorBinding == EClearBinding::EColorBound);
			return LinearColor(Value.mColor[0], Value.mColor[1], Value.mColor[2], Value.mColor[3]);
		}

		void getDepthStencil(float& outDepth, uint32 outStencil) const
		{
			BOOST_ASSERT(mColorBinding == EClearBinding::EDepthStencilBound);
			outDepth = Value.mDSValue.depth;
			outStencil = Value.mDSValue.stencil;
		}

		bool operator == (const ClearValueBinding& other) const
		{
			if (mColorBinding == other.mColorBinding)
			{
				if (mColorBinding == EClearBinding::EColorBound)
				{
					return
						Value.mColor[0] == other.Value.mColor[0] &&
						Value.mColor[1] == other.Value.mColor[1] &&
						Value.mColor[2] == other.Value.mColor[2] &&
						Value.mColor[3] == other.Value.mColor[3];

				}
				if (mColorBinding == EClearBinding::EDepthStencilBound)
				{
					return Value.mDSValue.depth == other.Value.mDSValue.depth && Value.mDSValue.stencil == other.Value.mDSValue.stencil;
				}
				return true;
			}
			return false;
		}
			
		EClearBinding mColorBinding;
		union ClearValueType
		{
			float mColor[4];
			DSValue mDSValue;
		}Value;

		static RHI_API const ClearValueBinding None;
		static RHI_API const ClearValueBinding Black;
		static RHI_API const ClearValueBinding White;
		static RHI_API const ClearValueBinding Transparent;
		static RHI_API const ClearValueBinding DepthOne;
		static RHI_API const ClearValueBinding DepthZero;
		static RHI_API const ClearValueBinding DepthNear;
		static RHI_API const ClearValueBinding DepthFar;
		static RHI_API const ClearValueBinding Green;
		static RHI_API const ClearValueBinding MidGray;
	};


	inline Matrix adjustProjectionMatrixForRHI(const Matrix& inProjectionMatrix)
	{
		ScaleMatrix clipSpaceFixScale(float3(1.0f, GProjectionSignY, 1.0f - GMinClipZ));
		TranslationMatrix clipSpaceFixTranslate(float3(0.0f, 0.0f, GMinClipZ));
		return inProjectionMatrix * clipSpaceFixScale * clipSpaceFixTranslate;
	}

	inline EShaderPlatform getFeatureLevelShaderPlatform(ERHIFeatureLevel::Type inFeatureLevel)
	{
		return GShaderPlatformForFeatureLevel[inFeatureLevel];
	}

	

	struct RHIResourceCreateInfo
	{
		RHIResourceCreateInfo()
			:mBulkData(nullptr),
			mResourceArray(nullptr),
			mClearValueBinding(LinearColor::Transparent)
		{}

		RHIResourceCreateInfo(ResourceBulkDataInterface* inBulkData)
			:mBulkData(inBulkData),
			mResourceArray(nullptr),
			mClearValueBinding(LinearColor::Transparent)
		{}

		RHIResourceCreateInfo(ResourceArrayInterface* inArrayInterface)
			:mBulkData(nullptr)
			,mResourceArray(inArrayInterface)
			,mClearValueBinding(LinearColor::Transparent)
		{}

		RHIResourceCreateInfo(const ClearValueBinding& inClearValueBinding)
			:mBulkData(nullptr)
			,mResourceArray(nullptr)
			,mClearValueBinding(inClearValueBinding)
		{}

		ResourceBulkDataInterface* mBulkData;
		ResourceArrayInterface*	mResourceArray;

		ClearValueBinding mClearValueBinding;
	};
	
	struct VRamAllocation 
	{
		VRamAllocation(uint32 inAllocationStart = 0, uint32 inAllocationSize = 0)
			:mAllocationStart(inAllocationStart)
			, mAllocationSize(inAllocationSize)
		{

		}
		bool isValid() { return mAllocationSize > 0; }
		uint32 mAllocationStart;
		uint32 mAllocationSize;
	};

	struct RHIResourceInfo
	{
		VRamAllocation mVRamAllocation;
	};

	struct SamplerStateInitializerRHI
	{
		SamplerStateInitializerRHI() {}
		SamplerStateInitializerRHI(
			ESamplerFilter inFilter,
			ESamplerAddressMode inAddressU = AM_Wrap,
			ESamplerAddressMode inAddressV = AM_Wrap,
			ESamplerAddressMode inAddressW = AM_Wrap,
			int32 inMipBias = 0,
			int32 inMaxAnisotropy = 0,
			float inMinMipLevel = 0,
			float inMaxMipLevel = FLT_MAX,
			uint32 inBorderColor = 0,
			ESamplerCompareFunction inSamplerComparisonFunctoin = SCF_Never	
		):
			mFilter(inFilter),
			mAddressU(inAddressU),
			mAddressV(inAddressV),
			mAddressW(inAddressW),
			mMipBias(inMipBias),
			mMinMipLevel(inMinMipLevel),
			mMaxMipLevel(inMaxMipLevel),
			mMaxAnisotropy(inMaxAnisotropy),
			mBorderColor(inBorderColor),
			mSamplerComparisonFunction(inSamplerComparisonFunctoin)

		{}

		TEnumAsByte<ESamplerFilter> mFilter;
		TEnumAsByte<ESamplerAddressMode> mAddressU;
		TEnumAsByte<ESamplerAddressMode> mAddressV;
		TEnumAsByte<ESamplerAddressMode> mAddressW;

		int32 mMipBias;
		float mMinMipLevel;
		float mMaxMipLevel;
		int32 mMaxAnisotropy;
		uint32 mBorderColor;
		TEnumAsByte<ESamplerCompareFunction> mSamplerComparisonFunction;
		
	};

	struct RasterizerStateInitializerRHI
	{
		ERasterizerFillMode mFillMode;
		ERasterizerCullMode mCullMode;
		float mDepthBias;
		float mSlopeScaleDepthBias;
		bool bAllowMSAA;
		bool bEnableLineAA;
	};

	struct DepthStencilStateInitializerRHI
	{
		bool bEnableDepthWrite;
		TEnumAsByte<ECompareFunction> mDepthTest;
		bool bEnableFrontFaceStencil;
		TEnumAsByte<ECompareFunction> mFrontFaceStencilTest;
		TEnumAsByte<EStencilOp> mFrontFaceStencilFailStencilOp;
		TEnumAsByte<EStencilOp> mFrontFaceDepthFailStencilOp;
		TEnumAsByte<EStencilOp> mFrontFacePassStencilOp;

		bool bEnableBackFaceStencil;
		TEnumAsByte<ECompareFunction> mBackFaceStencilTest;
		TEnumAsByte<EStencilOp> mBackFaceStencilFailStencilOp;
		TEnumAsByte<EStencilOp> mBackFaceDepthFailStencilOp;
		TEnumAsByte<EStencilOp> mBackFacePassStencilOp;

		uint8 mStencilReadMask;
		uint8 mStencilWriteMask;

		DepthStencilStateInitializerRHI(
			bool inEnableDepthWrite = true,
			ECompareFunction inDepthTest = CF_LessEqual,
			bool bInEnableFrontFaceStencil = false,
			ECompareFunction inFrontFaceStencilTest = CF_Always,
			EStencilOp inFrontFaceStencilFailStencilOp = SO_Keep,
			EStencilOp inFrontFaceDepthFailStencilOp = SO_Keep,
			EStencilOp inFrontFacePassStencilOp = SO_Keep,
			bool bInEnbaleBackFaceStencil = false,
			ECompareFunction inBackFaceStencilTest = CF_Always,
			EStencilOp inBackFaceStencilFailStencilOp = SO_Keep,
			EStencilOp inBackFaceDepthFailStencilOp = SO_Keep,
			EStencilOp inBackFacePassStencilOp = SO_Keep,
			uint8 inStencilReadMask = 0xff,
			uint8 inStencilWriteMask = 0xff
		):
			bEnableDepthWrite(inEnableDepthWrite),
			mDepthTest(inDepthTest),
			bEnableFrontFaceStencil(bInEnableFrontFaceStencil),
			mFrontFaceStencilTest(inFrontFaceStencilTest),
			mFrontFaceStencilFailStencilOp(inFrontFaceStencilFailStencilOp),
			mFrontFaceDepthFailStencilOp(inFrontFaceDepthFailStencilOp),
			mFrontFacePassStencilOp(inFrontFacePassStencilOp),
			bEnableBackFaceStencil(bInEnbaleBackFaceStencil),
			mBackFaceStencilFailStencilOp(inBackFaceStencilFailStencilOp),
			mBackFaceDepthFailStencilOp(inBackFaceDepthFailStencilOp),
			mBackFacePassStencilOp(inBackFacePassStencilOp),
			mStencilReadMask(inStencilReadMask),
			mStencilWriteMask(inStencilWriteMask)
		{}
	};

	class BlendStateInitializerRHI
	{
	public:
		struct RenderTarget
		{
			TEnumAsByte<EBlendOperation> mColorBlendOp;
			TEnumAsByte<EBlendFactor> mColorSrcBlend;
			TEnumAsByte<EBlendFactor> mColorDstBlend;
			TEnumAsByte<EBlendOperation> mAlphaBlendOp;
			TEnumAsByte<EBlendFactor> mAlphaSrcBlend;
			TEnumAsByte<EBlendFactor> mAlphaDstBlend;
			TEnumAsByte<EColorWriteMask> mColorWriteMask;

			RenderTarget(
				EBlendOperation inColorBlendOp = BO_Add,
				EBlendFactor inColorSrcBlend = BF_One,
				EBlendFactor inColorDstBlend = BF_Zero,
				EBlendOperation inAlphaBlendOp = BO_Add,
				EBlendFactor inAlphaSrcBlend = BF_One,
				EBlendFactor inAlphaDstBlend = BF_Zero,
				EColorWriteMask inColorWriteMask = CW_RGBA
			)
				:mColorBlendOp(inColorBlendOp),
				mColorSrcBlend(inColorSrcBlend),
				mColorDstBlend(inColorDstBlend),
				mAlphaBlendOp(inAlphaBlendOp),
				mAlphaSrcBlend(inAlphaSrcBlend),
				mAlphaDstBlend(inAlphaDstBlend),
				mColorWriteMask(inColorWriteMask)
			{

			}
		};

		TStaticArray<RenderTarget, MaxSimultaneousRenderTargets> mRenderTargets;
		bool bUseIndependentRenderTargetBlendStates;

		BlendStateInitializerRHI() {}
		BlendStateInitializerRHI(const RenderTarget& inRenderTargetBlendState)
			:bUseIndependentRenderTargetBlendStates(false)
		{
			mRenderTargets[0] = inRenderTargetBlendState;
		}

		template<uint32 NumRenderTargets>
		BlendStateInitializerRHI(const TStaticArray<RenderTarget, NumRenderTargets>& inRenderTargetBlendStates)
			: bUseIndependentRenderTargetBlendStates(NumRenderTargets > 1)
		{
			static_assert(NumRenderTargets <= MaxSimultaneousRenderTargets, "Too may render target blend states.");

			for (uint32 renderTargetIndex = 0; renderTargetIndex < NumRenderTargets; ++renderTargetIndex)
			{
				mRenderTargets[renderTargetIndex] = inRenderTargetBlendStates[renderTargetIndex];
			}
		}


	};

	struct VertexElement 
	{
		uint8 mStreamIndex;
		uint8 mOffset;
		TEnumAsByte<EVertexElementType> mType;
		uint8 mAttributeIndex;
		uint16 mStride;

		uint16 bUseInstanceIndex;
		VertexElement() {}
		VertexElement(uint8 inStreamIndex, uint8 inOffset, EVertexElementType inType, uint8 inAttributeIndex, uint16 inStride, bool bInUseInstanceIndex = false)
			:
			mStreamIndex(inStreamIndex),
			mOffset(inOffset),
			mType(inType),
			mAttributeIndex(inAttributeIndex),
			mStride(inStride),
			bUseInstanceIndex(bInUseInstanceIndex)
		{

		}

		void operator = (const VertexElement& other)
		{
			mStreamIndex = other.mStreamIndex;
			mOffset = other.mOffset;
			mType = other.mType;
			mAttributeIndex = other.mAttributeIndex;
			bUseInstanceIndex = other.bUseInstanceIndex;
		}

		friend Archive& operator << (Archive& ar, VertexElement& element)
		{
			ar << element.mStreamIndex;
			ar << element.mOffset;
			ar << element.mType;
			ar << element.mAttributeIndex;
			ar << element.mStride;
			ar << element.bUseInstanceIndex;
			return ar;
		}
	};

	typedef TArray<VertexElement, TFixedAllocator<MaxVertexElementCount>> VertexDeclarationElementList;
	
	struct StreamOutElement
	{
		uint32 mStream;
		const ANSICHAR* mSemanticName;
		uint32 mSementicIndex;
		uint8 mStartComponent;
		uint8 mComponentCount;
		uint8 mOutputSlot;
		StreamOutElement(){}
		StreamOutElement(uint32 inStream, const ANSICHAR* inSemanticName, uint32 inSemanticIndex, uint8 inComponentCount, uint8 inOutputSlot) :
			mStream(inStream),
			mSemanticName(inSemanticName),
			mSementicIndex(inSemanticIndex),
			mStartComponent(0),
			mComponentCount(inComponentCount),
			mOutputSlot(inOutputSlot)
		{}
	};

	typedef TArray<StreamOutElement> StreamOutElementList;

	struct ViewportBounds
	{
		float mTopLeftX;
		float mTopLeftY;
		float mWidth;
		float mHeight;
		float mMinDepth;
		float mMaxDepth;

		ViewportBounds() {}

		ViewportBounds(float inTopLeftX, float inTopLeftY, float inWidth, float inHeight, float inMinDepth = 0.0f, float inMaxDepth = 1.0f)
			:mTopLeftX(inTopLeftX)
			,mTopLeftY(inTopLeftY)
			,mWidth(inWidth)
			,mHeight(inHeight)
			,mMinDepth(inMinDepth)
			,mMaxDepth(inMaxDepth)
		{}

		friend Archive& operator<<(Archive& ar, ViewportBounds& viewportBounds)
		{
			ar << viewportBounds.mTopLeftX;
			ar << viewportBounds.mTopLeftY;
			ar << viewportBounds.mWidth;
			ar << viewportBounds.mHeight;
			ar << viewportBounds.mMinDepth;
			ar << viewportBounds.mMaxDepth;
			return ar;
		}
	};

#define GETSAFERHISHADER_HULL(shader) (shader ? shader->getHullShader() : (HullShaderRHIParamRef)HullShaderRHIRef())

#define GETSAFERHISHADER_VERTEX(shader) (shader ? shader->getVertexShader() : (VertexShaderRHIParamRef)VertexShaderRHIRef())

#define GETSAFERHISHADER_DOMAIN(shader) (shader ? shader->getDomainShader() : (DomainShaderRHIParamRef)DomainShaderRHIRef())

#define GETSAFERHISHADER_GEOMETRY(shader) (shader ? shader->getGeometryShader() : (GeometryShaderRHIParamRef)GeometryShaderRHIRef())

#define GETSAFERHISHADER_PIXELSHADER(shader) (shader ? shader->getPixelShader() : (PixelShaderRHIParamRef)PixelShaderRHIRef())


}