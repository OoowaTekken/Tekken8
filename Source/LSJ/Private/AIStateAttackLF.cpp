// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateAttackLF.h"
#include "AICharacterAnimInstance.h"

void UAIStateAttackLF::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	animInstace->PlayeAttackLFMontage ( );
}

void UAIStateAttackLF::Execute ( const float& deltatime )
{
}

void UAIStateAttackLF::Exit ( )
{
	Super::Exit ( );
}
