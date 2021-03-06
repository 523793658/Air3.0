#include "StaticMeshResources.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "StaticMeshVertexData.h"
#include "MeshUtilities/MeshUtilities.h"
#include "Modules/ModuleManager.h"
#include "Math/Color.h"
#include "Serialization/MemoryWriter.h"
#include "Classes/Materials/Material.h"
namespace Air
{

	bool GForceDefaultMaterial = false;

	Archive& operator<<(Archive& ar, StaticMeshSection& section)
	{
		ar << section.mMaterialIndex;
		ar << section.mFirstIndex;
		ar << section.mNumTriangles;
		ar << section.mMinVertexIndex;
		ar << section.mMaxVertexIndex;
		ar << section.bEnableCollision;
		ar << section.bCastShadow;
		return ar;
	}

#if WITH_EDITOR
	static float calculateViewDistance(float maxDeviation, float allowedPixelError)
	{
		const float viewDistance = (maxDeviation* 960.0f) / Math::max(allowedPixelError, RStaticMesh::mMinimumAutoLODPixelError);
		return viewDistance;
	}
#endif



	StaticMeshSceneProxy::StaticMeshSceneProxy(StaticMeshComponent* component, bool bCanLODsShareStaticLighting)
		:PrimitiveSceneProxy(component, component->getStaticMesh()->getName())
		,mOwner(component->getOwner())
		,mStaticMesh(component->getStaticMesh())
		,mRenderData(component->getStaticMesh()->mRenderData.get())
		,bCastShadow(component->bCastShadow)
		,bReverseCulling(component->bReverseCulling)
	{



		bool bAnySectionCastShadows = false;
		mLODs.empty(mRenderData->mLODResources.size());

		const auto featureLevel = getScene().getFeatureLevel();

		const int32 smCurrentMinLod = component->getStaticMesh()->mMinLOD;


		for (int32 lodIndex = 0; lodIndex < mRenderData->mLODResources.size(); lodIndex++)
		{
			LODInfo* newLodInfo = new (mLODs)LODInfo(component, lodIndex, bCanLODsShareStaticLighting);
			const int32 numSections = newLodInfo->mSections.size();
			for (int32 sectionIndex = 0; sectionIndex < numSections; ++sectionIndex)
			{
				const LODInfo::SectionInfo& sectionInfo = newLodInfo->mSections[sectionIndex];
				bAnySectionCastShadows |= mRenderData->mLODResources[lodIndex].mSections[sectionIndex].bCastShadow;
				if (sectionInfo.mMaterial == RMaterial::getDefaultMaterial(MD_Surface))
				{
					mMaterialRelevance |= RMaterial::getDefaultMaterial(MD_Surface)->getRelevance(featureLevel);
				}
			}
		}

		mClampedMinLOD = 0;

		bVFRequiresPrimitiveConstantBuffer = !useGPUScene(GMaxRHIShaderPlatform, featureLevel);

		
	}



	StaticMeshSceneProxy::LODInfo::LODInfo(const StaticMeshComponent* inComponent, int32 lodIndex, bool bCanLODsShareStaticLighting)
		:LightCacheInterface(nullptr, nullptr)
		,mOverrideColorVertexBuffer(nullptr)
		,mPreCulledIndexBuffer(nullptr)
		,bUsesMeshModifyingMaterials(false)
	{
		const auto featureLevel = inComponent->getWorld()->mFeatureLevel;
		StaticMeshRenderData* meshRenderData = inComponent->getStaticMesh()->mRenderData.get();
		StaticMeshLODResources& lodModel = meshRenderData->mLODResources[lodIndex];

		mSections.empty(meshRenderData->mLODResources[lodIndex].mSections.size());
		for (int32 sectionIndex = 0; sectionIndex < lodModel.mSections.size(); sectionIndex++)
		{
			const StaticMeshSection& section = lodModel.mSections[sectionIndex];
			SectionInfo sectionInfo;
			sectionInfo.mMaterial = inComponent->getMaterial(section.mMaterialIndex);
#if WITH_EDITORONLY_DATA
			sectionInfo.mMaterialIndex = section.mMaterialIndex;
#endif
			if (GForceDefaultMaterial && sectionInfo.mMaterial && !isTranslucentBlendMode(sectionInfo.mMaterial->getBlendMode()))
			{
				sectionInfo.mMaterial = RMaterial::getDefaultMaterial(MD_Surface);
			}

			if (!sectionInfo.mMaterial)
			{
				sectionInfo.mMaterial = RMaterial::getDefaultMaterial(MD_Surface);
			}

			const bool bRequiresAdjacencyInformation = false;

			mSections.add(sectionInfo);

		}
	}


