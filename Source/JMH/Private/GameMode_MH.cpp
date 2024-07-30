// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_MH.h"

#include "inGameUI.h"
#include "Blueprint/UserWidget.h"

AGameMode_MH::AGameMode_MH()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AGameMode_MH::BeginPlay()
{
	Super::BeginPlay();
	
	StartGame();
}

void AGameMode_MH::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CurrentState == EGameState::InProgress)
	{
		CountDown(DeltaTime);
	}
}

void AGameMode_MH::CountDown(float DeltaTime)
{
	gameTimer -= DeltaTime;
	if (inGameUI)
	{
		inGameUI->UpdateTimerDisplay(gameTimer);
	}
	if (gameTimer <= 0.0f)
	{
		//타임 종료
		SetGameState(EGameState::RoundEnd);
	}
}

void AGameMode_MH::SetGameState(EGameState NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
		HandleNewState(NewState);
	}
}

void AGameMode_MH::HandleNewState(EGameState NewState)
{
	switch (NewState)
	{
	case EGameState::GameStart:
		//라운드 시작
		//라운드 초기화
		StartRound();
		break;
	case EGameState::RoundStart:
		//타이머 초기화
		gameTimer = roundTimer;
	//HP 초기화
		SetGameState(EGameState::InProgress);
		break;

	case EGameState::InProgress:
		//게임 진행 중
		//HP체크,타임체크
		break;

	case EGameState::RoundEnd:
		// 라운드 종료 처리
		//HP가 0이 되었을 때 호출,
		//타이머가 0 이 되었을 떄 호출 
		CheckForGameOver();
		break;

	case EGameState::GameOver:
		//게임 종료 처리
		//승자 영상 출력
		break;

	default:
		break;
	}
}

void AGameMode_MH::StartGame()
{
	//게임 플레이어 1,2 시네마틱 영상재생
	SetGameState(EGameState::GameStart);
}

void AGameMode_MH::StartRound()
{
	//게임 UI생성 (타이머, HP,라운드카운트,캐릭터 이미지)
	inGameUI = CreateWidget<UinGameUI>(GetWorld() , inGameWidget);
	if (inGameUI)
	{
		inGameUI->AddToViewport();
		//inGameUI->SetIsEnabled(false);
	}

	SetGameState(EGameState::RoundStart);
}

void AGameMode_MH::CheckForGameOver()
{
	GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , TEXT("GameOver"));
	if (IsGameOverConditionMet())
	{
		SetGameState(EGameState::GameOver);
	}
}

bool AGameMode_MH::IsGameOverConditionMet()
{
	//if 3선승?,타임오버?(체력 많이 남은 플레이어 승)  return true;
	return false;
}
