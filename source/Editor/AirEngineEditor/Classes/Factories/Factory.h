#pragma once
#include "UObject/Object.h"
#include "EditorConfig.h"
#include "Modules/ModuleInterface.h"
namespace Air
{
	class EDITOR_ENGINE_API Factory
	{
	public:
		virtual std::shared_ptr<Object> createFromFileInner(Object* inObject, wstring filename, wstring name, EObjectFlags flags) = 0;

		virtual void getExtensions(TCHAR**& extensions, uint32& num) = 0;

		static void initAllFactory();

		static std::shared_ptr<Object> createFromFile(Object* inObject, wstring filename, wstring name, EObjectFlags flags);


	private:
		static TMap<wstring, Factory*> mExtensionMaps;

	protected:

	};

	class IFactoryModule : public IModuleInterface
	{
	public:
		virtual Factory* createFactory() = 0;
	};
}