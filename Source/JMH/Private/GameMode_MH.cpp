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

		initCameraLoc = maincamera->GetActorLocation();
		initCameraRot = maincamera->GetActorRotation();
	}
	//플레이어 초기위치 저장
	if (playerA && playerB)
	{
		initPlayerALoc = playerA->GetActorLocation();
		initPlayerARot = playerA->GetActorRotation();

		initPlayerBLoc = playerB->GetActorLocation();
		initPlayerBRot = playerB->GetActorRotation();

		//플레이어 인풋 막기 왜안돼는지 모르겟
		//playerAController = playerA->GetController();
		//playerBController = playerB->GetController();
		//DisablePlayerInput();
		if (playerA)
		{
			/*
			GEngine->AddOnScreenDebugMessage(-2 , 5.f , FColor::Red , TEXT("!!!!!!!!!!!!!!!!!!!!!!!"));
			//Cast<ACPP_CharacterPaul>(playerA)->SetDead(false);
			UInputComponent* ControlA = playerA->FindComponentByClass<UInputComponent>();
			if (ControlA)
			{
				ControlA->SetActive(false);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-2 , 5.f , FColor::Red , TEXT("...................."));
			}
		}

		if (playerB)
		{
			//PlayerBController = Cast<APlayerController>(playerB->GetController());
		}*/
		}
	}
	//라운드 스코어 초기화
	InitRoundState();

	StartGame();

	//라운드 종료 초기화
	bHasRoundEnded = false;
	bStartRound = false;
}

