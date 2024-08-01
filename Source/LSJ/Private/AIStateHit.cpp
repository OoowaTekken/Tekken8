// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateHit.h"

void UAIStateHit::SetAttackInfo ( FAttackInfoInteraction pAttackInfo )
{
	attackInfo=pAttackInfo;
}

void UAIStateHit::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	//공격 받는 애니메이션 추가
	if( attackInfo.DamagePoint == EDamagePointInteraction::Top)
		animInstace->PlayHitTopMontage ( );
	else
		animInstace->PlayHitMiddleMontage();
}

void UAIStateHit::Execute ( const float& deltatime )
{
	
}

void UAIStateHit::Exit ( )
{
	Super::Exit();
}
