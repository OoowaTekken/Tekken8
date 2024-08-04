// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_Tekken8CharacterParent.h"
#include "AIStateComponent.h"
#include "AIStateComboLaserAttack.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAIStateComboLaserAttack : public UAIStateComponent
{
	GENERATED_BODY()
	
	class AAICharacter* aiCharacter;
	FRotator lookPlayerRotator;
public:
	virtual void Enter ( class UAICharacterAnimInstance* pAnimInstance ) override;
	virtual void Execute ( const float& deltatime ) override;
	virtual void Exit ( ) override;
};

