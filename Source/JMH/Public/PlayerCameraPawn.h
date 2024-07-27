// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerCameraPawn.generated.h"

UCLASS()
class JMH_API APlayerCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;
	
	UPROPERTY()
	class ACharacter* playerA;

	UPROPERTY()
	class ACharacter* playerB;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinDistance = 500.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MAxDistance = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraLagSpeed = 5.0f;

	// 카메라가 직각으로 바라보게 할 방향 조정
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraOffsetDistance = 500.0f;
	
	UFUNCTION()
	void UpdateCameraDynamic(float DeltaTime);

	

};
