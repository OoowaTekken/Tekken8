// Fill out your copyright notice in the Description page of Project Settings.


#include "inGameUI.h"

#include "GameMode_MH.h"
#include "PlayerInfoUI.h"
#include "Components/TextBlock.h"

void UinGameUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UinGameUI::UpdateTimerDisplay(float TimerValue)
{
	if (text_Timer)
	{
		// 타이머 값을 문자열로 변환
		FString TimerString = FString::Printf(TEXT("%.0f"), TimerValue);

		// 텍스트 블록에 설정
		text_Timer->SetText(FText::FromString(TimerString));
	}
	
}
