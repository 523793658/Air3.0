
#include "RenderUtils.h"
#include "RenderResource.h"
#include "RHICommandList.h"
#include "PackedNormal.h"
namespace Air
{
	PixelFormatInfo GPixelFormats[PF_MAX] =
	{
		
		{ TEXT("unknown"),			0,			0,			0,			0,			0,				0,				0,				PF_Unknown,				PFF_None },
		{ TEXT("A32B32G32R32F"),	1,			1,			1,			16,			4,				0,				1,				PF_A32B32G32R32F,		PFF_None },
		{ TEXT("B8G8R8A8"),			1,			1,			1,			4,			4,				0,				1,				PF_B8G8R8A8,			PFF_None },
		{ TEXT("B8G8R8A8_SRGB"),	1,			1,			1,			4,			4,				0,				1,				PF_B8G8R8A8_SRGB,		PFF_SRGB },
		{ TEXT("G8"),				1,			1,			1,			1,			1,				0,				1,				PF_G8,					PFF_None },
		{ TEXT("G16"),				1,			1,			1,			2,			1,				0,				1,				PF_G16,					PFF_None },
		{ TEXT("BC1"),				4,			4,			1,			8,			3,				0,				1,				PF_BC1,				PFF_Compressed},
		{ TEXT("BC2"),				4,			4,			1,			16,			4,				0,				1,				PF_BC2,				PFF_Compressed },
		{ TEXT("BC3"),				4,			4,			1,			16,			4,				0,				1,				PF_BC3,				PFF_Compressed },
		{ TEXT("UYVY"),				2,			1,			1,			4,			4,				0,				0,				PF_UYVY,				PFF_None },
		{ TEXT("FloatRGB"),			1,			1,			1,			0,			3,				0,				1,				PF_FloatRGB,			PFF_None },
		{ TEXT("FloatRGBA"),		1,			1,			1,			8,			4,				0,				1,				PF_FloatRGBA,			PFF_None },
		{ TEXT("DepthStencil"),		1,			1,			1,			0,			1,				0,				0,				PF_DepthStencil,		PFF_None },
		{ TEXT("ShadowDepth"),		1,			1,			1,			4,			1,				0,				0,				PF_ShadowDepth,			PFF_None },
		{ TEXT("R32_FLOAT"),		1,			1,			1,			4,			1,				0,				1,				PF_R32_FLOAT,			PFF_None },
		{ TEXT("G16R16"),			1,			1,			1,			4,			2,				0,				1,				PF_G16R16,				PFF_None },
		{ TEXT("G16R16F"),			1,			1,			1,			4,			2,				0,				1,				PF_G16R16F,				PFF_None },
		{ TEXT("G16R16F_FILTER"),	1,			1,			1,			4,			2,				0,				1,				PF_G16R16F_FILTER,		PFF_None },
		{ TEXT("G32R32F"),			1,			1,			1,			8,			2,				0,				1,				PF_G32R32F,				PFF_None },
		{ TEXT("A2B10G10R10"),      1,          1,          1,          4,          4,              0,              1,				PF_A2B10G10R10,			PFF_None },
		{ TEXT("A2B10G10R10_UINT"),      1,          1,          1,          4,          4,              0,              1,				PF_A2B10G10R10_UINT,			PFF_None },
		{ TEXT("A16B16G16R16"),		1,			1,			1,			8,			4,				0,				1,				PF_A16B16G16R16,		PFF_None },
		{TEXT("A16B16G16R16_SINT"),		1,			1,			1,			8,			4,				0,				1,				PF_A16B16G16R16_SINT,		PFF_None },
		{ TEXT("A16B16G16R16F"),	1,			1,			1,			8,			4,				0,				1,				PF_A16B16G16R16F,		PFF_None },
		{ TEXT("D24"),				1,			1,			1,			4,			1,				0,				1,				PF_D24,					PFF_None },
		{ TEXT("PF_R16_UNORM"),			1,			1,			1,			2,			1,				0,				1,				PF_R16_UNORM,				PFF_None },
		{ TEXT("PF_R16F"),			1,			1,			1,			2,			1,				0,				1,				PF_R16F,				PFF_None },
		{ TEXT("PF_R16F_FILTER"),	1,			1,			1,			2,			1,				0,				1,				PF_R16F_FILTER,			PFF_None },
		{ TEXT("BC5"),				4,			4,			1,			16,			2,				0,				1,				PF_BC5,					PFF_None },
		{ TEXT("BC5_SNORM"),				4,			4,			1,			16,			2,				0,				1,				PF_BC5_SNORM,					PFF_None },
		{ TEXT("V8U8"),				1,			1,			1,			2,			2,				0,				1,				PF_V8U8,				PFF_None },
		{ TEXT("A1"),				1,			1,			1,			1,			1,				0,				0,				PF_A1,					PFF_None },
		{ TEXT("R8"),				1,			1,			1,			1,			1,				0,					0,				PF_R8,					PFF_None },
		{ TEXT("FloatR11G11B10"),	1,			1,			1,			0,			3,				0,				0,				PF_FloatR11G11B10,		PFF_None },
		{ TEXT("A8"),				1,			1,			1,			1,			1,				0,				1,				PF_A8,					PFF_None },
		{ TEXT("R32_UINT"),			1,			1,			1,			4,			1,				0,				1,				PF_R32_UINT,			PFF_None },
		{ TEXT("R32_SINT"),			1,			1,			1,			4,			1,				0,				1,				PF_R32_SINT,			PFF_None },

		// IOS Support
		{ TEXT("PVRTC2"),			8,			4,			1,			8,			4,				0,				0,				PF_PVRTC2,				PFF_None },
		{ TEXT("PVRTC4"),			4,			4,			1,			8,			4,				0,				0,				PF_PVRTC4,				PFF_None },

		{ TEXT("R16_UINT"),			1,			1,			1,			2,			1,				0,				1,				PF_R16_UINT,			PFF_None },
		{ TEXT("R16_SINT"),			1,			1,			1,			2,			1,				0,				1,				PF_R16_SINT,			PFF_None },
		{ TEXT("R16_SNORM"),			1,			1,			1,			2,			1,				0,				1,				PF_R16_SNORM,			PFF_None },
		{ TEXT("R16G16B16A16_UINT"),1,			1,			1,			8,			4,				0,				1,				PF_R16G16B16A16_UINT,	PFF_None },
		{ TEXT("R16G16B16A16_SINT"),1,			1,			1,			8,			4,				0,				1,				PF_R16G16B16A16_SINT,	PFF_None },
		{ TEXT("R5G6B5_UNORM"),     1,          1,          1,          2,          3,              0,              1,              PF_R5G6B5_UNORM,		PFF_None },
		{ TEXT("A1R5G5B5_UNORM"),     1,          1,          1,          2,          4,              0,              1,              PF_R5G5B5A1_UNORM,		PFF_None },
		{ TEXT("R8G8B8A8"),			1,			1,			1,			4,			4,				0,				1,				PF_R8G8B8A8,			PFF_None },
		{ TEXT("R8G8B8A8_SNORM"),			1,			1,			1,			4,			4,				0,				1,				PF_R8G8B8A8_SNORM,			PFF_None },
		{ TEXT("R8G8B8A8_SINT"),			1,			1,			1,			4,			4,				0,				1,				PF_R8G8B8A8_SINT,			PFF_None },
		{ TEXT("BC4"),				4,			4,			1,			8,			1,				0,				1,				PF_BC4,					PFF_Compressed},
		{ TEXT("BC4_SNORM"),				4,			4,			1,			8,			1,				0,				1,				PF_BC4_SNORM,					PFF_Compressed },
		{ TEXT("R8G8"),				1,			1,			1,			2,			2,				0,				1,				PF_R8G8,				PFF_None },
		{ TEXT("R8G8_SNORM"),				1,			1,			1,			2,			2,				0,				1,				PF_R8G8_SNORM,				PFF_None },

		{ TEXT("ATC_RGB"),			4,			4,			1,			8,			3,				0,				0,				PF_ATC_RGB,				PFF_None },
		{ TEXT("ATC_RGBA_E"),		4,			4,			1,			16,			4,				0,				0,				PF_ATC_RGBA_E,			PFF_None },
		{ TEXT("ATC_RGBA_I"),		4,			4,			1,			16,			4,				0,				0,				PF_ATC_RGBA_I,			PFF_None },
		{ TEXT("X24_G8"),			1,			1,			1,			1,			1,				0,				0,				PF_X24_G8,				PFF_None },
		{ TEXT("ETC1"),				4,			4,			1,			8,			3,				0,				0,				PF_ETC1,				PFF_Compressed},
		{ TEXT("ETC2_RGB"),			4,			4,			1,			8,			3,				0,				0,				PF_ETC2_RGB,			PFF_Compressed},
		{ TEXT("ETC2_RGBA"),		4,			4,			1,			16,			4,				0,				0,				PF_ETC2_RGBA,			PFF_Compressed},
		{ TEXT("PF_R32G32B32A32_UINT"),1,		1,			1,			16,			4,				0,				1,				PF_R32G32B32A32_UINT,	PFF_None },
		{ TEXT("PF_R16G16_UINT"),	1,			1,			1,			4,			4,				0,				1,				PF_R16G16_UINT,			PFF_None },
		{ TEXT("PF_R16G16_FLOAT"),	1,			1,			1,			4,			4,				0,				1,				PF_R16G16_FLOAT,			PFF_None },

		// ASTC support
		{ TEXT("ASTC_4x4"),			4,			4,			1,			16,			4,				0,				0,				PF_ASTC_4x4,			PFF_None },
		{ TEXT("ASTC_6x6"),			6,			6,			1,			16,			4,				0,				0,				PF_ASTC_6x6,			PFF_None },
		{ TEXT("ASTC_8x8"),			8,			8,			1,			16,			4,				0,				0,				PF_ASTC_8x8,			PFF_None },
		{ TEXT("ASTC_10x10"),		10,			10,			1,			16,			4,				0,				0,				PF_ASTC_10x10,			PFF_None },
		{ TEXT("ASTC_12x12"),		12,			12,			1,			16,			4,				0,				0,				PF_ASTC_12x12,			PFF_None },

		{ TEXT("BC6H"),				4,			4,			1,			16,			3,				0,				1,				PF_BC6H,				PFF_Compressed},
		{ TEXT("BC7"),				4,			4,			1,			16,			4,				0,				1,				PF_BC7,					PFF_Compressed},
		{ TEXT("R8_UINT"),			1,			1,			1,			1,			1,				0,				1,				PF_R8_UINT,				PFF_None },
		{ TEXT("R8_SNORM"),			1,			1,			1,			1,			1,				0,				1,				PF_R8_SNORM,				PFF_None },
		{ TEXT("BC1_SRGB"),				4,			4,			1,			8,			3,				0,				1,				PF_BC1_SRGB,					PFF_Compressed | PFF_SRGB
		},
		{ TEXT("BC2_SRGB"),				4,			4,			1,			16,			4,				0,				1,				PF_BC2_SRGB,					PFF_Compressed | PFF_SRGB
		},
		{ TEXT("BC3_SRGB"),				4,			4,			1,			16,			4,				0,				1,				PF_BC3_SRGB,					PFF_Compressed | PFF_SRGB
		},
		{ TEXT("BC4_SRGB"),				4,			4,			1,			16,			4,				0,				1,				PF_BC4_SRGB,					PFF_Compressed | PFF_SRGB
		},
		{ TEXT("BC5_SRGB"),				4,			4,			1,			16,			4,				0,				1,				PF_BC5_SRGB,					PFF_Compressed | PFF_SRGB
		},
		{ TEXT("BC7_SRGB"),				4,			4,			1,			16,			4,				0,				1,				PF_BC7_SRGB,					PFF_Compressed | PFF_SRGB
		},
		{ TEXT("R4G4B4A4"),				1,			4,			1,			2,			4,				0,				1,				PF_R4G4B4A4,					PFF_None
		}
	};