	StaticMeshLODResources::StaticMeshLODResources()
		:bHasAdjacencyInfo(false)
		,bHasDepthOnlyIndices(false)
		,bHasReversedIndices(false)
		,bHasreversedDepthOnlyIndices(false)
		,mDepthOnlyNumTriangles(0)
	{

	}



	StaticMeshLODResources::~StaticMeshLODResources()
	{
		
	}

	void StaticMeshLODResources::serialize(Archive& ar, Object* owner, int32 idx)
	{
		RStaticMesh* ownerStaticMesh = check_cast<RStaticMesh*>(owner);
		bool bMeshCPUAccess = ownerStaticMesh ? ownerStaticMesh->bAllowCPUAccess : false;
		bool bNeedsCPUAccess = bMeshCPUAccess;

		bHasAdjacencyInfo = false;
		bHasDepthOnlyIndices = false;
		bHasReversedIndices = false;
		bHasreversedDepthOnlyIndices = false;
		mDepthOnlyNumTriangles = 0;
		
		const uint8 adjacencyDataStripFlag = 1;
		
		mIndexBuffer.serialize(ar, bNeedsCPUAccess);
		
		mDepthOnlyIndexBuffer.serialize(ar, bNeedsCPUAccess);

		bHasDepthOnlyIndices = mDepthOnlyIndexBuffer.getNumIndices() != 0;
		mDepthOnlyNumTriangles = mDepthOnlyIndexBuffer.getNumIndices() / 3;
	}

	
	void StaticMeshVertexBuffer::initRHI()
	{
		_TangentsVertexBuffer.mVertexBufferRHI = createTangentsRHIBuffer_RenderThread();
		_TexcoordVertexBuffer.mVertexBufferRHI = createTexcoordRHIBuffer_RenderThread();

		if (_TangentsVertexBuffer.mVertexBufferRHI && RHISupportsManualVertexFetch(GMaxRHIShaderPlatform))
		{
			mTangentSRV = RHICreateShaderResourceView(
				mTangentsData ? _TangentsVertexBuffer.mVertexBufferRHI : nullptr,
				getUseHighPrecisionTangentBasis() ? 8 : 4,
				getUseHighPrecisionTangentBasis() ? PF_R16G16B16A16_SINT : PF_R8G8B8A8_SINT
			);
		}

		if (_TexcoordVertexBuffer.mVertexBufferRHI && RHISupportsManualVertexFetch(GMaxRHIShaderPlatform))
		{
			mTextureCoordinateSRV = RHICreateShaderResourceView(
				mTexcoordData ? _TexcoordVertexBuffer.mVertexBufferRHI : nullptr,
				getUseFullPrecisionUVs() ? 8 : 4,
				getUseFullPrecisionUVs() ? PF_G32R32F : PF_G16R16F
			);
		}
	}

	void StaticMeshVertexBuffer::cleanUp()
	{
		if (mTangentsData)
		{
			delete mTangentsData;
			mTangentsData = nullptr;
		}
		if (mTexcoordData)
		{
			delete mTexcoordData;
			mTexcoordData = nullptr;
		}
	}

	bool StaticMeshSceneProxy::isReversedCullingNeeded(bool bUseReversedIndices) const
	{
		return (bReverseCulling || isLocalToWorldDeterminantNegative()) && !bUseReversedIndices;
	}

