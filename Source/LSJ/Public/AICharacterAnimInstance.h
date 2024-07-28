// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AICharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAICharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ACharacter* owner;

protected:
	UFUNCTION(BlueprintCallable, Category = "Update Properties")
	virtual void UpdateProperties();
	virtual void NativeUpdateAnimation ( float DeltaSeconds ) override;
	virtual void NativeInitializeAnimation ( ) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float movementSpeed;

	UPROPERTY( VisibleAnywhere , BlueprintReadOnly )
	float direction;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite )
	bool bIsInAir;
};
