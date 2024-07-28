// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/Character.h"

void UAICharacterAnimInstance::UpdateProperties ( )
{
    if ( owner == nullptr )
        owner = Cast<ACharacter> ( TryGetPawnOwner ( ) );	// 소유자의 Pawn 를 가져온다.
    if ( owner )
    {
        // 공중에 있는지
        bIsInAir = owner->GetMovementComponent ( )->IsFalling ( );

        // Z축이 필요없기 때문에 Z는 0.f로 처리해서 속력를 구한다
        FVector velocity = owner->GetVelocity ( );
        velocity = FVector ( velocity.X , velocity.Y , 0.f );
        movementSpeed = FVector ( velocity.X , velocity.Y , 0.f ).Size ( );

        // AnimInstance에 있는 CalculateDriection() 메소드를 통해 방향을 구한다.
        direction = CalculateDirection ( velocity , owner->GetActorRotation ( ) );
    }
}

void UAICharacterAnimInstance::NativeUpdateAnimation ( float DeltaSeconds )
{
    Super::NativeUpdateAnimation ( DeltaSeconds );
    UpdateProperties ( );
}

void UAICharacterAnimInstance::NativeInitializeAnimation ( )
{
	Super::NativeInitializeAnimation ( );
	if ( owner == nullptr )
	{
		owner = Cast<ACharacter>(TryGetPawnOwner ( ));	// 소유자의 Pawn 를 가져온다.
	}
}
