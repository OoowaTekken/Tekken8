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
	DistanceThreshold = 350.0f; // 이상의 변화가 있을 때만 arm 길이 조정

	if (playerA && playerB)
	{
		InitialDirection = (playerB->GetActorLocation() - playerA->GetActorLocation()).GetSafeNormal();
		PreviousDirection = InitialDirection; // 이전 방향 초기화
	}
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

	playerALoc = playerA->GetActorLocation();
	playerBLoc = playerB->GetActorLocation();

	FVector centralLocation = (playerALoc + playerBLoc) * 0.5f; // + playerBLoc; //그게 그거인듯
	// 카메라의 위치 업데이트
	SetActorLocation(centralLocation + FVector(0 , 0 , 20));

	
	// 카메라와 두 플레이어 간의 거리 계산
	float distanceToPlayerA = FVector::Dist(GetActorLocation(), playerALoc);
	float distanceToPlayerB = FVector::Dist(GetActorLocation(), playerBLoc);

	// 더 가까운 플레이어를 기준으로 방향 설정
	//FVector closerPlayerLoc = (distanceToPlayerA < distanceToPlayerB) ? playerALoc : playerBLoc;
	//FVector fartherPlayerLoc = (distanceToPlayerA < distanceToPlayerB) ? playerBLoc : playerALoc;
	
	FVector currentDirection = (playerBLoc - playerALoc).GetSafeNormal();

	float angleDifference = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PreviousDirection, currentDirection)));

	if (angleDifference < 100.0f || angleDifference > 300.0f)
	{
		if (!bIsRotationFixed)
		{
			FRotator centralRotation = currentDirection.Rotation();
			SetActorRotation(centralRotation + FRotator(0, 90, 0));
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
	
	// 플레이어들 간의 거리 계산
	float playerDistance = FVector::Dist(playerALoc , playerBLoc);
	// 거리 변화가 임계값을 초과하는지 확인
	float distanceChange = FMath::Abs(playerDistance);

	if (distanceChange > DistanceThreshold)
	{
		float newArmLength = FMath::Clamp(baseArmLength + playerDistance * 0.5f , MinDistance , MaxDistance);
		SpringArmComp->TargetArmLength = newArmLength;
	}
}
