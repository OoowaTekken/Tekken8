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
	UPROPERTY()
	class AGameMode_MH* gm;
	
	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UPlayerInfoUI* WBP_PlayerInfo;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_Timer;


	UFUNCTION()
	void UpdateTimerDisplay(float TimerValue);


	//라운드 UI num 변경
	UFUNCTION()
	void ShowRoundText(int32 RoundNum);

	UFUNCTION()
	void HideRoundText();
	
	UFUNCTION()
	void ShowGameOver();

	UFUNCTION()
	void HideGameOver();
	
	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_Round;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* can_Round;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* can_GameOver;

	
	
	/*
	
	if(gameMode)
	{
		FString ValueAsString = FString::Printf(TEXT("%.0f") , gameMode->gameTimer);
		text_Timer->SetText(FText::FromString(ValueAsString));
	}
*/

	
	
};
