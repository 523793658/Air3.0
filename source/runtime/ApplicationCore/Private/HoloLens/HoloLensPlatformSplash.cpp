// Copyright Epic Games, Inc. All Rights Reserved.
#if PLATFORM_HOLOLENS

#include "HoloLens/HoloLensPlatformSplash.h"
#include "HoloLens/HoloLensPlatformApplicationMisc.h"

void FHoloLensSplash::Show()
{
	//@todo.HoloLens: Implement me
	FHoloLensPlatformApplicationMisc::PumpMessages(true);
}

void FHoloLensSplash::Hide()
{
	//@todo.HoloLens: Implement me
	FHoloLensPlatformApplicationMisc::PumpMessages(true);
}

void FHoloLensSplash::SetSplashText( const SplashTextType::Type InType, const TCHAR* InText )
{
	//@todo.HoloLens: Implement me
}
#endif