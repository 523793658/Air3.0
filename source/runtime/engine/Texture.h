#pragma once
#include "EngineMininal.h"
#include "RenderResource.h"
#include "ResLoader/ResLoader.h"
#include "Object.h"
#include "Containers/IndirectArray.h"
#include "TextureResource.h"

namespace Air
{
	struct TexturePlatformData
	{
		int32 mWidth;
		int32 mHeight;
		int32 mNumSlices;
		EPixelFormat mPixelFormat;
		TindirectArray<struct Texture2DMipMap> mMips;

		ENGINE_API TexturePlatformData();

		ENGINE_API ~TexturePlatformData();

		bool tryLoadMips(int32 firstMipToLoad, void** outMipData);

		void serialize(Archive& ar, class RTexture* owner);

		
	};


	class RTexture : public Object
	{
		GENERATED_RCLASS_BODY(RTexture, Object);
	public:

		enum TextureType
		{
			TT_1D,
			TT_2D,
			TT_3D,
			TT_Cube
		};

		enum CubeFaces
		{
			CF_Positive_X = 0,
			CF_Negative_X = 1,
			CF_Positive_Y = 2,
			CF_Negative_Y = 3,
			CF_Positive_Z = 4,
			CF_Negative_Z = 5,
		};


	public:
		ENGINE_API static ResLoadingDescPtr createLoadingDesc(wstring const & path);

		ENGINE_API virtual void postLoad() override;

		ENGINE_API virtual void updateResource();

		ENGINE_API virtual void releaseResource();

		virtual TextureResource* createResource() PURE_VIRTRUAL(RTexture::createResource, return nullptr;);

		
	public:
		class TextureResource* mResource;
		TextureReference mTextureReference;

		std::shared_ptr<class TextureData> mTextureData;

		uint32 bSRGB : 1;
	};
}