// Fill out your copyright notice in the Description page of Project Settings.


#include "inGameUI.h"

#include "GameMode_MH.h"
#include "PlayerInfoUI.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

void UinGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	can_Round->SetVisibility(ESlateVisibility::Collapsed);
	can_GameOver->SetVisibility(ESlateVisibility::Collapsed);
}

void UinGameUI::UpdateTimerDisplay(float TimerValue)
{
	if (text_Timer)
	{
		// 타이머 값을 문자열로 변환
		FString TimerString = FString::Printf(TEXT("%.0f") , TimerValue);

		if (TimerString == TEXT("0"))
		{
			text_Timer->SetText(FText::FromString(TEXT("0")));
		}
		else
		{
			// 텍스트 블록에 설정
			text_Timer->SetText(FText::FromString(TimerString));
		}
	}
}

//라운드 넘버 바꿔주기
void UinGameUI::ShowRoundText(int32 RoundNum)
{
	// int32를 FString으로 변환하여 텍스트 블록에 설정
	if (RoundNum)
	{
		FString Text = FString::Printf(TEXT("%d") , RoundNum);
		if (text_Round)
		{
			text_Round->SetText(FText::FromString(Text));
		}
		if (can_Round)
		{
			can_Round->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UinGameUI::HideRoundText()
{
	if (can_Round)
	{
		can_Round->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UinGameUI::ShowGameOver()
{
	if (can_GameOver)
	{
		can_GameOver->SetVisibility(ESlateVisibility::Visible);
	}
}

void UinGameUI::HideGameOver()
{
	if (can_GameOver)
	{
		can_GameOver->SetVisibility(ESlateVisibility::Collapsed);
	}
}
