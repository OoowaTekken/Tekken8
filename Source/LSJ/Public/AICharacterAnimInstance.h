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
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* walkForwardMontage;
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* walkBackMontage;
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* idleMontage;
		UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* attackRHMontage;
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* attackLFMontage;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AAICharacter* owner;
	UPROPERTY( EditAnyWhere , BlueprintReadOnly , Category = Move , Meta = (AllowPrivateAccess = true) )
	bool bStateWalkForward;
	UPROPERTY( EditAnyWhere , BlueprintReadOnly , Category = Move , Meta = (AllowPrivateAccess = true) )
	bool bStateWalkBack;
	FVector BeforeLocation;
	FVector NowLocation;
	
	bool OnLog = false;
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
    virtual void HandleOnMontageEnded(class UAnimMontage* Montage, bool bInterrupted);
	virtual void PlayerWalkForwardMontage();
	virtual void PlayerWalkBackMontage();
	virtual void PlayerBackDashMontage();
	virtual void PlayeAttackRHMontage ( );
	virtual void PlayeAttackLFMontage ( );
	virtual void PlayerIdleMontage( );
	virtual bool StateWalkForward ( ) const { return bStateWalkForward; }
	virtual void StateWalkForward ( bool val ) { bStateWalkForward = val; }
	virtual bool StateWalkBack ( ) const { return bStateWalkBack; }
	virtual void StateWalkBack ( bool val ) { bStateWalkBack = val; }
};