	void StaticMeshVertexBuffer::allocateData(bool bNeedsCPUAccess /* = true */)
	{
		cleanUp();
		uint8 vertexStride = 0;
		if (getUseHighPrecisionTangentBasis())
		{
			typedef TStaticMeshVertexTangentDatum<typename TStaticMeshVertexTangentTypeSelector<EStaticMeshVertexTangentBasisType::HighPrecision>::TangentTypeT> TangentType;
			mTangentsStride = sizeof(TangentType);
			mTangentsData = new TStaticMeshVertexData<TangentType>(bNeedsCPUAccess);
		}
		else
		{
			typedef TStaticMeshVertexTangentDatum<typename TStaticMeshVertexTangentTypeSelector<EStaticMeshVertexTangentBasisType::Default>::TangentTypeT> TangentType;
			mTangentsStride = sizeof(TangentType);
			mTangentsData = new TStaticMeshVertexData<TangentType>(bNeedsCPUAccess);
		}

		if (getUseFullPrecisionUVs())
		{
			typedef TStaticMeshVertexUVsDatum<typename TStaticMeshVertexUVsTypeSelector<EStaticMeshVertexUVType::HighPrecision>::UVsTypeT> UVType;
			mTangentsStride = sizeof(UVType);
			mTangentsData = new TStaticMeshVertexData<UVType>(bNeedsCPUAccess);
		}
		else
		{
			typedef TStaticMeshVertexUVsDatum<typename TStaticMeshVertexUVsTypeSelector<EStaticMeshVertexUVType::Default>::UVsTypeT> UVType;
			mTangentsStride = sizeof(UVType);
			mTangentsData = new TStaticMeshVertexData<UVType>(bNeedsCPUAccess);
		}
	}

	void StaticMeshVertexBuffer::serialize(Archive& ar, bool needsCPUAccess)
	{
		bNeedCPUAccess = needsCPUAccess;
		serializeMetaData(ar);
		if (ar.isLoading())
		{
			allocateData(bNeedCPUAccess);
		}
		if (ar.mArIsCountingMemory)
		{
			if (mTangentsData != nullptr)
			{
				mTangentsData->serialize(ar);

				mTangentDataPtr = mNumVertices ? mTangentsData->getDataPointer() : nullptr;
			}
			if (mTexcoordData != nullptr)
			{
				mTexcoordData->serialize(ar);
				mTexcoordDataPtr = mNumVertices ? mTexcoordData->getDataPointer() : nullptr;
			}

			if (mNumVertices && !getUseFullPrecisionUVs() && !GVertexElementTypeSupport.isSupported(VET_Half2))
			{
				convertHalfTexcoordsToFloat(nullptr);
			}
		}
	}

	void StaticMeshVertexBuffer::convertHalfTexcoordsToFloat(const uint8* inData)
	{
		BOOST_ASSERT(mTexcoordData);

		setUseFullPrecisionUVs(true);

		StaticMeshVertexDataInterface* originalTexcoordData = mTexcoordData;

		typedef TStaticMeshVertexUVsDatum<typename TStaticMeshVertexUVsTypeSelector<EStaticMeshVertexUVType::HighPrecision>::UVsTypeT> UVType;

		mTexcoordData = new TStaticMeshVertexData<UVType>(originalTexcoordData->getAllowCPUAccess());

		mTexcoordData->resizeBuffer(mNumVertices * getNumTexCoords());

		mTexcoordDataPtr = mTexcoordData->getDataPointer();

		mTangentsStride = sizeof(UVType);

		float2* destTexcoordDataPtr = (float2*)mTexcoordDataPtr;
		half2* sourceTexcoordDataPtr = (half2*)(inData ? inData : originalTexcoordData->getDataPointer());
		for (int i = 0; i < mNumVertices * getNumTexCoords(); i++)
		{
			*destTexcoordDataPtr++ = *sourceTexcoordDataPtr++;
		}
		delete originalTexcoordData;
		originalTexcoordData = nullptr;
	}

	void StaticMeshVertexBuffer::init(const TArray<StaticMeshBuildVertex>& inVertices, uint32 inNumTexCoords, bool needCPUAccess)
	{
		mNumTexcoords = inNumTexCoords;
		mNumVertices = inVertices.size();
		bNeedCPUAccess = needCPUAccess;
		allocateData();
	
		mTangentsData->resizeBuffer(mNumVertices);
		mTangentDataPtr = mNumVertices ? mTangentsData->getDataPointer() : nullptr;

		mTexcoordData->resizeBuffer(mNumVertices * getNumTexCoords());
		mTexcoordDataPtr = mNumVertices ? mTexcoordData->getDataPointer() : nullptr;

	}

