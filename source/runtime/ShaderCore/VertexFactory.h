#pragma once
#include "CoreMinimal.h"
#include "Containers/LinkList.h"
#include "Containers/Map.h"
#include "Containers/Array.h"
#include "ShaderCore.h"
#include "ShaderCoreConfig.h"
#include "Containers/StringConv.h"
#include "Shader.h"

namespace Air
{
	struct VertexStreamComponent
	{
		const VertexBuffer* mVertexBuffer;
		uint8 mOffset;
		uint8 mStride;
		TEnumAsByte<EVertexElementType> mType;
		bool bUseInstanceIndex;

		bool bSetByVertexFactoryInSetMesh;

		VertexStreamComponent()
			:mVertexBuffer(nullptr)
			,mOffset(0)
			,mStride(0)
			,mType(VET_None)
			,bUseInstanceIndex(false)
			,bSetByVertexFactoryInSetMesh(false)
		{

		}

		VertexStreamComponent(const VertexBuffer* inVertexBuffer, uint32 inOffset, uint32 inStride, EVertexElementType inType, bool bInUseInstanceIndex = false, bool bInSetByVertexFactoryInSetMesh = false)
			:mVertexBuffer(inVertexBuffer)
			,mOffset(inOffset)
			,mStride(inStride)
			,mType(inType)
			,bUseInstanceIndex(bInUseInstanceIndex)
			,bSetByVertexFactoryInSetMesh(bInSetByVertexFactoryInSetMesh)
		{}
	};

	class SHADER_CORE_API VertexFactoryShaderParameters
	{
	public:
		virtual ~VertexFactoryShaderParameters() {}
		virtual void bind(const class ShaderParameterMap& parameterMap) = 0;
		virtual void serialize(Archive& ar) = 0;
		virtual void setMesh(RHICommandList& RHICmdList, Shader* vertexShader, const class VertexFactory* vertexFactory, const class SceneView& view, const struct MeshBatchElement& batchElement, uint32 dataFlags) const = 0;
		virtual uint32 getSize() const { return sizeof(*this); }
	};


	class VertexFactoryType
	{
	public:
		

		typedef bool(*supportsTessellationShaderType)();
		typedef VertexFactoryShaderParameters* (*ConstructParametersType)(EShaderFrequency shaderFrequency);
		typedef bool(*ShouldCacheType)(EShaderPlatform, const class FMaterial*, const class ShaderType*);

		typedef void(*ModifyCompilationEnvironmentType)(EShaderPlatform, const class FMaterial*, ShaderCompilerEnvironment&);

		VertexFactoryShaderParameters* createShaderParameters(EShaderFrequency shaderFrequency) const { return (*mConstructParameters)(shaderFrequency); }

		bool isUsedWithMaterial()const { return bUseWithMaterials; }

		SHADER_CORE_API VertexFactoryType(
			const TCHAR* inName,
			const TCHAR* inShaderFilename,
			bool bInUseWithMaterials,
			bool bInSupportsStaticLighting,
			bool bInSupportsDynamicLightings,
			bool bInSupportsPrecisePrevWorldPos,
			bool bInSupportsPositionOnly,
			ConstructParametersType inConstructParameters,
			ShouldCacheType inShouldCache,
			ModifyCompilationEnvironmentType inModifyCompilationEnvironment,
			supportsTessellationShaderType inSupportsTessellationShaders
		);


		SHADER_CORE_API ~VertexFactoryType();

		static SHADER_CORE_API TLinkedList<VertexFactoryType*>*& getTypeList();

		static void initialize(const TMap<wstring, TArray<const TCHAR*>>& shaderFileToConstantBufferVariables);

		const TCHAR* getShaderFilename() const
		{
			return mShaderFilename;
		}

		bool shouldCache(EShaderPlatform platform, const class FMaterial* material, const class ShaderType* shaderType)
		{
			return (*mShouldCacheRef)(platform, material, shaderType);
		}
		inline int32 getId() const { return mHashIndex; }

		void flushShaderFileCache(const TMap<wstring, TArray<const TCHAR*>>& shaderfileToConstantBufferVariables)
		{
			mReferencedConstantBufferStructsCache.clear();
			generateReferencedConstantBuffers(mShaderFilename, mName, shaderfileToConstantBufferVariables, mReferencedConstantBufferStructsCache);
			for (int32 platform = 0; platform < SP_NumPlatforms; platform++)
			{
				bCachedConstantBufferStructDeclarations[platform] = false;
			}
		}

