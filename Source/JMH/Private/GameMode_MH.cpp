// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_MH.h"

#include "AICharacter.h"
#include "CPP_CharacterPaul.h"
#include "CPP_InputControl.h"
#include "inGameUI.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AGameMode_MH::AGameMode_MH()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AGameMode_MH::BeginPlay()
{
	Super::BeginPlay();

	StartGame();

	ACPP_InputControl* Control = Cast<ACPP_InputControl>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (this->Player1Class)
	{
		this->Player1 = this->GetWorld()->SpawnActor<ACPP_Tekken8CharacterParent>(
			this->Player1Class , FVector(0 , -300 , 0) , FRotator(0 , 90 , 0));
		Control->Player1 = Cast<ACPP_CharacterPaul>(Player1);
		playerA = Player1;
	}
	if (this->Player2Class)
	{
		this->Player2 = this->GetWorld()->SpawnActor<ACPP_Tekken8CharacterParent>(
			this->Player2Class , FVector(0 , 300 , 0) , FRotator(0 , -90 , 0));
		Control->Player2 = Cast<ACPP_CharacterPaul>(Player2);
		if (this->Player1)
		{
			this->Player1->aOpponentPlayer = Player2;
			this->Player2->aOpponentPlayer = Player1;
		}
		playerB = Player2;
	}

	if (!this->Player1Class)
	{
		ACPP_Tekken8CharacterParent* aiplayer1 = GetWorld()->SpawnActor<ACPP_Tekken8CharacterParent>(PlayerAIClass , FVector(0 , -300 , 0) ,
		                                                             FRotator(0 , 90 , 0));
		if (Player2)
			Player2->aOpponentPlayer = aiplayer1;
		playerA = aiplayer1;
	}
	if (!this->Player2Class)
	{
		ACPP_Tekken8CharacterParent* aiplayer2 = GetWorld()->SpawnActor<ACPP_Tekken8CharacterParent>(PlayerAIClass , FVector(0 , 300 , 0) ,
		                                                             FRotator(0 , -90 , 0));
		if (Player1)
			Player1->aOpponentPlayer = aiplayer2;
		playerB = aiplayer2;
	}

	if (CameraPawn)
	{
		SpawnedCameraPawn = Cast<APawn>(GetWorld()->SpawnActor<APawn>(CameraPawn));

		SetupCameraViewTarget();
		//FTimerHandle CameraSetupTimerHandle;
		//GetWorld()->GetTimerManager().SetTimer(CameraSetupTimerHandle, this,&AGameMode_MH::SetupCameraViewTarget, 0.5f, false);
		maincamera = Cast<APlayerCameraPawn>(SpawnedCameraPawn);
		playerA->SetMainCamera(maincamera);
		playerB->SetMainCamera(maincamera);

	}
	//라운드 스코어 초기화
	Player1Score = 0;
	Player2Score = 0;
	Winner = nullptr;
}

void AGameMode_MH::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//게임 진행중
	if (CurrentState == EGameState::InProgress)
	{
		//HP 체크
		//CheckPlayerHP(DeltaTime);
		//타임 체크
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
		//CheckForGameOver();
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
	//라운드 스코어 초기화
	Player1Score = 0;
	Player2Score = 0;
	Winner = nullptr;

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

void AGameMode_MH::SetupCameraViewTarget()
{
	UCameraComponent* CameraComponent = SpawnedCameraPawn->FindComponentByClass<UCameraComponent>();
	if (CameraComponent)
	{
		GEngine->AddOnScreenDebugMessage(-2 , 5.f , FColor::Red , TEXT("CameraActor!"));
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , TEXT("PlayerController!"));

			PlayerController->SetViewTarget(SpawnedCameraPawn);
		}
	}
}

void AGameMode_MH::CheckRoundWinner()
{
	if (Player1HP > Player2HP)
	{
		// Example: Player 1
		Player1Score += 1;
	}
	else if (Player1HP > Player2HP)
	{
		// Example: Player 2
		Player2Score += 1;
	}
	else
	{
		//무승부
		Player1Score += 1;
		Player2Score += 1;
	}
}

void AGameMode_MH::CheckFinalWinner()
{
	if (Player1Score > Player2Score)
	{
		Winner = UGameplayStatics::GetPlayerCharacter(GetWorld() , 0); // Example: Player 1
	}
	else if (Player2Score > Player1Score)
	{
		Winner = UGameplayStatics::GetPlayerCharacter(GetWorld() , 1); // Example: Player 2
	}
	else
	{
		Winner = nullptr; // Tie
	}
	HandleRoundEnd(Winner);
}

void AGameMode_MH::HandleRoundEnd(AActor* RoundWinner)
{
	if (RoundWinner)
	{
		UE_LOG(LogTemp , Log , TEXT("Round Winner: %s") , *RoundWinner->GetName());
		//애니메이션 송출
	}
	else
	{
		UE_LOG(LogTemp , Log , TEXT("Round ended in a tie."));
	}
}

void AGameMode_MH::CheckPlayerHP(float DeltaTime)
{
	if (playerA && playerB)
	{
		// 각 플레이어의 HP를 체크합니다.
		Player1HP = GetPlayerHP(playerA);
		Player2HP = GetPlayerHP(playerB);

		// HP가 0이거나 낮은 경우 라운드 종료 상태로 변경합니다.
		if (Player1HP <= 0 || Player2HP <= 0)
		{
			HandleNewState(EGameState::RoundEnd);
		}
	}
}

int32 AGameMode_MH::GetPlayerHP(ACharacter* Player)
{
	ACPP_Tekken8CharacterParent* players = Cast<ACPP_Tekken8CharacterParent>(Player);
	if (players)
	{
		return players->Hp;
	}
	else
	{
		return 0;
	}
}