	void StaticMeshVertexBuffer::bindTangentVertexBuffer(const VertexFactory* vertexFactory, StaticMeshDataType& data) const
	{
		{
			data.mTangetsSRV = mTangentSRV;
		}
		{
			uint32 tangentSizeInBytes = 0;
			uint32 tangentXOffset = 0;
			uint32 tangentYOffset = 0;
			EVertexElementType tangentElemType = VET_None;

			if (getUseHighPrecisionTangentBasis())
			{
				typedef TStaticMeshVertexTangentDatum<typename TStaticMeshVertexTangentTypeSelector<EStaticMeshVertexTangentBasisType::HighPrecision>::TangentTypeT> TangentType;
				tangentElemType = TStaticMeshVertexTangentTypeSelector<EStaticMeshVertexTangentBasisType::HighPrecision>::VertexElementType;
				tangentXOffset = STRUCT_OFFSET(TangentType, mTangentX);
				tangentYOffset = STRUCT_OFFSET(TangentType, mTangentY);
				tangentSizeInBytes = sizeof(TangentType);
			}
			else
			{
				typedef TStaticMeshVertexTangentDatum<typename TStaticMeshVertexTangentTypeSelector<EStaticMeshVertexTangentBasisType::Default>::TangentTypeT> TangentType;
				tangentElemType = TStaticMeshVertexTangentTypeSelector<EStaticMeshVertexTangentBasisType::Default>::VertexElementType;
				tangentXOffset = STRUCT_OFFSET(TangentType, mTangentX);
				tangentYOffset = STRUCT_OFFSET(TangentType, mTangentY);
				tangentSizeInBytes = sizeof(TangentType);
			}
			data.mTangentBasisComponents[0] = VertexStreamComponent(&_TangentsVertexBuffer, tangentXOffset, tangentSizeInBytes, tangentElemType, EVertexStreamUsage::ManualFetch);
			data.mTangentBasisComponents[1] = VertexStreamComponent(&_TangentsVertexBuffer, tangentYOffset, tangentSizeInBytes, tangentElemType, EVertexStreamUsage::ManualFetch);

		}
	}

	void StaticMeshVertexBuffer::bindPackedTexCoordVertexBuffer(const VertexFactory* vertexFactory, StaticMeshDataType& data) const
	{
		data.mTextureCoordinates.empty();
		data.mNumTexCoords = getNumTexCoords();
		{
			data.mTextureCoordinateSRV = mTextureCoordinateSRV;
		}
		{
			EVertexElementType uvDoubleWideVertexElementType = VET_None;
			EVertexElementType uvVertexElementType = VET_None;
			uint32 uvSizeInBytes = 0;

			if (getUseFullPrecisionUVs())
			{
				uvSizeInBytes = sizeof(TStaticMeshVertexUVsTypeSelector<EStaticMeshVertexUVType::HighPrecision>::UVsTypeT);
				uvDoubleWideVertexElementType = VET_Float4;
				uvVertexElementType = VET_Float2;
			}
			else
			{
				uvSizeInBytes = sizeof(TStaticMeshVertexUVsTypeSelector<EStaticMeshVertexUVType::Default>::UVsTypeT);
				uvDoubleWideVertexElementType = VET_Half4;
				uvVertexElementType = VET_Half2;
			}
			uint32 uvStride = uvSizeInBytes * getNumTexCoords();
			int32 uvIndex;
			for (uvIndex = 0; uvIndex < (int32)getNumTexCoords()-1; uvIndex+=2)
			{
				data.mTextureCoordinates.add(VertexStreamComponent(
					&_TexcoordVertexBuffer,
					uvSizeInBytes * uvIndex,
					uvStride,
					uvDoubleWideVertexElementType,
					EVertexStreamUsage::ManualFetch
				));
				if (uvIndex < (int32)getNumTexCoords())
				{
					data.mTextureCoordinates.add(VertexStreamComponent(
						&_TexcoordVertexBuffer,
						uvSizeInBytes * uvIndex,
						uvStride,
						uvVertexElementType,
						EVertexStreamUsage::ManualFetch
					));
				}
			}
		}
	}

	class PositionVertexData : public TStaticMeshVertexData<PositionVertex>
	{
	public:
		PositionVertexData(bool inNeedsCPUAccess = false)
			:TStaticMeshVertexData<PositionVertex>(inNeedsCPUAccess)
		{

		}
	};

	void PositionVertexBuffer::allocateData(bool bNeedsCPUAccess /* = true */)
	{
		cleanUp();
		mVertexData = new PositionVertexData(bNeedsCPUAccess);
		mStride = mVertexData->getStride();
	}

	void PositionVertexBuffer::cleanUp()
	{
		if (mVertexData)
		{
			delete mVertexData;
			mVertexData = nullptr;
		}
	}

