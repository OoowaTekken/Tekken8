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
	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_Timer;

	/*
	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* VictoryPoint1;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* VictoryPoint2;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* VictoryPoint3;
*/

	
	
};
