#include "Classes/Materials/MaterialInstanceDynamic.h"
#include "SimpleReflection.h"
namespace Air
{
	RMaterialInstanceDynamic::RMaterialInstanceDynamic(const ObjectInitializer& objectInitializer/* = ObjectInitializer::get() */)
		:ParentType(objectInitializer)
	{}

	std::shared_ptr<RMaterialInstanceDynamic> RMaterialInstanceDynamic::create(std::shared_ptr<class MaterialInterface>& parentMaterial, class Object* inOuter, wstring name)
	{
		Object* outer = inOuter ? inOuter : nullptr;
		std::shared_ptr<RMaterialInstanceDynamic> mid = newObject<RMaterialInstanceDynamic>(outer, name);
		mid->setParentInternal(parentMaterial, false);
		return mid;
	}


	std::shared_ptr<RMaterialInstanceDynamic> RMaterialInstanceDynamic::create(std::shared_ptr<class MaterialInterface>& parentMaterial, class Object* inOuter)
	{
		Object* outer = inOuter ? inOuter : nullptr;
		std::shared_ptr<RMaterialInstanceDynamic> mid = newObject<RMaterialInstanceDynamic>(outer);
		mid->setParentInternal(parentMaterial, false);
		return mid;
	}

	void RMaterialInstanceDynamic::setScalarParameterValue(wstring parameterName, float value)
	{
		setScalarParameterValueInternal(parameterName, value);
	}

	void RMaterialInstanceDynamic::setScalarParameterByInde(int32 parameterIndex, float value)
	{
		setScalarParameterByIndexInternal(parameterIndex, value);
	}

	void RMaterialInstanceDynamic::setVectorParameterValue(wstring parameterName, LinearColor& value)
	{
		setVectorParameterValueInternal(parameterName, value);
	}

	void RMaterialInstanceDynamic::setVectorParameterByIndex(int32 parameterIndex, LinearColor& value)
	{
		setVectorParameterByIndexInternal(parameterIndex, value);
	}

	DECLARE_SIMPLER_REFLECTION(RMaterialInstanceDynamic);
}