// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStateComponent.h"
#include "AIStateAttackLF.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAIStateAttackLF : public UAIStateComponent
{
	GENERATED_BODY()
	class AAICharacter* aiCharacter;
public:
	virtual void Enter ( class UAICharacterAnimInstance* pAnimInstance ) override;
	virtual void Execute ( const float& deltatime ) override;
	virtual void Exit ( ) override;

};
