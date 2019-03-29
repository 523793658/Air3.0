#include "Classes/Components/StaticMeshComponent.h"
#include "StaticMeshResources.h"
#include "SimpleReflection.h"
#include "Classes/Engine/World.h"
namespace Air
{
	StaticMeshComponent::StaticMeshComponent(const ObjectInitializer& objectInitializer)
		:MeshComponent(objectInitializer)
	{

	}

	PrimitiveSceneProxy* StaticMeshComponent::createSceneProxy()
	{
		if (getStaticMesh() == nullptr || getStaticMesh()->mRenderData == nullptr || getStaticMesh()->mRenderData->mLODResources.size() == 0 || getStaticMesh()->mRenderData->mLODResources[0].mVertexBuffer.getNumVertices() == 0)
		{
			return nullptr;
		}

		PrimitiveSceneProxy* proxy = new StaticMeshSceneProxy(this, getStaticMesh()->bLODsShareStaticLighting);

		return proxy;
	}

	bool StaticMeshComponent::setStaticMesh(class RStaticMesh* newMesh)
	{
		if (newMesh == getStaticMesh())
		{
			return false;
		}


		AActor *owner = getOwner();
		if (World* world = getWorld())
		{
			if (world->hasBegunPlay() && !areDynamicDataChangeAllowed() && owner != nullptr)
			{
				return false;
			}
		}

		mStaticMesh = newMesh;

		markRenderStateDirty();
		markCachedMaterialParameterNameIndicesDirty();
		return true;
	}

	DECLARE_SIMPLER_REFLECTION(StaticMeshComponent);
}