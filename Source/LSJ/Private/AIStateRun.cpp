// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateRun.h"
#include "AICharacterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAIStateRun::SetDistance ( float pDistance )
{
	distance = pDistance;
}

void UAIStateRun::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	animInstace->bRun = true;
}

void UAIStateRun::Execute ( const float& deltatime )
{
	if(nullptr==owner)
		return;
	if( nullptr == owner->aOpponentPlayer)
		return;
	owner->LookTarget (deltatime);
	owner->GetCharacterMovement()->AddInputVector(owner->GetActorForwardVector()*deltatime* moveSpeed );

	if(distance>FVector::Dist( owner->GetActorLocation(),owner->aOpponentPlayer->GetActorLocation()))
	{
		Exit();
		return;
	}
}

void UAIStateRun::Exit ( )
{
	animInstace->bRun = false;
	owner->SetStateIdle();
	Super::Exit ( );
}
