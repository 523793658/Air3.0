#pragma once
#include "core/Application.h"
#include "ApplicationManager.h"
#include "Classes/Engine/World.h"
namespace Air
{
	class FreeCamera
	{
	private:
		Air::ViewportCameraTransform& mCamera;

		bool bIsMoving{ false };
		bool bIsRotating{ false };
		Air::int2 mLastMousePosition;
		Air::float3 mSpeed{ 0,0,0 };
	public:

		FreeCamera(Air::ViewportCameraTransform& camera)
			:mCamera(camera)
		{
		}

		void onKeyPress(uint32 key);

		void onKeyRelease(uint32 key);

		void onMousePress(uint32 key, int32 x, int32 y);

		void onMouseRelease(uint32 key, int32 x, int32 y);

		void onMouseMove(uint32 key, int32 x, int32 y);

		void update(float deltaTime, bool bIdleMode);
	};


	class DemoInitEngine : public Application
	{
		GENERATED_RCLASS_BODY(DemoInitEngine, Application);

	public:

		virtual void init(DemoEngine* inEngine) override;
		

		virtual void update(float deltaTime, bool bIdleMode);

		virtual void release()
		{

		}

		virtual void start() override;

		virtual string getTitle()
		{
			return "Init Engine";
		}

		void onKeyPress(uint32 key);

		void onKeyRelease(uint32 key);

		void onMousePress(uint32 key, int32 x, int32 y);

		void onMouseRelease(uint32 key, int32 x, int32 y);

		void onMouseMove(uint32 key, int32 x, int32 y);

	private:
		void registerEvent(DemoEngine& engine);
		
		KeyEventFunction OnKeyPress;
		KeyEventFunction OnkeyRelease;
		MouseEventFunction OnMousePress;
		MouseEventFunction OnMouseReleas;
		MouseEventFunction OnMouseMove;

		FreeCamera* mFreeCamera;
	};
}