		SHADER_CORE_API const SHAHash& getSourceHash() const;

		const SerializationHistory* getSerializationHistory(EShaderFrequency frequency) const
		{
			return &mSerializationHistory[frequency];
		}

		const TCHAR* getName() const { return mName; }

		static VertexFactoryType* getVFByName(const wstring& VFName);

		SHADER_CORE_API void addReferencedConstantBufferIncludes(ShaderCompilerEnvironment& outEnvironment, wstring& outSourceFilePrefix, EShaderPlatform platform);

		bool supportsTessellationShaders() const
		{
			return (*mSupportsTessellationShadersRef)();
		}

		const TMap<const TCHAR*, CachedConstantBufferDeclaration>& getReferencedConstantBufferStructsCache() const
		{
			return mReferencedConstantBufferStructsCache;
		}

		void modifyCompilationEnvironment(EShaderPlatform platform, const FMaterial* material, ShaderCompilerEnvironment& outEnvironment)
		{
			wstring vertexFactoryIncludeString = printf(TEXT("#include \"%s.hlsl\""), getShaderFilename());
			outEnvironment.mIncludeFileNameToContentsMap.emplace(TEXT("VertexFactory.hlsl"), stringToArray<ANSICHAR>(vertexFactoryIncludeString.c_str(), vertexFactoryIncludeString.length() + 1));
			outEnvironment.setDefine(TEXT("HAS_PRIMITIVE_CONSTANT_BUFFER"), 1);
			(*mModifyCompilationEnvironmentRef)(platform, material, outEnvironment);
		}

		static int32 getNumVertexFactoryTypes() { return mNextHashIndex; }

	private:
		static bool bInitializedSerializationHistory;

		static SHADER_CORE_API uint32 mNextHashIndex;

		TMap<const TCHAR*, CachedConstantBufferDeclaration> mReferencedConstantBufferStructsCache;

		SerializationHistory mSerializationHistory[SF_NumFrequencies];
		bool bCachedConstantBufferStructDeclarations[SP_NumPlatforms];
		const TCHAR* mName;
		wstring mTypeName;
		const TCHAR* mShaderFilename;
		uint32 mHashIndex;
		uint32 bSupportsStaticLighting : 1;
		uint32 bSupportsDynamicLighting : 1;
		uint32 bSupportsPrecisePrevWorldPos : 1;
		uint32 bSupportsPositionOnly : 1;
		uint32 bUseWithMaterials : 1;
		ConstructParametersType mConstructParameters;
		supportsTessellationShaderType mSupportsTessellationShadersRef;
		ShouldCacheType mShouldCacheRef;
		ModifyCompilationEnvironmentType mModifyCompilationEnvironmentRef;

		TLinkedList<VertexFactoryType*> mGlobalListLink;
	};

	extern SHADER_CORE_API Archive& operator << (Archive& ar, VertexFactoryType*& typeRef);

	class VertexFactoryTypeDependency
	{
	public:
		VertexFactoryTypeDependency()
			:mVertexFactoryType(nullptr)
		{}

		VertexFactoryType* mVertexFactoryType;
		SHAHash mVFSourceHash;

		bool operator ==(const VertexFactoryTypeDependency& reference) const
		{
			return mVertexFactoryType == reference.mVertexFactoryType && mVFSourceHash == reference.mVFSourceHash;
		}

		friend Archive& operator << (Archive& ar, class VertexFactoryTypeDependency& ref)
		{
			ar << ref.mVertexFactoryType << ref.mVFSourceHash;
			return ar;
		}
	};


	class SHADER_CORE_API VertexFactory : public RenderResource
	{
	public:
		virtual VertexFactoryType* getType() const { return nullptr; }
		const VertexDeclarationRHIRef& getDeclaration() const { return mDeclaration; }
		struct VertexStream
		{
			const VertexBuffer* mVertexBuffer;
			uint32 mStride;
			uint32 mOffset;
			bool bUseInstanceIndex;
			bool bSetByVertexFactoryInSetMesh;
			friend bool operator == (const VertexStream& a, const VertexStream&  b)
			{
				return a.mVertexBuffer == b.mVertexBuffer && a.mStride == b.mStride && a.mOffset == b.mOffset;
			}

