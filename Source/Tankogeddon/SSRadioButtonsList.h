#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"



class TANKOGEDDON_API SSRadioButtonsList : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSRadioButtonsList)
	{}
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);
};
