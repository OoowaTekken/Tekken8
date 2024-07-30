// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "inGameUI.generated.h"

/**
 * 
 */
UCLASS()
class JMH_API UinGameUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_Timer;

	UPROPERTY()
	class AGameMode_MH* gm;

	UFUNCTION()
	void UpdateTimerDisplay(float TimerValue);
	
	/*
	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* VictoryPoint1;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* VictoryPoint2;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* VictoryPoint3;
	
	if(gameMode)
	{
		FString ValueAsString = FString::Printf(TEXT("%.0f") , gameMode->gameTimer);
		text_Timer->SetText(FText::FromString(ValueAsString));
	}
*/

	
	
};
