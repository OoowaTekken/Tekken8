// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateAttackRH.h"
#include "AICharacterAnimInstance.h"
#include "CPP_CharacterPaul.h"
#include "AICharacter.h"
#include "Components/SphereComponent.h"
void UAIStateAttackRH::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter(pAnimInstance);

	animInstace->PlayeAttackRHMontage ();
	aiCharacter = Cast<AAICharacter> ( owner );
}

void UAIStateAttackRH::Execute ( const float& deltatime )
{
	ToLookTargetRotate ( deltatime );
}

void UAIStateAttackRH::Exit ( )
{
	Super::Exit ( );
}
