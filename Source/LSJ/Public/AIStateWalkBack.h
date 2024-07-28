// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStateComponent.h"
#include "AIStateWalkBack.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAIStateWalkBack : public UAIStateComponent
{
	GENERATED_BODY()
	
public:
	virtual void Enter ( class UAICharacterAnimInstance* pAnimInstance ) override;
	virtual void Execute ( ) override;
	virtual void Exit ( ) override;
	virtual void TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction ) override;
};