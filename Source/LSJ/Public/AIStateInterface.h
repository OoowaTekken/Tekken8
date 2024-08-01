// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LSJ_API IAIStateInterface
{
	GENERATED_BODY()
protected:
	class UAICharacterAnimInstance* animInstace;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Enter ( class UAICharacterAnimInstance* pAnimInstance ) = 0;
	virtual void Execute ( const float& deltatime ) = 0;
	virtual void Exit ( ) = 0;
};