	SIZE_T calcTextureMipWidthInBlocks(uint32 width, EPixelFormat format, uint32 mipIndex)
	{
		const uint32 blockSizeX = GPixelFormats[format].BlockSizeX;
		const uint32 WidthInTixels = std::max<uint32>(width >> mipIndex, 1);
		const uint32 widthInBlocks = (WidthInTixels + blockSizeX - 1) / blockSizeX;
		return widthInBlocks;
	}

	SIZE_T calcTextureMipHeightInBlocks(uint32 height, EPixelFormat format, uint32 MipIndex)
	{
		const uint32 BlockSizeY = GPixelFormats[format].BlockSizeY;
		const uint32 HeightInTexels = std::max<uint32>(height >> MipIndex, 1);
		const uint32 HeightInBlocks = (HeightInTexels + BlockSizeY - 1) / BlockSizeY;
		return HeightInBlocks;
	}


	SIZE_T CalcTextureMipMapSize(uint32 width, uint32 height, EPixelFormat format, uint32 mipIndex)
	{
		const uint32 widthInBlocks = calcTextureMipWidthInBlocks(width, format, mipIndex);
		const uint32 heightInBlocks = calcTextureMipHeightInBlocks(height, format, mipIndex);
		return widthInBlocks * heightInBlocks * GPixelFormats[format].BlockBytes;
	}

