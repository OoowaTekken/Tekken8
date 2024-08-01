// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateAttackRH.h"
#include "AICharacterAnimInstance.h"

void UAIStateAttackRH::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter(pAnimInstance);
	animInstace->PlayeAttackRHMontage ();
}

void UAIStateAttackRH::Execute ( const float& deltatime )
{
}

void UAIStateAttackRH::Exit ( )
{
	Super::Exit ( );
}