	void PositionVertexBuffer::initRHI()
	{
		BOOST_ASSERT(mVertexData);
		ResourceArrayInterface* resourceArray = mVertexData->getResourceArray();
		if (resourceArray->getResourceDataSize())
		{
			RHIResourceCreateInfo info(resourceArray);
			mVertexBufferRHI = RHICreateVertexBuffer(resourceArray->getResourceDataSize(), BUF_Static, info);
		}
	}

	void PositionVertexBuffer::serialize(Archive& ar, bool bNeedsCPUAccess)
	{
		ar << mStride << mNumVertices;
		if (ar.isLoading())
		{
			allocateData(bNeedsCPUAccess);
		}
		if (mVertexData != nullptr)
		{
			mVertexData->serialize(ar);
			mData = mVertexData->getDataPointer();
		}
	}

	void PositionVertexBuffer::init(const TArray<StaticMeshBuildVertex>& inVertices)
	{
		mNumVertices = inVertices.size();
		allocateData();
		mVertexData->resizeBuffer(mNumVertices);
		mData = mVertexData->getDataPointer();
		for (int32 vertexIndex = 0; vertexIndex < inVertices.size(); vertexIndex++)
		{
			const StaticMeshBuildVertex& sourceVertex = inVertices[vertexIndex];
			const uint32 destVertexIndex = vertexIndex;
			vertexPosition(destVertexIndex) = sourceVertex.mPosition;
		}
	}

	StaticMeshRenderData::StaticMeshRenderData()
	{
		for (int32 lodIndex = 0; lodIndex < MAX_STATIC_MESH_LODS; lodIndex++)
		{
			mScreenSize[lodIndex] = 0.0f;
		}
	}

	void StaticMeshRenderData::serialize(Archive& ar, RStaticMesh* owner, bool bCooked)
	{

	}


	void StaticMeshRenderData::resolveSectionInfo(RStaticMesh* owner)
	{
		int32 lodIndex = 0;
		int32 maxLods = mLODResources.size();
		BOOST_ASSERT(maxLods <= MAX_STATIC_MESH_LODS);
		for (; lodIndex < maxLods; lodIndex++)
		{
			StaticMeshLODResources& lod = mLODResources[lodIndex];
			for (int32 sectionIndex = 0 ; sectionIndex < lod.mSections.size(); ++sectionIndex)
			{
				MeshSectionInfo info = owner->mSectionInfoMap.get(lodIndex, sectionIndex);
				StaticMeshSection& section = lod.mSections[sectionIndex];
				section.mMaterialIndex = info.mMaterialIndex;
				section.bEnableCollision = info.bEnableCollision;
				section.bCastShadow = info.bCastShadow;
			}

			const float autoComputeLODPowerBase = 0.75f;
			if (owner->bAutoComputeLODScreenSize)
			{
				if (lodIndex == 0)
				{
					mScreenSize[lodIndex] = 1.0f;
				}
				else if (lod.mMaxDeviation <= 0.0f)
				{
					mScreenSize[lodIndex] = Math::pow(autoComputeLODPowerBase, lodIndex);
				}
				else
				{
					const float viewDistance = calculateViewDistance(lod.mMaxDeviation, owner->mSourceModels[lodIndex].mReductionSettings.mPixelError);

					const float holfFOV = PI * 0.25f;
					const float screenWidth = 1920.0f;
					const float screenHeight = 1080.0f;

					const PerspectiveMatrix projMatrix(holfFOV, screenWidth, screenHeight, 1.0f);
					mScreenSize[lodIndex] = computeBoundsScreenSize(float3::Zero, mBounds.mSphereRadius, float3(0.0f, 0.0f, viewDistance + mBounds.mSphereRadius), projMatrix);
				}
			}
			else if (owner->mSourceModels.isValidIndex(lodIndex))
			{
				mScreenSize[lodIndex] = owner->mSourceModels[lodIndex].mScreenSize;
			}
			else
			{
				BOOST_ASSERT(lodIndex > 0);
				const float tolerance = 0.01f;
				float autoDisplayFactor = Math::pow(autoComputeLODPowerBase, lodIndex);

				mScreenSize[lodIndex] = Math::clamp(autoDisplayFactor, 0.0f, mScreenSize[lodIndex - 1] - tolerance);
			}
		}
		for (; lodIndex < MAX_STATIC_MESH_LODS; ++lodIndex)
		{
			mScreenSize[lodIndex] = 0.0f;
		}
	}