	SIZE_T calcTextureSize(uint32 sizeX, uint32 sizeY, EPixelFormat format, uint32 MipCount)
	{
		SIZE_T size = 0;
		for (uint32 mipIndex = 0; mipIndex < MipCount; ++mipIndex)
		{
			size += CalcTextureMipMapSize(sizeX, sizeY, format, mipIndex);
		}
		return size;
	}

	void quantizeSceneBufferSize(const int2& inBufferSize, int2& outBufferSize)
	{
		const uint32 dividableBy = 4;
		const uint32 mask = ~(dividableBy - 1);
		outBufferSize.y = (inBufferSize.y + dividableBy - 1) & mask;
		outBufferSize.x = (inBufferSize.x + dividableBy - 1) & mask;
	}




	RENDER_CORE_API bool isSimpleForwardShadingEnable(EShaderPlatform platform)
	{
		return false;
	}

	


	template<int32 R, int32 G, int32 B, int32 A>
	class ColoredTexture : public Texture
	{
	public:
		virtual void initRHI() override
		{
			RHIResourceCreateInfo createInfo;
			Texture2DRHIRef texture2D = RHICreateTexture2D(1, 1, PF_B8G8R8A8, 1, 1, TexCreate_ShaderResource, createInfo);
			mTextureRHI = texture2D;

			uint32 destStride;
			Color* destBuffer = (Color*)RHILockTexture2D(texture2D, 0, RLM_WriteOnly, destStride, false);
			*destBuffer = Color(R, G, B, A);
			RHIUnlockTexture2D(texture2D, 0, false);
			SamplerStateInitializerRHI samplerStateInitializer(SF_Point, AM_Wrap, AM_Wrap, AM_Wrap);
			mSamplerStateRHI = RHICreateSamplerState(samplerStateInitializer);
		}
	};