			VertexStream()
				:mVertexBuffer(nullptr)
				,mStride(0)
				,mOffset(0)
				,bUseInstanceIndex(false)
				,bSetByVertexFactoryInSetMesh(false)
			{}
		};
		void set(RHICommandList& RHICmdList) const;

		VertexElement accessPositionStreamComponent(const VertexStreamComponent& component, uint8 attributeIndex);

		VertexElement accessStreamComponent(const VertexStreamComponent& component, uint8 attributeIndex);

		void VertexFactory::initPositionDeclaration(const VertexDeclarationElementList& elements);

		void initDeclaration(VertexDeclarationElementList& elements);

		virtual uint64 getStaticBatchElementVisibility(const class SceneView& view, const struct MeshBatch* batch) const { return 1; }

		TArray<VertexStream, TFixedAllocator<MaxVertexElementCount>> mStreams;
	private:
		TArray<VertexStream, TFixedAllocator<MaxVertexElementCount>> mPositionStream;
		VertexDeclarationRHIRef	mDeclaration;
		VertexDeclarationRHIRef mPositionDeclaration;
	};

	class SHADER_CORE_API VertexFactoryParameterRef
	{
	public:
		VertexFactoryParameterRef(VertexFactoryType* inVertexFactoryType, const ShaderParameterMap& parameterMap, EShaderFrequency inShaderFrequency);

		VertexFactoryParameterRef() 
			:mParameters(nullptr),
			mVertexFactoryType(nullptr)
		{}

		~VertexFactoryParameterRef() { delete mParameters; }

		void setMesh(RHICommandList& RHICmdList, Shader* shader, const VertexFactory* vertexFactory, const class SceneView& view, const struct MeshBatchElement& batchElement, uint32 dataFlags) const
		{
			if (mParameters)
			{
				mParameters->setMesh(RHICmdList, shader, vertexFactory, view, batchElement, dataFlags);
			}
		}

		const VertexFactoryType* getVertexFactoryType() const { return mVertexFactoryType; }

		const SHAHash& getHash() const;

		friend SHADER_CORE_API bool operator <<(Archive& ar, VertexFactoryParameterRef& ref);

		uint32 getAllocatedSize() const
		{
			return mParameters ? mParameters->getSize() : 0;
		}
	private:
		VertexFactoryShaderParameters* mParameters;
		VertexFactoryType* mVertexFactoryType;
		EShaderFrequency mShaderFrequency;
		SHAHash mVFHash;
	};

#define DECALRE_VERTEX_FACTORY_TYPE(FactoryClass) \
	public:\
	static VertexFactoryType mStaticType;	\
	virtual VertexFactoryType* getType() const override{return &mStaticType;}


	class CompareVertexFactoryTypes
	{
	public:
		FORCEINLINE bool operator()(const VertexFactoryType& a, const VertexFactoryType& b)const
		{
			int32 al = CString::strlen(a.getName());
			int32 bl = CString::strlen(b.getName());
			if (al == bl)
			{
				return CString::strncmp(a.getName(), b.getName(), al) > 0;
			}
			return al > bl;
		}
	};


#define IMPLEMENT_VERTEX_FACTORY_TYPE(FactoryClass, shaderFilename, bUsedWithMaterials, bSupportsStaticLighting, bSupportsDynamicLighting, bPrecisePrevWorldPos, bSupportsPositionOnly)	\
	VertexFactoryShaderParameters* Construct##FactoryClass##ShaderParameters(EShaderFrequency shaderFrequency){return FactoryClass::constructShaderParameters(shaderFrequency);} \
		VertexFactoryType FactoryClass::mStaticType( \
		TEXT(#FactoryClass),	\
		TEXT(shaderFilename),	\
		bUsedWithMaterials,	\
		bSupportsStaticLighting,	\
		bSupportsDynamicLighting,	\
		bPrecisePrevWorldPos,	\
		bSupportsPositionOnly,	\
		Construct##FactoryClass##ShaderParameters,	\
		FactoryClass::shouldCache,	\
		FactoryClass::modifyCompilationEnvironment,	\
		FactoryClass::supportsTessellationShaders\
	);

#define STRUCTMEMBER_VERTEXSTREAMCOMPONENT(vertexBuffer, vertexType, Member, memberType) \
	VertexStreamComponent(vertexBuffer, STRUCT_OFFSET(vertexType, Member), sizeof(vertexType), memberType)
}