void AGameMode_MH::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//게임 진행중
	if (CurrentState == EGameState::InProgress)
	{
		//HP 체크
		CheckPlayerHP();
		if (!bHasRoundEnded && bStartRound)
		{
			//타임 체크
			CountDown(DeltaTime);
		}
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


		//inputplayerController = GetWorld()->GetFirstPlayerController();
		//if(inputplayerController)
		//{
		//	DisablePlayerInput();
		//	GEngine->AddOnScreenDebugMessage(-11, 5.f , FColor::Red , TEXT("inputplayerController"));
		//}

		//라운드 시작
		//라운드 초기화
		//라운드 스코어 초기화
		InitRoundState();
		SetGameStart();
		break;
	case EGameState::RoundStart:
		//타이머,HP 초기화
		ResetRoundState();
	//플레이어 위치 초기화
		if (playerA && playerB)
		{
			playerA->SetActorLocation(initPlayerALoc);
			playerA->SetActorRotation(initPlayerARot);
			playerB->SetActorLocation(initPlayerBLoc);
			playerB->SetActorRotation(initPlayerBRot);
		}
		if (maincamera)
		{
			maincamera->SetActorLocation(initCameraLoc);
			maincamera->SetActorRotation(initCameraRot);
		}
	//라운드 num 띄우기
		inGameUI->ShowRoundText(++CurrentRoundNum);

	//5초후 라운드 시작 //인풋 막아두기rl
		GetWorld()->GetTimerManager().SetTimer(TimerHandle , this , &AGameMode_MH::RoundStart , 3.0f , false);

		GEngine->AddOnScreenDebugMessage(-5 , 5.f , FColor::Red , TEXT("RoundStart"));
		break;

	case EGameState::InProgress:
		GEngine->AddOnScreenDebugMessage(-9 , 5.f , FColor::Red , TEXT("InProgress"));

	//게임 진행 중
	//HP체크,타임체크
		break;

	case EGameState::RoundEnd:


		// 라운드 종료 처리
		//HP가 0이 되었을 때 호출,
		//타이머가 0 이 되었을 떄 호출
		//타이머가 0이 된 경우에만 End HP 호출
		//PlayerInfoUI->UpdateEndHP(player1HP,player2HP);	
		//라운드 스코어 ++
		CheckRoundWinner();

	//5초후 라운드 체크-> 다시시작 or 게임오버
		GetWorld()->GetTimerManager().SetTimer(TimerHandle , this , &AGameMode_MH::CheckForGameOver , 3.0f , false);
	//CheckForGameOver();
		GEngine->AddOnScreenDebugMessage(-6 , 5.f , FColor::Red , TEXT("RoundEnd"));
		break;

	case EGameState::GameOver:
		//게임 종료 처리
		inGameUI->ShowGameOver();
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

void AGameMode_MH::SetGameStart()
{
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

void AGameMode_MH::RoundStart()
{
	inGameUI->HideRoundText();
	bStartRound = true;
	SetGameState(EGameState::InProgress);
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
		if (PlayerInfoUI)
		{
			PlayerInfoUI->PlayHPShakeAnim(1);
		}
	}
	else if (Player == playerB)
	{
		player2HP = NewHP;
		if (PlayerInfoUI)
		{
			PlayerInfoUI->PlayHPShakeAnim(2);
		}
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
	else
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

void AGameMode_MH::DisablePlayerInput()
{
	if (PlayerAController)
	{
		PlayerAController->DisableInput(nullptr); // 또는 플레이어 A의 컨트롤러를 비활성화
		GEngine->AddOnScreenDebugMessage(-10 , 5.f , FColor::Red , TEXT("Player A Input Disabled"));
	}

	if (PlayerBController)
	{
		PlayerBController->DisableInput(nullptr); // 또는 플레이어 B의 컨트롤러를 비활성화
		GEngine->AddOnScreenDebugMessage(-11 , 5.f , FColor::Red , TEXT("Player B Input Disabled"));
	}
	/*
	if (inputplayerController)
	{
		inputplayerController->DisableInput(nullptr);
		GEngine->AddOnScreenDebugMessage(-10 , 5.f , FColor::Red , TEXT("DisablePlayerInput"));
	}*/
}

void AGameMode_MH::EnablePlayerInput()
{
	//if (PlayerAController)
	//{
	//PlayerAController->EnableInput(nullptr); // 또는 플레이어 A의 컨트롤러를 활성화
	//GEngine->AddOnScreenDebugMessage(-12, 5.f, FColor::Green, TEXT("Player A Input Enabled"));
	//}

	//if (PlayerBController)
	//{
	//PlayerBController->EnableInput(nullptr); // 또는 플레이어 B의 컨트롤러를 활성화
	//GEngine->AddOnScreenDebugMessage(-13, 5.f, FColor::Green, TEXT("Player B Input Enabled"));
	//}
	/*
	if (inputplayerController)
	{
		inputplayerController->EnableInput(nullptr);
		GEngine->AddOnScreenDebugMessage(-10 , 5.f , FColor::Red , TEXT("EnablePlayerInput"));
	}*/
}

void AGameMode_MH::CheckPlayerHP()
{
	if (playerA && playerB)
	{
		// 각 플레이어의 HP를 체크
		player1HP = GetPlayerHP(playerA);
		player2HP = GetPlayerHP(playerB);

		// HP가 0이거나 낮은 경우 라운드 종료 상태로 변경
		if ((player1HP <= 0 || player2HP <= 0) && !bHasRoundEnded)
		{
			HandleNewState(EGameState::RoundEnd);
			bHasRoundEnded = true;
			bStartRound = false; // 라운드 종료 플래그 설정		
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
	playerA->Hp = player1MaxHP;
	playerB->Hp = player2MaxHP;

	player1HP = player1MaxHP;
	player2HP = player2MaxHP;

	bHasRoundEnded = false;
	// UI를 업데이트
	if (PlayerInfoUI)
	{
		PlayerInfoUI->UpdateHealthBars(player1HP , player1MaxHP , player2HP , player2MaxHP);
	}
}

void AGameMode_MH::InitRoundState()
{
	CurrentRoundNum = initRoundNum;
	Player1Score = 0;
	Player2Score = 0;
	Winner = nullptr;

	ResetRoundState();
}
