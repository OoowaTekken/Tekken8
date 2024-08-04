// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PualAnimInstance.h"
#include "CPP_CharacterPaul.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCPP_PualAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACPP_CharacterPaul* player = Cast<ACPP_CharacterPaul>(TryGetPawnOwner());

	if (nullptr == player)
		return;

	FVector velocity = player->GetVelocity();

	this->fVertical = FVector::DotProduct(player->GetActorForwardVector(), velocity);
	this->fHorizontal = FVector::DotProduct(player->GetActorRightVector(), velocity);

	this->bCrouch = player->bCrouched;
	this->bFalling =  player->GetCharacterMovement()->IsFalling();

}
