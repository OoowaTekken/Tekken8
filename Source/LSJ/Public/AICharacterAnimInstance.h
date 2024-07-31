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
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* walkForwardMontage;
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* walkBackMontage;
	UPROPERTY( EditAnyWhere , BlueprintReadOnly , Category = Move , Meta = (AllowPrivateAccess = true) )
	bool bStateWalkForward;
	UPROPERTY( EditAnyWhere , BlueprintReadOnly , Category = Move , Meta = (AllowPrivateAccess = true) )
	bool bStateWalkBack;
	FVector BeforeLocation;
	FVector NowLocation;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Update Properties")
	virtual void UpdateProperties();
	virtual void NativeUpdateAnimation ( float DeltaSeconds ) override;
	virtual void NativeInitializeAnimation ( ) override;

public:
	UAICharacterAnimInstance();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float movementSpeed;

	UPROPERTY( VisibleAnywhere , BlueprintReadOnly )
	float direction;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite )
	bool bIsInAir;

	 // Animation Montage가 끝났을 때 호출될 함수
    UFUNCTION()
    void HandleOnMontageEnded(class UAnimMontage* Montage, bool bInterrupted);
	void PlayerWalkForwardMontage();
	void PlayerWalkBackMontage();
	void PlayerBackDashMontage();
	bool StateWalkForward ( ) const { return bStateWalkForward; }
	void StateWalkForward ( bool val ) { bStateWalkForward = val; }
	bool StateWalkBack ( ) const { return bStateWalkBack; }
	void StateWalkBack ( bool val ) { bStateWalkBack = val; }
};
