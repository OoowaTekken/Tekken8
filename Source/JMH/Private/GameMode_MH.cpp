// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_MH.h"

AGameMode_MH::AGameMode_MH()
{
	// 기본 상태 초기화
	CurrentState = EGameState::GameStart;
}

void AGameMode_MH::BeginPlay()
{
	Super::BeginPlay();
	StartGame();
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
		StartRound();
		break;
	case EGameState::RoundStart:
		//라운드 시작
		SetGameState(EGameState::InProgress);
		break;

	case EGameState::InProgress:
		// 게임 진행 중
		//HP체크,타임체크
		break;

	case EGameState::RoundEnd:
		// 라운드 종료 처리
		CheckForGameOver();
		break;

	case EGameState::GameOver:
		// 게임 종료 처리
			//승자 영상 출력
			break;
		
	default:
		break;
	}
}

void AGameMode_MH::StartGame()
{
	//게임 플레이어 1,2 영상 재생
	SetGameState(EGameState::GameStart);
}

void AGameMode_MH::StartRound()
{
	//플레이어 스폰.
	//게임 UI생성 (타이머, HP,라운드카운트)
	SetGameState(EGameState::RoundStart);
}

void AGameMode_MH::CheckForGameOver()
{
	if (IsGameOverConditionMet())
	{
		SetGameState(EGameState::GameOver);
	}
}

bool AGameMode_MH::IsGameOverConditionMet()
{
	// 게임 오버 조건 체크
	//if 3선승?,타임오버?(체력 많이 남은 플레이어 승)  return true;
	return false;
}
