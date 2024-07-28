// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCameraPawn::APlayerCameraPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComp);
	//카메라
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void APlayerCameraPawn::BeginPlay()
{
	Super::BeginPlay();

	//월드에 있는 플레이어 변수에 적용
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld() , ACharacter::StaticClass() , FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (FoundActors.Num() >= 2)
		{
			// 플레이어 A,B
			playerA = Cast<ACharacter>(FoundActors[0]);
			playerB = Cast<ACharacter>(FoundActors[1]);
		}
	}

	// 초기화
	PreviousPlayerDistance = 0.0f;
	DistanceThreshold = 350.0f; // 이상의 변화가 있을 때만 arm 길이 조정
	
	// 플레이어의 초기 위치 저장
	if (playerA && playerB)
	{
		InitialPlayerALoc = playerA->GetActorLocation();
		InitialPlayerBLoc = playerB->GetActorLocation();
	}

	//playerALoc = playerA->GetActorLocation();
	//playerBLoc = playerB->GetActorLocation();
	
}

// Called every frame
void APlayerCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraDynamic(DeltaTime);
	
}

// Called to bind functionality to input
void APlayerCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void APlayerCameraPawn::UpdateCameraDynamic(float DeltaTime)
{
	if (!playerA || !playerB) return;
	
	FVector playerALoc = playerA->GetActorLocation();
	FVector playerBLoc = playerB->GetActorLocation();
	
	FVector centralLocation = (playerALoc+playerBLoc) * 0.5f;// + playerBLoc; //그게 그거인듯
	
	// 카메라와 플레이어들 간의 거리 계산
	float playerDistance = FVector::Dist(playerALoc, playerBLoc);

	// 거리 변화가 임계값을 초과하는지 확인
	float distanceChange = FMath::Abs(playerDistance - PreviousPlayerDistance);

	if (distanceChange > DistanceThreshold)
	{
		float newArmLength = FMath::Clamp(baseArmLength + playerDistance * 0.5f, MinDistance, MaxDistance);
		SpringArmComp->TargetArmLength = newArmLength;
	}
	// 두 플레이어 사이의 방향 벡터 계산
	FVector directionVector = playerBLoc - playerALoc;
	directionVector.Normalize();
	
	// 방향 벡터를 회전으로 변환
	FRotator centralRotation = directionVector.Rotation();
	
	// 카메라의 위치와 회전 업데이트
	SetActorLocation(centralLocation+FVector(0,0,20));
	SetActorRotation(centralRotation+FRotator(0,90,0));
	
	PreviousPlayerDistance = playerDistance;
}



