// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Tekken8CharacterParent.h"
#include "Animation/AnimMontage.h"
#include "AICharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UAICharacterAnimInstance::UpdateProperties ( )
{
    if ( owner == nullptr )
        owner = Cast<AAICharacter> ( TryGetPawnOwner ( ) );	// 소유자의 Pawn 를 가져온다.
    if ( owner )
    {
        // 공중에 있는지
        bIsInAir = owner->GetMovementComponent ( )->IsFalling ( );

        // Z축이 필요없기 때문에 Z는 0.f로 처리해서 속력를 구한다
        FVector velocity = owner->GetVelocity ( );
        velocityZ = velocity.Z;
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

    //if ( Montage_IsPlaying ( walkBackMontage ) )
    //{
    //    //owner->GetMesh ( )->DetachFromParent();
    //    if ( BeforeLocation == NowLocation )
    //    {
    //        //원점과 중앙좌표의 오차를 저장
    //        NowLocation = owner->GetActorLocation ( ) - owner->GetMesh ( )->GetBoneTransform ( 0 ).GetLocation ( );
    //    }
    //    else
    //    {
    //        //이전의 좌표를 기억
    //        BeforeLocation = NowLocation;
    //        //원점과 중앙좌표의 오차를 저장
    //        NowLocation = owner->GetActorLocation ( ) - owner->GetMesh ( )->GetBoneTransform ( 0 ).GetLocation ( );
    //        FVector v = (BeforeLocation - NowLocation);
    //        owner->SetActorLocation ( owner->GetActorLocation ( ) + v * DeltaSeconds );

    //        //Z값은 언리얼의 Y좌표.캡슐에 지장이 생기니 X Y값만 조정한다
    //        owner->GetMesh ( )->MoveComponent ( FVector ( NowLocation.X* DeltaSeconds , NowLocation.Y* DeltaSeconds , 0 ) , owner->GetActorRotation() , false );
    //    }
    //}
}

void UAICharacterAnimInstance::NativeInitializeAnimation ( )
{
	Super::NativeInitializeAnimation ( );
	if ( owner == nullptr )
	{
		owner = Cast<AAICharacter>(TryGetPawnOwner ( ));	// 소유자의 Pawn 를 가져온다.
	}
    if ( !OnMontageEnded.IsAlreadyBound ( this , &UAICharacterAnimInstance::HandleOnMontageEnded ) )
    OnMontageEnded.AddDynamic ( this , &UAICharacterAnimInstance::HandleOnMontageEnded );
}

UAICharacterAnimInstance::UAICharacterAnimInstance ( )
{
    static ConstructorHelpers::FObjectFinder <UAnimMontage> walkForwardMontageFinder
    ( TEXT ( "/Script/Engine.AnimSequence'/Game/Jaebin/Kazuya/Walk_Forward/Walking_Anim.Walking_Anim'" ) );
    if ( walkForwardMontageFinder.Succeeded ( ) )
        walkForwardMontage = walkForwardMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage> walkBackMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalStep_Backward_Anim1_Montage.FinalStep_Backward_Anim1_Montage'" ) );
    if ( walkBackMontageFinder.Succeeded ( ) )
        walkBackMontage= walkBackMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>idleMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/KB_Idle_T1_Montage.KB_Idle_T1_Montage'" ) );
    if ( idleMontageFinder.Succeeded ( ) )
       idleMontage = idleMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>attackRHMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/Punching_Anim1_Montage.Punching_Anim1_Montage'" ) );
    if ( attackRHMontageFinder.Succeeded ( ) )
        attackRHMontage = attackRHMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>attackLFMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/Kicking_Anim1_Montage.Kicking_Anim1_Montage'" ) );
    if ( attackLFMontageFinder.Succeeded ( ) )
        attackLFMontage = attackLFMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>hitTopMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/Hit_Face_Montage.Hit_Face_Montage'" ) );
    if ( hitTopMontageFinder.Succeeded ( ) )
        hitTopMontage = hitTopMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>hitMiddleMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/Kicking_Anim1_Montage.Kicking_Anim1_Montage'" ) );
    if ( hitMiddleMontageFinder.Succeeded ( ) )
        hitMiddleMontage = hitMiddleMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>uppercutLHMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/KB_Uppercut-LB_S_3_Montage.KB_Uppercut-LB_S_3_Montage'" ) );
    if ( uppercutLHMontageFinder.Succeeded ( ) )
        uppercutLHMontage = uppercutLHMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>comboLaserMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/Combo1/Combo1.Combo1'" ) );
    if ( comboLaserMontageFinder.Succeeded ( ) )
        comboLaserMontage = comboLaserMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>hitFallingMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/FallBack1_Montage.FallBack1_Montage'" ) );
    if ( hitFallingMontageFinder.Succeeded ( ) )
        hitFallingMontage = hitFallingMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>hitFallingTurnMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/Combo1/A_FallBack_Twist_R2_Montage.A_FallBack_Twist_R2_Montage'" ) );
    if ( hitFallingTurnMontageFinder.Succeeded ( ) )
        hitFallingTurnMontage = hitFallingTurnMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>boundMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/NewAnimation6_Montage.NewAnimation6_Montage'" ) );
    if ( boundMontageFinder.Succeeded ( ) )
        boundMontage = boundMontageFinder.Object;
}

void UAICharacterAnimInstance::HandleOnMontageEnded ( UAnimMontage* Montage , bool bInterrupted )
{

    
    // Montage가 끝났을 때의 처리 로직
    if ( bInterrupted )
    {
    // Animation Montage가 정상적으로 끝나지 않고 중간에 인터럽트되었습니다. 인터럽트는 다른 애니메이션이 재생되었거나, 명시적으로 중단되는 등의 이유로 발생할 수 있습니다.

     /*   if ( Montage == hitTopMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::HitGround );
        }*/
    }
    else
    {
    // Animation Montage가 정상적으로 끝났습니다.
        if ( Montage == walkBackMontage )
        {
            //owner->SetActorLocation ( owner->GetActorLocation ( ) + BeforeLocation - NowLocation );
            if ( OnLog ) UE_LOG ( LogTemp , Warning , TEXT ( "walkBackMontage walkBackMontage %s" ) , *Montage->GetName ( ) );
        }
        else
        if ( Montage == attackLFMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::AttackLower );
        }
        else
            if ( Montage == attackRHMontage )
            {
                owner->ExitCurrentState ( ECharacterStateInteraction::AttackLower );
            }

        else
            if ( Montage == uppercutLHMontage )
            {
                owner->ExitCurrentState ( ECharacterStateInteraction::AttackLower );
            }
        else
        if ( Montage == comboLaserMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::AttackLower );
        }
        else if ( Montage == hitMiddleMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::HitGround );
        }
        else if ( Montage == hitTopMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::HitGround );
        }
		 else if ( Montage == hitFallingMontage )
		 {
			 owner->ExitCurrentState ( ECharacterStateInteraction::HitFalling );
		 }
         else if ( Montage == hitFallingTurnMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::HitFalling );
        }
         else if ( Montage == boundMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::HitFalling );
        }
    }
}