	class SolidColorTextureCube : public Texture
	{
	public:
		SolidColorTextureCube(const Color& inColor, EPixelFormat inPixelFormat = PF_B8G8R8A8)
			:mColor(inColor)
			,mPixelFormat(inPixelFormat)
		{

		}
		virtual void initRHI() override
		{
			RHIResourceCreateInfo createInfo;
			TextureCubeRHIRef textureCube = RHICreateTextureCube(1, mPixelFormat, 1, 0, createInfo);
			mTextureRHI = textureCube;
			for (uint32 faceIndex = 0; faceIndex < 6; faceIndex++)
			{
				uint32 destStride;
				Color* destBuffer = (Color*)RHILockTextureCubeFace(textureCube, faceIndex, 0, 0, RLM_WriteOnly, destStride, false);
				*destBuffer = mColor;
				RHIUnlockTextureCubeFace(textureCube, faceIndex, 0, 0, false);
			}
			SamplerStateInitializerRHI samplerStateInitializer(SF_Point, AM_Wrap, AM_Wrap, AM_Wrap);
			mSamplerStateRHI = RHICreateSamplerState(samplerStateInitializer);
		}

		virtual uint32 getWidth() const override
		{
			return 1;
		}

		virtual uint32 getHeight() const override
		{
			return 1;
		}

	private:
		Color mColor;
		EPixelFormat mPixelFormat;
	};


	class WhiteTextureCube : public SolidColorTextureCube
	{
	public:
		WhiteTextureCube() : SolidColorTextureCube(Color::White) {}
	};
	Texture* GWhiteTextureCube = new TGlobalResource<WhiteTextureCube>();

	class BlackTextureCube : public SolidColorTextureCube
	{
	public:
		BlackTextureCube():SolidColorTextureCube(Color::Black){}
	};

	Texture* GBlackTextureCube = new TGlobalResource<BlackTextureCube>();

	Texture* GBlackTexture = new TGlobalResource<ColoredTexture<0, 0, 0, 255>>;
	Texture* GWhiteTexture = new TGlobalResource<ColoredTexture<0, 0, 0, 255>>;

	class Vector4VectorDeclaration : public RenderResource
	{
	public:
		VertexDeclarationRHIRef mVertexDeclarationRHI;
		virtual void initRHI() override
		{
			VertexDeclarationElementList elements;
			elements.add(VertexElement(0, 0, VET_Float4, 0, sizeof(float4)));
			mVertexDeclarationRHI = RHICreateVertexDeclaration(elements);
		}

		virtual void releaseRHI() override
		{
			mVertexDeclarationRHI.safeRelease();
		}
	};


	TGlobalResource<Vector4VectorDeclaration> GVector4VectorDeclaration;

	RENDER_CORE_API VertexDeclarationRHIRef& getVertexDeclarationVector4()
	{
		return GVector4VectorDeclaration.mVertexDeclarationRHI;
	}

