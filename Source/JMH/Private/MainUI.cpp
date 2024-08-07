// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Tournament)
	{
		Btn_Tournament->OnClicked.AddDynamic(this, &UMainUI::Tournament);
	}
	
	if (Btn_PVSP)
	{
		Btn_PVSP->OnClicked.AddDynamic(this, &UMainUI::PlayerVSPlayer);
	}

	if (Btn_PVSCPU)
	{
		Btn_PVSCPU->OnClicked.AddDynamic(this, &UMainUI::PlayerVSCPU);
	}

	if (Btn_Left)
	{
		Btn_Left->OnClicked.AddDynamic(this, &UMainUI::LeftSide);
	}
	
	if (Btn_Right)
	{
		Btn_Right->OnClicked.AddDynamic(this, &UMainUI::RightSide);
	}
}

void UMainUI::Tournament()
{
	//토너먼트 선택이라고 변수 게임모드 전달
	if (Can_ChooseCharacter)
	{
		//캐릭터 선택으로 이동
		WidgetSwitcher->SetActiveWidgetIndex(2);
	}
}

void UMainUI::PlayerVSPlayer()
{
	//플레이어 VS 플레이어라고 변수 게임모드 전달
	if (Can_ChooseCharacter)
	{
		//캐릭터 선택으로 이동
		WidgetSwitcher->SetActiveWidgetIndex(2);
	}
}

void UMainUI::PlayerVSCPU()
{
	//플레이어 VS 플레이어라고 변수 게임모드 전달
	if (Can_ChooseCharacter)
	{
		//플레이사이드 선택으로 이동
		WidgetSwitcher->SetActiveWidgetIndex(1);
	}
}

void UMainUI::LeftSide()
{
	// 1P 위치에서 스폰, 1P
	//게임모드로 1p에 생성되고 2P는 NULL되게
	if (Can_ChooseCharacter)
	{
		//캐릭터 선택으로 이동
		WidgetSwitcher->SetActiveWidgetIndex(2);
	}
}

void UMainUI::RightSide()
{
	//2P 위치에서 스폰, 2P
	//게임모드로 2p에 생성되고 1P는 NULL되게
	if (Can_ChooseCharacter)
	{
		//캐릭터 선택으로 이동
		WidgetSwitcher->SetActiveWidgetIndex(2);
	}
}

void UMainUI::ChooseCharactter()
{
	//어떤 캐릭 눌렀냐에 따라 게임모드로 메쉬 전달
	//캐릭터 선택 ->
	
}
