#pragma once
#include "Classes/GameFramework/Actor.h"
namespace Air
{
	class ENGINE_API AStaticMeshActor : public AActor
	{
		GENERATED_RCLASS_BODY(AStaticMeshActor, AActor)
	public:
		

		class StaticMeshComponent* getStaticMeshComponent()const;

	public:

		class StaticMeshComponent* mStaticMeshComponennt;
	};
}