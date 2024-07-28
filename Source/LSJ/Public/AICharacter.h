// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
#include "Tekken8CharacterParent.h"
#include "AICharacter.generated.h"

UCLASS()
class LSJ_API AAICharacter : public ATekken8CharacterParent
{
	GENERATED_BODY()

	UPROPERTY (EditDefaultsOnly)
	class UAnimInstance* animInstance;

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