void UAICharacterAnimInstance::PlayComboLaserMontage()
{
    Montage_Play(comboLaserMontage);
}

void UAICharacterAnimInstance::PlayBoundMontage ( )
{
    FAlphaBlendArgs a;

    Montage_Play( boundMontage , 0.5f , EMontagePlayReturnType::MontageLength , 0.2f , false );
}

void UAICharacterAnimInstance::PlayHitFallingMontage ( )
{
    Montage_Play ( hitFallingMontage,0.5f);
}

void UAICharacterAnimInstance::PlayHitFallingTurnMontage ( )
{
    Montage_Play ( hitFallingTurnMontage , 0.5f );
}

void UAICharacterAnimInstance::PlayHitTopMontage ( )
{
    Montage_Play ( hitTopMontage );
}

void UAICharacterAnimInstance::PlayHitMiddleMontage ( )
{
    Montage_Play ( hitMiddleMontage );
}

void UAICharacterAnimInstance::PlayerWalkForwardMontage ( )
{
    NowLocation = owner->GetActorLocation ( );
    BeforeLocation = NowLocation;
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
void UAICharacterAnimInstance::PlayeAttackRHMontage ( )
{
    Montage_Play ( attackRHMontage );
}
void UAICharacterAnimInstance::PlayeAttackLFMontage ( )
{
    Montage_Play ( attackLFMontage );
}
void UAICharacterAnimInstance::PlayerIdleMontage ( )
{
    //UAnimMontage* MontageToPlay, float InPlayRate/*= 1.f*/, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages
    Montage_Play (idleMontage);
}

void UAICharacterAnimInstance::AnimNotify_LookTarget ( )
{
    //회전
    FRotator lookPlayerRotator = UKismetMathLibrary::FindLookAtRotation ( owner->GetActorLocation ( ) , owner->aOpponentPlayer->GetActorLocation ( ) );
    owner->targetRotator = lookPlayerRotator;
    owner->bLookTarget = true;
}
