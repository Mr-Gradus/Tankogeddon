#include "StyleSet.h"
#include "Engine.h"
#include "SlateBasics.h"
#include "Slate/SlateGameResources.h"
#include "Styling/ISlateStyle.h"

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

TSharedPtr<ISlateStyle> FindStyle(const FString& ScopeToDirectory)
{
	TSharedPtr<FSlateStyleSet> Style = FSlateGameResources::New(FName("StyleSet"), ScopeToDirectory, ScopeToDirectory);

	Style->Set("Red", FLinearColor(1.f, 0.f, 0.f, 1.f));
	Style->Set("DefaultPadding", FMargin(50.f, 20.f));
	Style->Set("BlueButton",
	new IMAGE_BRUSH("Blue_button", FVector2D(256.0f, 256.0f)));
	Style->Set("GreenButton",
		new IMAGE_BRUSH("Green_button", FVector2D(256.0f, 256.0f)));

	return Style;
}

TSharedPtr<ISlateStyle> FStyleSet::StylePtr = nullptr;

void FStyleSet::Initialize()
{
	Shutdown();

	StylePtr = FindStyle("/Game/UI");
	FSlateStyleRegistry::RegisterSlateStyle(*StylePtr);
}

void FStyleSet::Shutdown()
{
	if (StylePtr.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StylePtr);

		ensure(StylePtr.IsUnique());
		StylePtr.Reset();
	}
}

const ISlateStyle& FStyleSet::Get()
{
	if (!StylePtr.IsValid())
	{
		Initialize();
	}
	return *StylePtr;
}

/*
TSharedPtr<ISlateStyle> FindStyle()
{
	FString folder = FPaths::GameSourceDir() / TEXT("/Game/Content/UI");

	TSharedPtr<FSlateStyleSet> Style = FSlateGameResources::New(FName("StyleSet"), folder, folder);

	Style->Set("Red", FLinearColor(1.f, 0.f, 0.f, 1.f));
	Style->Set("DefaultPadding", FMargin(50.f, 20.f));
	Style->Set("BlueButton",
	new IMAGE_BRUSH("Blue_button", FVector2D(256.0f, 256.0f)));
	Style->Set("GreenButton",
		new IMAGE_BRUSH("Green_button", FVector2D(256.0f, 256.0f)));

	return Style;
}
*/
#undef IMAGE_BRUSH
#undef BOX_BRUSH