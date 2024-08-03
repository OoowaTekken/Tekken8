﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateWalkForward.h"
#include "AICharacterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"

void UAIStateWalkForward::SetDistance ( float pDistance )
{
	distance = pDistance;
}

void UAIStateWalkForward::Enter (UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter(pAnimInstance);
	if ( owner->GetDistanceTo ( owner->aOpponentPlayer ) < distance )
		Exit ( );
	animInstace->StateWalkForward(true);
}

void UAIStateWalkForward::Execute ( const float& deltatime )
{
	owner->GetMovementComponent()->AddInputVector(owner->GetActorForwardVector());
}

void UAIStateWalkForward::Exit ( )
{
	animInstace->StateWalkForward ( false );
	Super::Exit ( );
}
void UAIStateWalkForward::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent ( DeltaTime , TickType , ThisTickFunction );
	lookPlayerRotator = UKismetMathLibrary::FindLookAtRotation ( owner->GetActorLocation ( ) , owner->aOpponentPlayer->GetActorLocation ( ) );
	owner->SetActorRotation ( FMath::RInterpConstantTo ( owner->GetActorRotation ( ) , lookPlayerRotator , DeltaTime ,200.0f ) );
	if( owner->GetDistanceTo( owner->aOpponentPlayer )<distance)
		Exit();

	// ...
}