	int2 calcMipMapExtent(uint32 textureWidth, uint32 textureHeight, EPixelFormat format, uint32 mipIndex)
	{
		return int2(Math::max<uint32>(textureWidth >> mipIndex, GPixelFormats[format].BlockSizeX), Math::max<uint32>(textureHeight >> mipIndex, GPixelFormats[format].BlockSizeY));
	}

	Archive& operator <<(Archive& ar, PackagedRGBA16N& n)
	{
		ar << n.mX;
		ar << n.mY;
		ar << n.mZ;
		ar << n.mW;
		return ar;
	}

	void copyTextureData2D(const void* source, void* dest, uint32 height, EPixelFormat format, uint32 sourceStirde, uint32 destStride)
	{
		const uint32 blockSizeY = GPixelFormats[format].BlockSizeY;
		const uint32 numBlocksY = (height + blockSizeY - 1) / blockSizeY;

		if (sourceStirde == destStride || destStride == 0)
		{
			Memory::memcpy(dest, source, numBlocksY * sourceStirde);
		}
		else
		{
			const int32 numBytesPerRow = Math::min<uint32>(sourceStirde, destStride);
			for (uint32 blockY = 0; blockY < numBlocksY; ++blockY)
			{
				Memory::memcpy(
					(uint8*)dest + destStride * blockY,
					(uint8*)source + sourceStirde * blockY,
					numBytesPerRow
				);
			}
		}
	}

	RENDER_CORE_API bool mobileSupportsGPUScene(EShaderPlatform platform)
	{
		return false;
	}

	RENDER_CORE_API bool useVirtualTexturing(ERHIFeatureLevel::Type inFeatureLevel, const ITargetPlatform* targetPlatform)
	{
		return false;
	}

	void calcMipMapExtent3D(uint32 width, uint32 height, uint32 depth, EPixelFormat format, uint32 mipIndex, uint32& outXExtent, uint32& outYExtent, uint32& outZExtent)
	{
		outXExtent = Math::max<uint32>(width >> mipIndex, GPixelFormats[format].BlockSizeX);
		outYExtent = Math::max<uint32>(height >> mipIndex, GPixelFormats[format].BlockSizeY);
		outZExtent = Math::max<uint32>(depth >> mipIndex, GPixelFormats[format].BlockSizeZ);
	}

	SIZE_T calcTextureMipMapSize3D(uint32 width, uint32 height, uint32 depth, EPixelFormat format, uint32 mipIndex)
	{
		uint32 xExtent;
		uint32 yExtent;
		uint32 zExtent;

		calcMipMapExtent3D(width, height, depth, format, mipIndex, xExtent, yExtent, zExtent);

		xExtent += GPixelFormats[format].BlockSizeX - 1;
		yExtent += GPixelFormats[format].BlockSizeY - 1;
		zExtent += GPixelFormats[format].BlockSizeZ - 1;

		const uint32 xPitch = (xExtent / GPixelFormats[format].BlockSizeX) * GPixelFormats[format].BlockBytes;
		const uint32 numRows = yExtent / GPixelFormats[format].BlockSizeY;
		const uint32 numLayers = zExtent / GPixelFormats[format].BlockSizeZ;

		return numLayers * numRows * xPitch;
	}

	SIZE_T calcTextureSize3D(uint32 width, uint32 height, uint32 depth, EPixelFormat format, uint32 mipCount)
	{
		SIZE_T size = 0;
		for (uint32 mipIndex = 0; mipIndex < mipCount; ++mipIndex)
		{
			size += calcTextureMipMapSize3D(width, height, depth, format, mipIndex);
		}
		return size;
	}

	RENDER_CORE_API uint64 GForwardShadingPlatformMask = 0;
	static_assert(SP_NumPlatforms <= sizeof(GForwardShadingPlatformMask) * 8, "GForwardShadingPlatformMask must be large enough to support all shader platform");

	RENDER_CORE_API uint64 GSelectiveBasePassOutputsPlatformMask = 0;
	static_assert(SP_NumPlatforms <= sizeof(GSelectiveBasePassOutputsPlatformMask) * 8, "GSelectiveBasePassOutputsPlatformMask must be large enough to support all shader platform");

	RENDER_CORE_API bool GPUSceneUseTexture2D(const StaticShaderPlatform platform)
	{
		if (isMobilePlatform(platform))
		{
			static TConsoleVariableData<int32>* CVar = IConsoleManager::get().FindTConsoleVariableDataInt(TEXT("r.Mobile.UseGPUSceneTexture"));
			if (platform == SP_OPENGL_ES3_1_ANDROID)
			{
				return true;
			}
			else
			{
				return (CVar && CVar->getValueOnAnyThread() != 0) ? true : false;
			}
		}
		return false;
	}
}