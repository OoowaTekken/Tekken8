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

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
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
			// 플레이어 캐릭터에 대한 작업 수행
			playerA = Cast<ACharacter>(FoundActors[0]);
			playerB = Cast<ACharacter>(FoundActors[1]);
		}
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
	
	FVector playerALoc = playerA->GetActorLocation();
	FVector playerBLoc = playerB->GetActorLocation();

	FRotator playerARot = playerA->GetActorRotation();
	FRotator playerBRot = playerB->GetActorRotation();
	
	FVector centralLocation = (playerALoc-playerBLoc) * 0.5f + playerBLoc;
	//SetActorLocation(centralLocation);

	FRotator centralRotation = FMath::Lerp(playerARot, playerBRot, 0.5f);
	//SetActorRotation(centralRotation);
	
	// 카메라와 플레이어들 간의 거리 계산
	float playerDistance = FVector::Dist(playerALoc, playerBLoc);

	// 카메라와 플레이어 간의 거리 조정 (MinDistance와 MaxDistance 사이로 제한)
	float distanceRatio = FMath::Clamp(playerDistance, MinDistance, MAxDistance);
	FVector direction = (centralLocation - GetActorLocation()).GetSafeNormal();
	FVector desiredCameraLocation = centralLocation - direction * distanceRatio;

	// 카메라의 현재 위치와 회전 가져오기
	FVector currentCameraLocation = CameraComp->GetComponentLocation();
	FRotator currentCameraRotation = CameraComp->GetComponentRotation();

	// 중앙 위치와 카메라가 바라보는 방향 계산
	FVector desiredDirection = (centralLocation - desiredCameraLocation).GetSafeNormal();
	FRotator desiredCameraRotation = desiredDirection.Rotation();

	// 위치와 회전을 보간하여 새로운 카메라 위치와 회전 설정
	FVector newCameraLocation = FMath::VInterpTo(currentCameraLocation, desiredCameraLocation, DeltaTime, CameraLagSpeed);
	FRotator newCameraRotation = FMath::RInterpTo(currentCameraRotation, desiredCameraRotation, DeltaTime, CameraLagSpeed);
	
	// 카메라의 위치와 회전 업데이트
	CameraComp->SetWorldLocation(centralLocation);
	CameraComp->SetWorldRotation(centralRotation);
}



