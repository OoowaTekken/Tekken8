// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoUI.h"

#include "GameMode_MH.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerInfoUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerInfoUI::SetPlayerinfo(ACharacter* PlayerA , ACharacter* PlayerB , int32 playerAnum , int32 playerBnum)
{
	GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , TEXT("setplayerinfo"));
	if (PlayerA)
	{
		GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , TEXT("PlayerA is valid."));
		// pual=1,kazuya=2,jin=3
		if (playerAnum == 1)
		{
			//플레이어 1이 폴
			//폴이름, 사진
			text_NameA->SetText(PAUL);

			FSlateBrush Brush;
			Brush.SetResourceObject(img_paul);
			img_PlayerA->SetBrush(Brush);
		}
		else if (playerAnum == 2)
		{
			//카즈야
			//카즈야이름, 사진
			text_NameA->SetText(KAZUYA);

			FSlateBrush Brush;
			Brush.SetResourceObject(img_kazuya);
			img_PlayerA->SetBrush(Brush);
		}
	}

	if (PlayerB)
	{
		if (playerBnum == 1)
		{
			GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , TEXT("PlayerB is valid."));
			//플레이어2 폴
			//폴이름, 사진
			text_NameB->SetText(PAUL);

			FSlateBrush Brush;
			Brush.SetResourceObject(img_paul);
			img_PlayerB->SetBrush(Brush);
		}
		else if (playerBnum == 2)
		{
			//플레이어2 카즈야
			//카즈야이름, 사진
			text_NameB->SetText(KAZUYA);

			FSlateBrush Brush;
			Brush.SetResourceObject(img_kazuya);
			img_PlayerB->SetBrush(Brush);
		}
	}
	//hp 흔들리게, 라운드 연결, hp 깎이기
}

void UPlayerInfoUI::UpdateHealthBars(float Player1CurrentHP , float Player1MaxHP , float Player2CurrentHP ,
                                     float Player2MaxHP)
{
	if (Pro_HPBarA)
	{
		// 플레이어 1의 체력 바 업데이트
		float Player1HealthPercentage = CalculateHealthPercentage(Player1CurrentHP , Player1MaxHP);
		Pro_HPBarA->SetPercent(Player1HealthPercentage);
	}

	if (Pro_HPBarB)
	{
		// 플레이어 2의 체력 바 업데이트
		float Player2HealthPercentage = CalculateHealthPercentage(Player2CurrentHP , Player2MaxHP);
		Pro_HPBarB->SetPercent(Player2HealthPercentage);
	}
}

float UPlayerInfoUI::CalculateHealthPercentage(float CurrentHP , float MaxHP) const
{
	if (MaxHP <= 0.0f)
	{
		return 0.0f;
	}

	return FMath::Clamp(CurrentHP / MaxHP , 0.0f , 1.0f);
}

void UPlayerInfoUI::UpdateRoundImages(int32 RoundScoreA , int32 RoundScoreB)
{
	//playerA와 playerB의 라운드 스코어를 받아와 이미지 업데이트

	switch (RoundScoreA)
	{
	case 1:
		img_VictoryPoint1A->SetVisibility(ESlateVisibility::Visible);
		break;
	case 2:
		img_VictoryPoint1A->SetVisibility(ESlateVisibility::Visible);
		img_VictoryPoint2A->SetVisibility(ESlateVisibility::Visible);
		break;
	case 3:
		img_VictoryPoint1A->SetVisibility(ESlateVisibility::Visible);
		img_VictoryPoint2A->SetVisibility(ESlateVisibility::Visible);
		img_VictoryPoint3A->SetVisibility(ESlateVisibility::Visible);
		break;
	default:
		break;
	}

	switch (RoundScoreB)
	{
	case 1:
		img_VictoryPoint1B->SetVisibility(ESlateVisibility::Visible);
		break;
	case 2:
		img_VictoryPoint1B->SetVisibility(ESlateVisibility::Visible);
		img_VictoryPoint2B->SetVisibility(ESlateVisibility::Visible);
		break;
	case 3:
		img_VictoryPoint1B->SetVisibility(ESlateVisibility::Visible);
		img_VictoryPoint2B->SetVisibility(ESlateVisibility::Visible);
		img_VictoryPoint3B->SetVisibility(ESlateVisibility::Visible);
		break;
	default:
		break;
	}
}

void UPlayerInfoUI::InitRoundImages()
{
	img_VictoryPoint1A->SetVisibility(ESlateVisibility::Hidden);
	img_VictoryPoint2A->SetVisibility(ESlateVisibility::Hidden);
	img_VictoryPoint3A->SetVisibility(ESlateVisibility::Hidden);
	img_VictoryPoint1B->SetVisibility(ESlateVisibility::Hidden);
	img_VictoryPoint2B->SetVisibility(ESlateVisibility::Hidden);
	img_VictoryPoint3B->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerInfoUI::UpdateEndHP(float playerAHP , float playerBHP)
{
	//문자열로 변환
	FString AHPString = FString::Printf(TEXT("%.0f") , playerAHP);
	FString BHPString = FString::Printf(TEXT("%.0f") , playerBHP);

	if (AHPString == TEXT("0") || BHPString == TEXT("0"))
	{
		text_EndHPA->SetText(FText::FromString(TEXT("0")));
		text_EndHPB->SetText(FText::FromString(TEXT("0")));
	}
	else
	{
		// 텍스트 블록에 설정
		text_EndHPA->SetText(FText::FromString(AHPString));
		text_EndHPA->SetText(FText::FromString(BHPString));
	}
}
