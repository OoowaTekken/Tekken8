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
	class USceneComponent* RootComp;
	
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ACharacter* playerA;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ACharacter* playerB;

	FVector playerALoc;
	FVector playerBLoc;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	float baseArmLength = 50.f;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinDistance = 50.f;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxDistance = 400.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraLagSpeed = 1.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraLagRotSpeed = 15.f;
	
	UFUNCTION()
	void UpdateCameraDynamic(float DeltaTime);
	
	UFUNCTION()
	void CameraZoom(float DeltaTime, float playerDistance);

private:
	
	FVector InitialDirection;
	FVector PreviousDirection;
	float DistanceThreshold; // 거리 변화 임계값
	float RotationThreshold = 1.0f;// 회전 변화 임계값
	float newArmLength;
	
	bool bIsRotationFixed;

	UPROPERTY()
	class AGameMode_MH* gm;

	UFUNCTION()
	void RequestZoomEffect(FVector TargetLocation, float InZoomAmount, float InShakingValue, float InDuration);
private:
	FVector ZoomTargetLocation;
	float ZoomAmount = 0.5f;
	float ShakingValue = 0.0f;
	bool bIsZoomActive= false;
	float ZoomDuration = 0.0f;
	float ZoomElapsedTime = 0.0f;
	

	


};
