// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStateComponent.h"
#include "AIStateWalkCross.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAIStateWalkCross : public UAIStateComponent
{
	GENERATED_BODY()
	//FVector Center;
	//float RadAngle;
	float direction = 1.0f;
	float reduceRange = 20.f;
	float OrbitRadius;
	float OrbitSpeed;
	float CurrentAngle;
	float startFrame;
	float endFrame;
	//FVector OrbitLocation;
public:
	UPROPERTY(EditAnywhere, Category = "AI")
	bool  bClockwise;
	virtual void Enter (class UAICharacterAnimInstance* pAnimInstance ) override;
	virtual void Execute ( const float& deltatime ) override;
	virtual void Exit ( ) override;

	void OrbitAroundTarget (float Speed );
	void MoveStep ( float DeltaTime );
};
