// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_MH.h"

#include "AICharacter.h"
#include "CPP_CharacterPaul.h"
#include "CPP_InputControl.h"
#include "inGameUI.h"
#include "PlayerInfoUI.h"
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

	ACPP_InputControl* Control = Cast<ACPP_InputControl>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (this->Player1Class)
	{
		this->Player1 = this->GetWorld()->SpawnActor<ACPP_Tekken8CharacterParent>(
			this->Player1Class , FVector(0 , -300 , 0) , FRotator(0 , 90 , 0));
		Control->Player1 = Cast<ACPP_CharacterPaul>(Player1);
		playerA = Player1;

		//mh
		player1MaxHP = playerA->MaxHp;
		//paul
		playerANum = 1;
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

		//mh
		player2MaxHP = playerB->MaxHp;
		//paul
		playerBNum = 1;
	}

	if (!this->Player1Class)
	{
		ACPP_Tekken8CharacterParent* aiplayer1 = GetWorld()->SpawnActor<ACPP_Tekken8CharacterParent>(
			PlayerAIClass , FVector(0 , -300 , 0) ,
			FRotator(0 , 90 , 0));
		if (Player2)
			Player2->aOpponentPlayer = aiplayer1;
		playerA = aiplayer1;

		//mh
		player1MaxHP = playerA->MaxHp;
		//Kazuya
		playerANum = 2;
	}
	if (!this->Player2Class)
	{
		ACPP_Tekken8CharacterParent* aiplayer2 = GetWorld()->SpawnActor<ACPP_Tekken8CharacterParent>(
			PlayerAIClass , FVector(0 , 300 , 0) ,
			FRotator(0 , -90 , 0));
		if (Player1)
			Player1->aOpponentPlayer = aiplayer2;
		playerB = aiplayer2;

		//mh
		player2MaxHP = playerB->MaxHp;
		//Kazuya
		playerBNum = 2;
	}

	if (CameraPawn)
	{
		SpawnedCameraPawn = Cast<APawn>(GetWorld()->SpawnActor<APawn>(CameraPawn));

		SetupCameraViewTarget();

		maincamera = Cast<APlayerCameraPawn>(SpawnedCameraPawn);
		playerA->SetMainCamera(maincamera);
		playerA->aOpponentPlayer = playerB;
		playerB->SetMainCamera(maincamera);
		playerB->aOpponentPlayer = playerA;
	}
	//라운드 스코어 초기화
	InitRoundState();

	StartGame();
}

void AGameMode_MH::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//게임 진행중
	if (CurrentState == EGameState::InProgress)
	{
		//HP 체크
		CheckPlayerHP();
		//타임 체크
		CountDown(DeltaTime);
	}
}

void AGameMode_MH::CountDown(float DeltaTime)
{
	gameTimer -= DeltaTime;
	if (inGameUI)
	{
		if (gameTimer <= 0)
		{
			//타임 종료
			gameTimer = 0;
			SetGameState(EGameState::RoundEnd);
		}
		else
		{
			inGameUI->UpdateTimerDisplay(gameTimer);
		}
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
		//타이머,HP 초기화
		ResetRoundState();
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
		PlayerInfoUI->UpdateEndHP(player1HP,player2HP);	
		//라운드 스코어 ++
		CheckRoundWinner();
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
	//라운드 스코어 초기화
	InitRoundState();

	//게임 UI생성 (타이머, HP,라운드카운트,캐릭터 이미지)
	inGameUI = CreateWidget<UinGameUI>(GetWorld() , inGameWidget);
	if (inGameUI)
	{
		inGameUI->AddToViewport();

		if (playerA && playerB)
		{
			PlayerInfoUI = inGameUI->WBP_PlayerInfo;
			PlayerInfoUI->SetPlayerinfo(playerA , playerB , playerANum , playerBNum);
			PlayerInfoUI->InitRoundImages();
		}
	}

	SetGameState(EGameState::RoundStart);
}

void AGameMode_MH::CheckForGameOver()
{
	if (IsGameOverConditionMet())
	{
	    GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , TEXT("GameOver"));
		SetGameState(EGameState::GameOver);
	}
	else
	{
		SetGameState(EGameState::RoundStart);
	}
}

bool AGameMode_MH::IsGameOverConditionMet()
{
	//if 3선승 return true;
	if (Player1Score == 3 || Player2Score == 3)
	{
		return true;
	}
	return false;
}

void AGameMode_MH::SetupCameraViewTarget()
{
	UCameraComponent* CameraComponent = SpawnedCameraPawn->FindComponentByClass<UCameraComponent>();
	if (CameraComponent)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->SetViewTarget(SpawnedCameraPawn);
		}
	}
}

void AGameMode_MH::UpdatePlayerHP(ACPP_Tekken8CharacterParent* Player , float NewHP)
{
	if (Player == playerA)
	{
		player1HP = NewHP;
	}
	else if (Player == playerB)
	{
		player2HP = NewHP;
	}

	// UI를 업데이트
	if (PlayerInfoUI)
	{
		PlayerInfoUI->UpdateHealthBars(player1HP , player1MaxHP , player2HP , player2MaxHP);
	}
}

float AGameMode_MH::SetShakeIntensity(float Damage)
{
	if (Damage >= 7.0f)
	{
		return 1.0f; // 강한 흔들림
	}
	else if (Damage >= 3.0f)
	{
		return 0.5f; // 중간 흔들림
	}
	else if (Damage > 0.0f)
	{
		return 0.2f; // 약한 흔들림
	}
//일단 흔들리게만 하기... 
	return 0.0f;
}

void AGameMode_MH::CheckRoundWinner()
{
	if (player1HP == player2HP)
	{
		//무승부
		Player1Score += 1;
		Player2Score += 1;
	}
	else if (player1HP > player2HP)
	{
		//Player 1
		Player1Score += 1;
	}
	else if (player1HP < player2HP)
	{
		//Player 2
		Player2Score += 1;
	}
	//라운드 스코어 이미지 업데이트
	PlayerInfoUI->UpdateRoundImages(Player1Score , Player2Score);
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

void AGameMode_MH::CheckPlayerHP()
{
	if (playerA && playerB)
	{
		// 각 플레이어의 HP를 체크
		player1HP = GetPlayerHP(playerA);
		player2HP = GetPlayerHP(playerB);

		// HP가 0이거나 낮은 경우 라운드 종료 상태로 변경
		if (player1HP <= 0 || player2HP <= 0)
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

void AGameMode_MH::ResetRoundState()
{
	gameTimer = initroundTimer;

	player1HP = player1MaxHP;
	player2HP = player2MaxHP;
}

void AGameMode_MH::InitRoundState()
{
	CurrentRoundNum = initRoundNum;
	Player1Score = 0;
	Player2Score = 0;
	Winner = nullptr;

	ResetRoundState();
}
