// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"

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
        
		if ( bStateWalkBack && GetInstanceForMontage (walkBackMontage)!=nullptr &&GetInstanceForMontage ( walkBackMontage )->GetBlendTime ( ) < 0.1f )
			bStateWalkBack = false;
        //bStateWalkBack = //실행되고 있을때
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
    OnMontageEnded.AddDynamic ( this , &UAICharacterAnimInstance::HandleOnMontageEnded );
}

UAICharacterAnimInstance::UAICharacterAnimInstance ( )
{
    static ConstructorHelpers::FObjectFinder <UAnimMontage> walkForwardMontageFinder
    ( TEXT ( "/Script/Engine.AnimSequence'/Game/Jaebin/Kazuya/Walk_Forward/Walking_Anim.Walking_Anim'" ) );
    if ( walkForwardMontageFinder.Succeeded ( ) )
        walkForwardMontage = walkForwardMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage> walkBackMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/Step_Backward_Anim_Montage.Step_Backward_Anim_Montage'" ) );
    if ( walkBackMontageFinder.Succeeded ( ) )
        walkBackMontage= walkBackMontageFinder.Object;
}

void UAICharacterAnimInstance::HandleOnMontageEnded ( UAnimMontage* Montage , bool bInterrupted )
{
    if( Montage == walkBackMontage)
        UE_LOG ( LogTemp , Warning , TEXT ( "walkBackMontage walkBackMontage %s" ) , *Montage->GetName ( ) );
    // Montage가 끝났을 때의 처리 로직
    if ( bInterrupted )
    {
    // Animation Montage가 정상적으로 끝나지 않고 중간에 인터럽트되었습니다. 인터럽트는 다른 애니메이션이 재생되었거나, 명시적으로 중단되는 등의 이유로 발생할 수 있습니다.
        UE_LOG ( LogTemp , Warning , TEXT ( "Montage was interrupted. %s" ),*Montage->GetName());
    }
    else
    {
    // Animation Montage가 정상적으로 끝났습니다.
        UE_LOG ( LogTemp , Warning , TEXT ( "Montage ended successfully. %s" ), *Montage->GetName ());
    }
}

void UAICharacterAnimInstance::PlayerWalkForwardMontage ( )
{
    Montage_Play( walkForwardMontage );
}

void UAICharacterAnimInstance::PlayerWalkBackMontage ( )
{
//UAnimMontage* MontageToPlay, float InPlayRate/*= 1.f*/, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages
    Montage_Play (walkBackMontage,1.0f, EMontagePlayReturnType::MontageLength,0.0f,true);
}
void UAICharacterAnimInstance::PlayerBackDashMontage ( )
{
    //UAnimMontage* MontageToPlay, float InPlayRate/*= 1.f*/, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages
    Montage_Play ( walkBackMontage , 3.0f , EMontagePlayReturnType::MontageLength , 0.0f , true );
}