	void StaticMeshRenderData::initResource(RStaticMesh* owner)
	{
#if WITH_EDITOR
		resolveSectionInfo(owner);
#endif
		for (int32 lodIndex = 0; lodIndex < mLODResources.size(); ++lodIndex)
		{
			mLODResources[lodIndex].initResources(owner);
		}
	}

	void StaticMeshRenderData::releaseResources()
	{
		for (int32 lodIndex = 0; lodIndex < mLODResources.size(); lodIndex++)
		{
			mLODResources[lodIndex].releaseResource();
		}
	}

	void StaticMeshRenderData::allocateLODResource(int32 numLODs)
	{
		BOOST_ASSERT(mLODResources.size() == 0);
		while (mLODResources.size() < numLODs)
		{
			new(mLODResources)StaticMeshLODResources();
		}


	}

	SIZE_T StaticMeshRenderData::getResourceSize() const
	{
		return getResourceSizeBytes();
	}

	SIZE_T StaticMeshRenderData::getResourceSizeBytes() const
	{
		return 0;
	}

	void StaticMeshRenderData::computeUVDensities()
	{
#if WITH_EDITORONLY_DATA
		for (StaticMeshLODResources& lodModel : mLODResources)
		{
			const int32 numTexCoords = Math::min<int32>(lodModel.getNumTexCoords(), MAX_STATIC_TEXCOORDS);
			for (StaticMeshSection& sectionInfo : lodModel.mSections)
			{
				Memory::memzero(sectionInfo.mUVDensities);
				Memory::memzero(sectionInfo.mWeights);
			}
		}
#endif
	}

#if WITH_EDITORONLY_DATA
	void StaticMeshRenderData::cache(RStaticMesh* owner, const StaticMeshLODSettings& LODSettings)
	{
		{
			int32 numLODs = owner->mSourceModels.size();
			const StaticMeshLODGroup& lodGroup = LODSettings.getLODGroup(owner->mLODGroup);

			TArray<uint8> derivedData;

			IMeshUtilities& meshUtilities = ModuleManager::get().loadModuleChecked<IMeshUtilities>(TEXT("MeshUtilities"));
			meshUtilities.buildStaticMesh(*this, owner->mSourceModels, lodGroup);
			//MemoryWriter ar(derivedData, true);
			//serialize(ar, owner, false);
		}
	}
#endif


	void StaticMeshLODSettings::initialize(const ConfigFile& iniFIle)
	{
		mGroups.findOrAdd(Name_None);

		const TCHAR* iniSection = TEXT("StaticMeshLODSettings");
		const auto& it = iniFIle.find(iniSection);
		if (it != iniFIle.end())
		{
			for (auto& keyIt : it->second)
			{
				wstring groupName = keyIt.first;
				StaticMeshLODGroup& group = mGroups.findOrAdd(groupName);
				readEntry(group, keyIt.second.getValue());
			}
		}

		for (auto& it : mGroups)
		{
			StaticMeshLODGroup& group = it.second;
			float percentTrianglesPerLOD = group.mDefualtSettings[1].mPercentTriangles;
			for (int32 lodIndex = 1; lodIndex < MAX_STATIC_MESH_LODS; ++lodIndex)
			{
				float percentTriangles = group.mDefualtSettings[lodIndex++].mPercentTriangles;
				group.mDefualtSettings[lodIndex] = group.mDefualtSettings[lodIndex - 1];
				group.mDefualtSettings[lodIndex].mPercentTriangles = percentTriangles * percentTrianglesPerLOD;
			}
		}
	}

	void StaticMeshLODSettings::readEntry(StaticMeshLODGroup& group, wstring entry)
	{

	}

	MeshReductionSettings StaticMeshLODGroup::getSettings(const MeshReductionSettings& inSettings, int32 LODIndex) const
	{
		BOOST_ASSERT(LODIndex >= 0 && LODIndex < MAX_STATIC_MESH_LODS);
		MeshReductionSettings finalSettings = inSettings;
		float percentTrianglesMult = (LODIndex == 0) ? mBasePercentTrianglesMult : mSettingsBias.mPercentTriangles;
		finalSettings.mPercentTriangles = Math::clamp(inSettings.mPercentTriangles * percentTrianglesMult, 0.0f, 1.0f);
		finalSettings.mMaxDeviation = Math::max(inSettings.mMaxDeviation + mSettingsBias.mMaxDeviation, 0.0f);
		finalSettings.mPixelError = Math::max(inSettings.mPixelError + mSettingsBias.mPixelError, 1.0f);
		return finalSettings;
	}

