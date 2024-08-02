// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraPawn.h"

#include "GameMode_MH.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Tekken8CharacterParent.h"
// Sets default values
APlayerCameraPawn::APlayerCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComp);
	//카메라
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

void APlayerCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	gm = Cast<AGameMode_MH>(GetWorld()->GetAuthGameMode());
	//월드에 있는 플레이어 변수에 적용 // 플레이어 A,B
	if (gm){
		playerA = gm->playerA;
		playerB = gm->playerB;
	}
	// 초기화
	DistanceThreshold = 350.0f; // 이상의 변화가 있을 때만 arm 길이 조정

	if (playerA && playerB)
	{
		InitialDirection = (playerB->GetActorLocation() - playerA->GetActorLocation()).GetSafeNormal();
		PreviousDirection = InitialDirection; // 이전 방향 초기화
	}
}

void APlayerCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//플레이어가 호출하는 상황 일 때
	if (bIsZoomActive)
	{
		UpdateCameraDynamic(DeltaTime);
		ZoomElapsedTime += DeltaTime;
		if (ZoomElapsedTime >= ZoomDuration)
		{
			// 흔들림 값 초기화 등 필요한 기본값 복귀
			ResetZoomEffect();
		}
	}

	else
	{
		UpdateCameraDynamic(DeltaTime);
	}
}

void APlayerCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCameraPawn::UpdateCameraDynamic(float DeltaTime)
{
	if (!playerA || !playerB) return;

	// 플레이어들 간의 거리 계산
	float playerDistance = FVector::Dist(playerALoc , playerBLoc);
	// 거리 변화가 임계값을 초과하는지 확인
	float distanceChange = FMath::Abs(playerDistance);

	playerALoc = playerA->GetActorLocation();
	playerBLoc = playerB->GetActorLocation();

	if (bIsZoomActive)
	{
		// 줌 효과 적용
		float targetArmLength = FMath::Clamp(baseArmLength * ZoomAmount , MinDistance , MaxDistance);
		SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength , targetArmLength , DeltaTime ,
		                                                  CameraLagSpeed);

		//카메라 쉐이크
		FVector shakeOffset = FVector::ZeroVector;
		if (ShakingValue > 0.0f)
		{
			shakeOffset = FMath::VRand() * ShakingValue;
		}
		
		FVector cameraLocation = FMath::VInterpTo(GetActorLocation() , ZoomTargetLocation + shakeOffset , DeltaTime ,
		                                          CameraLagSpeed);
		SetActorLocation(cameraLocation);
	}
	else
	{
		FVector centralLocation = (playerALoc + playerBLoc) * 0.5f; // + playerBLoc; //그게 그거인듯
		// 카메라의 위치 업데이트
		SetActorLocation(centralLocation + FVector(0 , 0 , 20));

		FVector currentDirection = (playerBLoc - playerALoc).GetSafeNormal();
		currentDirection.Z = 0;
		currentDirection.Normalize();
		float angleDifference = FMath::RadiansToDegrees(
			FMath::Acos(FVector::DotProduct(PreviousDirection , currentDirection)));

		// 목표 회전 계산
		FRotator targetRotation = currentDirection.Rotation() + FRotator(0 , 90 , 0);
		// 현재 회전값 가져오기
		FRotator currentRotation = GetActorRotation();

		//카메라 회전각 제어(180도 무시)
		if (angleDifference < 100.0f || angleDifference > 300.0f)
		{
			if (!bIsRotationFixed)
			{
				// 목표 회전을 부드럽게 보간
				FRotator newRotation = FMath::RInterpTo(currentRotation , targetRotation , DeltaTime ,
				                                        CameraLagRotSpeed);
				SetActorRotation(newRotation);

				PreviousDirection = currentDirection;
			}
		}
		else
		{
			bIsRotationFixed = true;
		}

		if (FMath::Abs(angleDifference) < 90.0f)
		{
			bIsRotationFixed = false;
		}

		if (distanceChange > DistanceThreshold)
		{
			CameraZoom(DeltaTime , playerDistance);
		}
	}
}

void APlayerCameraPawn::CameraZoom(float DeltaTime , float playerDistance)
{
	//플레이어 둘다
	float targetArmLength = FMath::Clamp(baseArmLength + playerDistance * ZoomAmount , MinDistance , MaxDistance);

	//Lerp를 사용하여 카메라 줌을 부드럽게 변경
	newArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength , targetArmLength , DeltaTime , CameraLagSpeed);
	SpringArmComp->TargetArmLength = newArmLength;
}

void APlayerCameraPawn::RequestZoomEffect(FVector TargetLocation , float InZoomAmount , float InShakingValue ,
                                          float InDuration)
{
	ZoomTargetLocation = TargetLocation;
	ZoomAmount = InZoomAmount;
	ShakingValue = InShakingValue;
	ZoomDuration = InDuration;
	ZoomElapsedTime = 0.0f;
	currentArmLength = SpringArmComp->TargetArmLength;
	bIsZoomActive = true;
}

void APlayerCameraPawn::ResetZoomEffect()
{
	ZoomAmount = 0.5f;
	ShakingValue = 0.0f;
	ZoomDuration = 0.0f;
	ZoomElapsedTime = 0.0f;
	SpringArmComp->TargetArmLength = currentArmLength;
	bIsZoomActive= false;
}
