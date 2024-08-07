// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */

UCLASS()
class JMH_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Can_MainMenu;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Can_ChooseSide;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Can_ChooseCharacter;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Can_VS;

	//MainMenu
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Tournament;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_PVSP;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_PVSCPU;

	//
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_OPTIONS;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_COMMUNITY;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_PLAYDATA;


	//ChoosePlaySide
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Left;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Right;

	//
	
	UPROPERTY(meta = (BindWidget))
	class UImage* img_Player1;

	UPROPERTY(meta = (BindWidget))
	class UImage* img_Player2;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_JIN;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Paul;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Random;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Kazuya;

	//Player1 Info

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player1Name;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player1Info01;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player1Info02;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player1Info03;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player1Info04;

	
	// Player2 Info
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player2Name;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player2Info01;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player2Info02;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player2Info03;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player2Info04;



	//VS
	
	UPROPERTY(meta = (BindWidget))
	class UImage* img_P1;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* img_P2;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_P1Name;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_P2Name;

public:

	UFUNCTION()
	void Tournament();

	UFUNCTION()
	void PlayerVSPlayer();
	
	UFUNCTION()
	void PlayerVSCPU();

	UFUNCTION()
	void LeftSide();

	UFUNCTION()
	void RightSide();

	UFUNCTION()
	void ChooseCharactter();

	
};