	template <bool bRenderThread>
	VertexBufferRHIRef StaticMeshVertexBuffer::createTangentsRHIBuffer_Interal()
	{
		if (getNumVertices())
		{
			ResourceArrayInterface* RESTRICT resourceArray = mTangentsData ? mTangentsData->getResourceArray() : nullptr;
			const uint32 sizeInBytes = resourceArray ? resourceArray->getResourceDataSize() : 0;
			RHIResourceCreateInfo createInfo(resourceArray);
			createInfo.bWithoutNativeResource = !mTangentsData;
			if (bRenderThread)
			{
				return RHICreateVertexBuffer(sizeInBytes, BUF_Static | BUF_ShaderResource, createInfo);
			}
			else
			{
				return RHIAsyncCreateVertexBuffer(sizeInBytes, BUF_Static | BUF_ShaderResource, createInfo);
			}
		}
	}

	template<bool bRenderThread>
	VertexBufferRHIRef StaticMeshVertexBuffer::createTexcoordRHIBuffer_Internal()
	{
		if (getNumTexCoords())
		{
			ResourceArrayInterface* RESTRICT resourceArray = mTexcoordData ? mTexcoordData->getResourceArray() : nullptr;
			const uint32 sizeInBytes = resourceArray ? resourceArray->getResourceDataSize() : 0;
			RHIResourceCreateInfo createinfo(resourceArray);
			createinfo.bWithoutNativeResource = !mTexcoordData;
			if (bRenderThread)
			{
				return RHICreateVertexBuffer(sizeInBytes, sizeInBytes, createinfo);
			}
			else
			{ 
				return RHIAsyncCreateVertexBuffer(sizeInBytes, sizeInBytes, createinfo);
			}
		}
	}

	VertexBufferRHIRef StaticMeshVertexBuffer::createTangentsRHIBuffer_RenderThread()
	{
		return createTangentsRHIBuffer_Interal<true>();
	}
	VertexBufferRHIRef StaticMeshVertexBuffer::createTangentsRHIBuffer_Async()
	{
		return createTangentsRHIBuffer_Interal<false>();
	}

	VertexBufferRHIRef StaticMeshVertexBuffer::createTexcoordRHIBuffer_RenderThread()
	{
		return createTexcoordRHIBuffer_Internal<true>();
	}
	VertexBufferRHIRef StaticMeshVertexBuffer::createTexcoordRHIBuffer_Async()
	{
		return createTexcoordRHIBuffer_Internal<false>();
	}

	void StaticMeshVertexBuffer::serializeMetaData(Archive& ar)
	{
		ar << mNumTexcoords << mNumVertices;
		ar << bUseFullPrecisionUVs;
		ar << bUseHighPrecisionTangentBasis;
		initTangentAndTexcoordStrides();
	}

	void StaticMeshVertexBuffer::initTangentAndTexcoordStrides()
	{
		typedef TStaticMeshVertexTangentDatum<typename TStaticMeshVertexTangentTypeSelector<EStaticMeshVertexTangentBasisType::HighPrecision>::TangentTypeT> HighPrecTangentType;
		typedef TStaticMeshVertexTangentDatum<typename TStaticMeshVertexTangentTypeSelector<EStaticMeshVertexTangentBasisType::Default>::TangentTypeT> DefaultTangentType;
		typedef TStaticMeshVertexUVsDatum<typename TStaticMeshVertexUVsTypeSelector<EStaticMeshVertexUVType::HighPrecision>::UVsTypeT> HighPrecUVType;
		typedef TStaticMeshVertexUVsDatum<typename TStaticMeshVertexUVsTypeSelector<EStaticMeshVertexUVType::Default>::UVsTypeT> DefaultUVType;
		mTangentsStride = getUseHighPrecisionTangentBasis() ? sizeof(HighPrecTangentType) : sizeof(DefaultTangentType);
		mTexcoordStride = getUseFullPrecisionUVs() ? sizeof(HighPrecUVType) : sizeof(DefaultUVType);
	}

	void StaticMeshVertexBuffer::bindLightMapVertexBuffer(const VertexFactory* vertexFactory, struct StaticMeshDataType& data, int lightMapCoordinateIndex) const
	{

	}
}