// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateBound.h"
#include "CPP_Tekken8CharacterParent.h"
#include "AICharacterAnimInstance.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
void UAIStateBound::SetAttackInfo ( FAttackInfoInteraction& pAttackInfo )
{
	attackInfoArray.Empty ( );
	attackInfoArray.Add ( pAttackInfo );
}

void UAIStateBound::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsBound" ) , false ); // 원하는 값을 설정
	{
		FVector Direction = owner->GetActorLocation ( ) - owner->aOpponentPlayer->GetActorLocation ( ) + owner->aOpponentPlayer->GetActorForwardVector ( ) * -1000.f;
		Direction.Z = 0.0f; // 수평 방향으로만 계산 (필요 시)

		// 벡터 정규화
		Direction.Normalize ( );

		// 반전된 벡터에 강도 적용 //attackInfoArray[0].KnockBackDirection.X
		FVector v = attackInfoArray[0].KnockBackDirection * 0.7f;
		v.X = 0;
		v.Y = 0;
		FVector LaunchVelocity = v;
		owner->LaunchCharacter ( LaunchVelocity , true , false );
		//owner->GetCharacterMovement ( )->AddImpulse ( attackInfoArray[0].KnockBackDirection * 100.0f , true );
	}
	//공격 받는 애니메이션 추가
	animInstace->StopAllMontages ( 1.0f );
	//animInstace->PlayBoundMontage ( );
}

void UAIStateBound::Execute ( const float& deltatime )
{
	if ( animInstace->velocityZ < 0.1 && owner->GetCharacterMovement ( )->IsFalling())
	{
		FVector Gravity = FVector ( 0 , 0 , -980.0f * 1.0f * deltatime ); // 기본 중력 값은 -980 cm/s^2
		FVector NewLocation = owner->GetActorLocation ( ) + (Gravity);
		owner->SetActorLocation ( NewLocation );
	}

}

void UAIStateBound::Exit ( )
{
	Super::Exit ( );
}
