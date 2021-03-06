// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealSourceFile.h"
#include "UnrealHeaderTool.h"
#include "Misc/PackageName.h"
#include "HeaderParser.h"

void FUnrealSourceFile::AddDefinedClass(UClass* Class, FSimplifiedParsingClassInfo&& ParsingInfo)
{
	DefinedClasses.Add(Class, MoveTemp(ParsingInfo));
}

const FString& FUnrealSourceFile::GetFileId() const
{
	if (FileId.Len() == 0)
	{
		FString StdFilename = Filename;

		FPaths::MakeStandardFilename(StdFilename);

		bool bRelativePath = FPaths::IsRelative(StdFilename);

		if (!bRelativePath)
		{
			// If path is still absolute that means MakeStandardFilename has failed
			// In this case make it relative to the current project. 
			bRelativePath = FPaths::MakePathRelativeTo(StdFilename, *FPaths::GetPath(FPaths::GetProjectFilePath()));
		}

		// If the path has passed either MakeStandardFilename or MakePathRelativeTo it should be using internal path seperators
		if (bRelativePath)
		{
			// Remove any preceding parent directory paths
			while (StdFilename.RemoveFromStart(TEXT("../")));
		}

		FStringOutputDevice Out;

		for (TCHAR Char : StdFilename)
		{
			if (FChar::IsAlnum(Char))
			{
				Out.AppendChar(Char);
			}
			else
			{
				Out.AppendChar('_');
			}
		}

		FileId = MoveTemp(Out);
	}

	return FileId;
}

const FString& FUnrealSourceFile::GetStrippedFilename() const
{
	if (StrippedFilename.Len() == 0)
	{
		StrippedFilename = FPaths::GetBaseFilename(Filename);
	}

	return StrippedFilename;
}

FString FUnrealSourceFile::GetGeneratedMacroName(FClassMetaData* ClassData, const TCHAR* Suffix) const
{
	return GetGeneratedMacroName(ClassData->GetGeneratedBodyLine(), Suffix);
}

FString FUnrealSourceFile::GetGeneratedMacroName(int32 LineNumber, const TCHAR* Suffix) const
{
	if (Suffix != nullptr)
	{
		return FString::Printf(TEXT("%s_%d%s"), *GetFileId(), LineNumber, Suffix);
	}

	return FString::Printf(TEXT("%s_%d"), *GetFileId(), LineNumber);
}

FString FUnrealSourceFile::GetGeneratedBodyMacroName(int32 LineNumber, bool bLegacy) const
{
	return GetGeneratedMacroName(LineNumber, *FString::Printf(TEXT("%s%s"), TEXT("_GENERATED_BODY"), bLegacy ? TEXT("_LEGACY") : TEXT("")));
}

void FUnrealSourceFile::SetGeneratedFilename(FString&& InGeneratedFilename)
{
	GeneratedFilename = MoveTemp(InGeneratedFilename);
}

void FUnrealSourceFile::SetHasChanged(bool bInHasChanged)
{
	bHasChanged = bInHasChanged;
}

void FUnrealSourceFile::SetModuleRelativePath(FString&& InModuleRelativePath)
{
	ModuleRelativePath = MoveTemp(InModuleRelativePath);
}

void FUnrealSourceFile::SetIncludePath(FString&& InIncludePath)
{
	IncludePath = MoveTemp(InIncludePath);
}

const FString& FUnrealSourceFile::GetContent() const
{
	return Content;
}

EGeneratedCodeVersion FUnrealSourceFile::GetGeneratedCodeVersionForStruct(UStruct* Struct) const
{
	if (const EGeneratedCodeVersion* Version = GeneratedCodeVersions.Find(Struct))
	{
		return *Version;
	}

	return FHeaderParser::DefaultGeneratedCodeVersion;
}

void FUnrealSourceFile::MarkDependenciesResolved()
{
	bDependenciesResolved = true;
}

bool FUnrealSourceFile::AreDependenciesResolved() const
{
	return bDependenciesResolved;
}

void FUnrealSourceFile::SetScope(FFileScope* InScope)
{
	if (&Scope.Get() != InScope)
	{
		Scope = TSharedRef<FFileScope>(InScope);
	}
}

void FUnrealSourceFile::SetScope(TSharedRef<FFileScope> InScope)
{
	Scope = InScope;
}

void FUnrealSourceFile::MarkAsParsed()
{
	bParsed = true;
}

bool FUnrealSourceFile::IsParsed() const
{
	return bParsed;
}

bool FUnrealSourceFile::HasChanged() const
{
	return bHasChanged;
}

FString FUnrealSourceFile::GetFileDefineName() const
{
	const FString API = FPackageName::GetShortName(Package).ToUpper();
	return FString::Printf(TEXT("%s_%s_generated_h"), *API, *GetStrippedFilename());
}
