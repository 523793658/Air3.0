// Copyright Epic Games, Inc. All Rights Reserved.
#if PLATFORM_ANDROID
#include "Android/AndroidCursor.h"
#include "Android/AndroidApplication.h"

FAndroidCursor::FAndroidCursor() 
	: CurrentType(EMouseCursor::None)
	, CurrentPosition(FVector2D::ZeroVector)
	, CursorClipRect()
	, bShow(false)
	, UIScaleFactor(1.0f)
{
	ComputeUIScaleFactor();
}

void FAndroidCursor::ComputeUIScaleFactor()
{
	FPlatformRect ScreenRect = FAndroidWindow::GetScreenRect();
	int32_t ScreenWidth, ScreenHeight;
	FAndroidWindow::CalculateSurfaceSize(ScreenWidth, ScreenHeight);

	if (ScreenWidth > ScreenHeight)
	{
		UIScaleFactor = (ScreenRect.Bottom - ScreenRect.Top) / (float)ScreenHeight;
	}
	else
	{
		UIScaleFactor = (ScreenRect.Right - ScreenRect.Left) / (float)ScreenWidth;
	}
}

void FAndroidCursor::SetPosition( const int32 X, const int32 Y )
{
	FVector2D NewPosition(X, Y);

//	NewPosition *= UIScaleFactor;

	UpdateCursorClipping(NewPosition);
	CurrentPosition = NewPosition;
}

void FAndroidCursor::SetType( const EMouseCursor::Type InNewCursor )
{
	CurrentType = InNewCursor;
}

void FAndroidCursor::GetSize(int32& Width, int32& Height) const
{
	Width = 32;
	Height = 32;
}

void FAndroidCursor::Show(bool bInShow)
{
	bShow = bInShow;
}

void FAndroidCursor::Lock(const RECT* const Bounds)
{
	if (Bounds == NULL)
	{
		FDisplayMetrics DisplayMetrics;
		FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);

		// The Android cursor should never leave the screen
		CursorClipRect.Min = FIntPoint::ZeroValue;
		CursorClipRect.Max.X = DisplayMetrics.PrimaryDisplayWidth - 1;
		CursorClipRect.Max.Y = DisplayMetrics.PrimaryDisplayHeight - 1;
	}
	else
	{
		CursorClipRect.Min.X = FMath::TruncToInt(Bounds->left);
		CursorClipRect.Min.Y = FMath::TruncToInt(Bounds->top);
		CursorClipRect.Max.X = FMath::TruncToInt(Bounds->right) - 1;
		CursorClipRect.Max.Y = FMath::TruncToInt(Bounds->bottom) - 1;
	}

	FVector2D Position = GetPosition();
	if (UpdateCursorClipping(Position))
	{
//		SetPosition(Position.X, Position.Y);
	}
}

bool FAndroidCursor::UpdateCursorClipping(FVector2D& CursorPosition)
{
	bool bAdjusted = false;

	if (CursorClipRect.Area() > 0)
	{
		if (CursorPosition.X < CursorClipRect.Min.X)
		{
			CursorPosition.X = CursorClipRect.Min.X;
			bAdjusted = true;
		}
		else if (CursorPosition.X > CursorClipRect.Max.X)
		{
			CursorPosition.X = CursorClipRect.Max.X;
			bAdjusted = true;
		}

		if (CursorPosition.Y < CursorClipRect.Min.Y)
		{
			CursorPosition.Y = CursorClipRect.Min.Y;
			bAdjusted = true;
		}
		else if (CursorPosition.Y > CursorClipRect.Max.Y)
		{
			CursorPosition.Y = CursorClipRect.Max.Y;
			bAdjusted = true;
		}
	}

	return bAdjusted;
}
#endif