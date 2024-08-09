﻿// Fill out your copyright notice in the Description page of Project Settings.
#define MYDEBUGMODE 0;

#include "CPP_CharacterPaul.h"

#include "GameMode_MH.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameMode_MH.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "CPP_PualAnimInstance.h"

// Sets default values
ACPP_CharacterPaul::ACPP_CharacterPaul ( )
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->uCharacterMesh = this->GetMesh ( );

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSkeletalMesh ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'" ) );
	if ( tempSkeletalMesh.Succeeded ( ) )
		uCharacterMesh->SetSkeletalMeshAsset ( tempSkeletalMesh.Object );
	uCharacterMesh->SetRelativeLocation ( FVector ( 0 , 0 , -90 ) );
	uCharacterMesh->SetRelativeRotation ( FRotator ( 0 , 0 , -90 ) );
	uCharacterMesh->SetWorldScale3D(FVector(0.1,0.1,0.1));
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
// Called when the game starts or when spawned
void ACPP_CharacterPaul::BeginPlay ( )
{
	Super::BeginPlay ( );

	// 커멘드를 설정합니다.
	this->SettingCommandTree ( );
	// 위치 세팅
	this->ToLocation = this->GetActorLocation ( ) + this->GetActorForwardVector ( ) * 100;
	this->sFrameStatus.FrameBlockUsing = 0;
	this->sFrameStatus.FrameUsing = 0;
	GameModeMH = Cast<AGameMode_MH>( GetWorld ( )->GetAuthGameMode ( ) );

	uAnim = this->GetMesh ( )->GetAnimInstance ( );
	
}


// Called to bind functionality to input
void ACPP_CharacterPaul::SetupPlayerInputComponent ( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent ( PlayerInputComponent );
}

/************************************************************************/
/*                     기본적인 게임에 대한 설정                        */
/************************************************************************/


FInputKey ACPP_CharacterPaul::GetCurrInputKey ( )
{
	return this->CurrInputKey;
}

int32 ACPP_CharacterPaul::GetCurrInputKeyValue ( )
{
	int32 KeyValue =
		//this->CurrInputKey.bKey1		* 0b0000000000000001 +
		this->CurrInputKey.bKey2 * 0b0000000000000010 +
		//this->CurrInputKey.bKey3		* 0b0000000000000100 +
		this->CurrInputKey.bKey4 * 0b0000000000001000 +
		//this->CurrInputKey.bKey5		* 0b0000000000010000 +
		this->CurrInputKey.bKey6 * 0b0000000000100000 +
		//this->CurrInputKey.bKey7		* 0b0000000001000000 +
		this->CurrInputKey.bKey8 * 0b0000000010000000 +
		//this->CurrInputKey.bKey9		* 0b0000000100000000 +
		this->CurrInputKey.bLeftPunch * 0b0000001000000000 +
		this->CurrInputKey.bRightPunch * 0b0000010000000000 +
		this->CurrInputKey.bLeftKick * 0b0000100000000000 +
		this->CurrInputKey.bRightKick * 0b0001000000000000;

	return KeyValue;
}

bool ACPP_CharacterPaul::SetCurrInputKeyValue ( int32 value )
{
	//this->CurrInputKey.bKey1		= value & 0b0000000000000001;
	this->CurrInputKey.bKey2 = value & 0b0000000000000010;
	//this->CurrInputKey.bKey3		= value & 0b0000000000000100;
	this->CurrInputKey.bKey4 = value & 0b0000000000001000;
	//this->CurrInputKey.bKey5		= value & 0b0000000000010000;
	this->CurrInputKey.bKey6 = value & 0b0000000000100000;
	//this->CurrInputKey.bKey7		= value & 0b0000000001000000;
	this->CurrInputKey.bKey8 = value & 0b0000000010000000;
	//this->CurrInputKey.bKey9		= value & 0b0000000100000000;
	this->CurrInputKey.bLeftPunch = value & 0b0000001000000000;
	this->CurrInputKey.bRightPunch = value & 0b0000010000000000;
	this->CurrInputKey.bLeftKick = value & 0b0000100000000000;
	this->CurrInputKey.bRightKick = value & 0b0001000000000000;

	return true;
}

bool ACPP_CharacterPaul::SetCurrInputKey ( FInputKey value )
{
	this->CurrInputKey = value;
	return true;
}

int32 ACPP_CharacterPaul::InputKeyValue ( int ArrowKey , bool LeftArm , bool RightArm , bool LeftKick , bool RightKick )
{
	int32 inputValue = 0;

	inputValue = (ArrowKey ? 1 << (ArrowKey - 1) : 0) +
		LeftArm * 0b0000001000000000 +
		RightArm * 0b0000010000000000 +
		LeftKick * 0b0000100000000000 +
		RightKick * 0b0001000000000000;
	return inputValue;
}


/************************************************************************/
/*       프레임 작업에 대한 정보입니다                                  */
/************************************************************************/

// Called every frame
void ACPP_CharacterPaul::Tick ( float DeltaTime )
{
	Super::Tick ( DeltaTime );
// 	if ( this->GameModeMH->Player2 == this )
// 		return;
// 	if (this->GameModeMH->Player1 == this)
// 		this->DebugMode = 1;

	// 주의 모든 작업은 FrameSystem에서 하는걸 권장합니다.
	this->fCurrTimeForFrame += DeltaTime;

	if ( this->fCurrTimeForFrame <= this->fFrameTime )
		return;
	sFrameStatus.FrameBlockUsing--;
	sFrameStatus.FrameUsing--;
	sAttackInfo.ActionFrame--;
	iCurrFrame++;

	if ( this->bIsDead )
		return;

	// 다음 행동 정의
	this->FrameSystem ( );
}

/**
 * @title Frame 시스템을 위한 함수입니다.
 */
void ACPP_CharacterPaul::FrameSystem ( )
{
	AnimationFrame();
	this->currKeyValue = this->GetCurrInputKeyValue ( );

	this->fCurrTimeForFrame = 0;

	// 맞음으로 인한 모든 행동 불가 액션취소
	if ( sFrameStatus.FrameBlockUsing > 0 )
	{
		sAttackInfo.ActionFrame = -1;
		return;
	}
	//this->AnimationFrame ( );
	// 액션 프레임 발동
	if ( sAttackInfo.ActionFrame == 0 )
		this->CommentHitFrameExecute ( );

	// 다음행동 예약
	// 예약된 다음 행동 실행
	PlayNextAction ( );
}

bool ACPP_CharacterPaul::PlayNextAction()
{
	if ( currKeyValue != 0 && this->GameModeMH->Player1 == this )
		DebugMode = 1;
	if ( sFrameStatus.FrameUsing < -1 && (!sCurrCommand || !sCurrCommand->NextKeys.Num() || !CheckKeyArray()) )
	{
		UE_LOG(LogTemp, Warning, TEXT("is clean up"));
		sNextCommand = mBaseCommandTree[0];
		sCurrCommand = sNextCommand;
		sNextCommand = nullptr;
		sCurrCommandKeys = sCurrCommand->NextKeys;
		return false;
	}
	if ( sCurrCommand->NextTrees.Find ( currKeyValue ) )
	{
		FCommandTree* temptree = sCurrCommand->NextTrees[currKeyValue];
		if ( temptree->timingStart <= iCurrFrame )
			if ( sNextCommand != temptree )
				sNextCommand = temptree;
	}
	if ( sFrameStatus.FrameUsing < 0 )
	{
		if ( sCurrCommand )
		{
			sCurrCommand->action.Execute ( );
		}
 		if (!sNextCommand)
 			sNextCommand = mBaseCommandTree[0];
		sCurrCommand = sNextCommand;
		sCurrCommandKeys = sCurrCommand->NextKeys;
	}
	return false;
}

bool ACPP_CharacterPaul::CheckKeyArray ( )
{
	TArray<int32> tempNextKeys;
	tempNextKeys = sCurrCommandKeys;
	for ( int32 nextKey : tempNextKeys )
	{
		if ( sCurrCommand->NextTrees[nextKey]->timingEnd && sCurrCommand->NextTrees[nextKey]->timingEnd < iCurrFrame )
			sCurrCommandKeys.Remove ( nextKey );
	}
	if ( tempNextKeys.IsEmpty ( ) )
		return false;
	return true;
}
// 스킬 인포를 정의하기 위한 함수
void ACPP_CharacterPaul::SetActtacInfoSkell ( EDamagePointInteraction damagePointInteraction, 
	int32 damageAmount = 0 , int32 actionFrame = 0, int32 retrieveFrame = 0,
	int32 missFrame = 0, int32 hitFrame = 0, int32 grardFrame = 0, int32 counterFrame = 0)
{
	sAttackInfo.DamagePoint = damagePointInteraction;
	sAttackInfo.DamageAmount = damageAmount;

	sAttackInfo.ActionFrame = actionFrame;
	sAttackInfo.RetrieveFrame = retrieveFrame;

	sAttackInfo.MissFrame = missFrame;
	sAttackInfo.HitFrame = hitFrame;
	sAttackInfo.GrardFrame = grardFrame;
	sAttackInfo.CounterFrame = counterFrame;

	this->SetAttackInfoOwnerOpposite();
}

// 자동 계산 함수
void ACPP_CharacterPaul::SetAttackInfoOwnerOpposite ( )
{
	//공격자
	sAttackInfo.OwnerHitFrame = sAttackInfo.RetrieveFrame + (sAttackInfo.HitFrame < 0 ? sAttackInfo.HitFrame * -1 : 0);
	sAttackInfo.OwnerGuardFrame = sAttackInfo.RetrieveFrame + (sAttackInfo.GrardFrame < 0 ? sAttackInfo.GrardFrame * -1 : 0);
	sAttackInfo.OwnerCounterFrame = sAttackInfo.RetrieveFrame + (sAttackInfo.CounterFrame < 0 ? sAttackInfo.CounterFrame * -1 : 0);
	sAttackInfo.OwnerMissFrame = sAttackInfo.RetrieveFrame + (sAttackInfo.MissFrame < 0 ? sAttackInfo.MissFrame * -1 : 0);

	//수비자
	sAttackInfo.OppositeHitFrame = sAttackInfo.RetrieveFrame + (sAttackInfo.HitFrame > 0 ? sAttackInfo.HitFrame : 0);
	sAttackInfo.OppositeGuardFrame = sAttackInfo.RetrieveFrame + (sAttackInfo.GrardFrame > 0 ? sAttackInfo.GrardFrame : 0);
	sAttackInfo.OppositeCounterFrame = sAttackInfo.RetrieveFrame + (sAttackInfo.CounterFrame > 0 ? sAttackInfo.CounterFrame : 0);
}

/************************************************************************/
/*                    애니메이션을 위한  작업입니다                     */
/************************************************************************/

void ACPP_CharacterPaul::AnimationFrame ( )
{
	if ( bMoveTo )
	{
		if (!this->ToLocationFrame.IsEmpty())
		{
			FVector dir = this->ToLocationFrame.Pop ( );
			AddMovementInput ( dir );
		}
		//this->SetActorLocation ( this->GetActorLocation ( ) + dir );
	}
	// 인풋이 있을 경우 상대를 바라본다 
	if ( currKeyValue )
	{
		FRotator Lookrotation = UKismetMathLibrary::FindLookAtRotation ( this->GetActorLocation ( ) , this->aOpponentPlayer->GetActorLocation ( ) );
		Lookrotation.Pitch = this->GetActorRotation ( ).Pitch;
		this->SetActorRotation ( Lookrotation );
	}
}

void ACPP_CharacterPaul::SetToLocationFrame ( FVector dir , int32 frame )
{
	FVector relativeDir = RelativePointVector ( dir.X , dir.Y , dir.Z );
	FVector	frameDir =  (relativeDir - this->GetActorLocation()) / frame;
	this->ToLocationFrame.Empty();
	for ( int32 i = 0 ; i < frame ; i++ )
		this->ToLocationFrame.Add(frameDir);
}

void ACPP_CharacterPaul::SetToLocationPoint ( float x , float y , float z )
{
	FVector moveToPoint = this->GetActorLocation ( ) +
		(
			this->GetActorForwardVector ( ) * x +
			this->GetActorRightVector ( ) * y +
			this->GetActorUpVector ( ) * z
		);
	this->ToLocation = moveToPoint;

	this->ToLocationFrame.Push(FVector(1,1,1));
}

void ACPP_CharacterPaul::SetToLocationPoint ( FVector vector )
{
	FVector moveToPoint = this->GetActorLocation ( ) +
		(
			this->GetActorForwardVector ( ) * vector.X +
			this->GetActorRightVector ( ) * vector.Y +
			this->GetActorUpVector ( ) * vector.Z
		);
	this->ToLocation = moveToPoint;
}


void ACPP_CharacterPaul::SetToWorldLocationPoint ( FVector vector )
{
	this->ToLocation = vector;
}


FVector ACPP_CharacterPaul::RelativePointVector ( float x , float y , float z )
{
	FVector relativePoint = this->GetActorLocation ( ) +
		(
			this->GetActorForwardVector ( ) * x +
			this->GetActorRightVector ( ) * y +
			this->GetActorUpVector ( ) * z
		);

	return relativePoint;
}

bool ACPP_CharacterPaul::PlayMontageFrameSystem ( UAnimMontage* montage )
{
	if ( !uAnim || !montage )
		return false;
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "Montage Start" ) );
	UCPP_PualAnimInstance* tempAnim = Cast<UCPP_PualAnimInstance> ( uAnim );
	tempAnim->Montage_Play ( montage );
	return true;
}

/************************************************************************/
/*                커멘드 크리에 대한 정보                               */
/************************************************************************/

// Command Setting Function

void ACPP_CharacterPaul::SettingCommandTree ( )
{
	this->AddCommandTree ( mBaseCommandTree , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandIdle );
	mBaseCommandTree[0]->NextTrees.Add ( InputKeyValue ( 0 , 0 , 0 , 0 , 0 ) , mBaseCommandTree[0] );

	int32 forwardkey = InputKeyValue ( 6 , 0 , 0 , 0 , 0 );
	int32 backkey = InputKeyValue ( 4 , 0 , 0 , 0 , 0 );
	int32 upkey = InputKeyValue ( 8 , 0 , 0 , 0 , 0 );
	int32 downkey = InputKeyValue ( 2 , 0 , 0 , 0 , 0 );
	int32 LP = InputKeyValue ( 0 , true , 0 , 0 , 0 );
	int32 RP = InputKeyValue ( 0 , 0 , true , 0 , 0 );
	int32 LK = InputKeyValue ( 0 , 0 , 0 , true , 0 );
	int32 RK = InputKeyValue ( 0 , 0 , 0 , 0 , true );
	
	int32 i1key = backkey + downkey;
	int32 i3key = forwardkey + downkey;
	int32 i7key = backkey + upkey;
	int32 i9key = backkey + upkey;

	int32 downLPkey = downkey + LP;
	int32 downRPkey = downkey + RP;
	int32 downLKkey = downkey + LK;
	int32 downRKkey = downkey + RK;

	/************************************************************************/
	/*                          Command Tree Create                         */
	/************************************************************************/
	/**
	 *  ForwardKey
	 */
	this->AddCommandBaseTree ( { 0 } , forwardkey , 0 , 5 , &ACPP_CharacterPaul::CommandMoveForward );
	SetSelfReLinkTree ( { 0,forwardkey } );
	// MoveForward Dash
	this->AddCommandBaseTree ( { 0, forwardkey } , 0 , 0 , 3 , &ACPP_CharacterPaul::CommandStar );
	
	// DASH while
	this->AddCommandBaseTree ( { 0, forwardkey, 0 } , forwardkey , 0 , 3 , &ACPP_CharacterPaul::CommandMoveForwarDash );
	SetSelfReLinkTree ( { 0,forwardkey,0, forwardkey } );
	this->AddCommandBaseTree ( { 0, forwardkey, 0, forwardkey } , 0 , 0 , 3 , &ACPP_CharacterPaul::CommandEnd );


	/**
	 *  BackKey
	 */
	this->AddCommandBaseTree ( { 0 } , backkey , 0 , 5 , &ACPP_CharacterPaul::CommandMoveBack );
	SetSelfReLinkTree ( { 0,backkey } );
	// MoveBack Dash
	this->AddCommandBaseTree ( { 0, backkey } , 0 , 0 , 3 , &ACPP_CharacterPaul::CommandStar );
	// DASH while
	this->AddCommandBaseTree ( { 0, backkey, 0 } , backkey , 0 , 3 , &ACPP_CharacterPaul::CommandMoveBackDash );
	this->AddCommandBaseTree ( { 0, backkey, 0, backkey } , backkey , 0 , 3 , &ACPP_CharacterPaul::CommandEnd );
	this->AddCommandBaseTree ( { 0, backkey, 0, backkey } , 0 , 0 , 3 , &ACPP_CharacterPaul::CommandEnd );

	/**
	 * UPKey
	 */
	 // Move Lateral Plus
	this->AddCommandBaseTree ( { 0 } , upkey , 0 , 3 , &ACPP_CharacterPaul::CommandStarWaitNext );
	// 3frame wait
	this->AddCommandBaseTree ( { 0, upkey } , 0 , 4, 6 , &ACPP_CharacterPaul::CommandMoveLateralUpDash );
	this->AddCommandBaseTree ( { 0, upkey, 0 } , 0 , 5 , 10 , &ACPP_CharacterPaul::CommandStar );
	// Move while Lateral
	this->AddCommandBaseTree ( { 0, upkey, 0, 0 } , upkey , 0 , 3 , &ACPP_CharacterPaul::CommandMoveLateralUpLoop );
	SetSelfReLinkTree ( { 0,upkey, 0, 0, upkey } );
	this->AddCommandBaseTree ( { 0,upkey,0, 0, upkey } , 0 , 0 , 3 , &ACPP_CharacterPaul::CommandEnd );

	// Jump
	this->AddCommandBaseTree ( { 0 , upkey } , upkey , 5 , 6 , &ACPP_CharacterPaul::CommandJump );
	this->AddCommandBaseTree ( { 0 , upkey, upkey } , upkey , 0 , 3 , &ACPP_CharacterPaul::CommandEnd );
	//SetSelfReLinkTree ( { 0,upkey,0,0, upkey, upkey, upkey } );

	/**
	 * DownKey
	 */
	 // Move Lateral Minus
	this->AddCommandBaseTree ( { 0 } , downkey , 0 , 3 , &ACPP_CharacterPaul::CommandStarWaitNext );
	// 3frame wait
	this->AddCommandBaseTree ( { 0 ,downkey } , 0 , 4 , 6 , &ACPP_CharacterPaul::CommandMoveLateralDownDash );
	this->AddCommandBaseTree ( { 0 ,downkey, 0 } , 0 , 5 , 10 , &ACPP_CharacterPaul::CommandStar );
	// Move while Lateral
	this->AddCommandBaseTree ( { 0 ,downkey, 0, 0 } , downkey , 0 , 0  , &ACPP_CharacterPaul::CommandMoveLateralDownLoop );
	SetSelfReLinkTree ( { 0,downkey, 0, 0, downkey } );
	this->AddCommandBaseTree ( { 0,downkey,0, 0, downkey } , 0 , 0 , 3 , &ACPP_CharacterPaul::CommandEnd );

	// Crouch
	this->AddCommandBaseTree ( { 0 } , downkey , 0 , 0 , &ACPP_CharacterPaul::CommandStar );
	this->AddCommandBaseTree ( { 0 ,downkey } , downkey , 4 , 6 , &ACPP_CharacterPaul::CommandDownCrouch );

	// While Crouch
	this->AddCommandBaseTree ( { 0 ,downkey, downkey } , downkey , 1 , 2 , &ACPP_CharacterPaul::CommandDownCrouch );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey } , 0 , 1 , 2 , &ACPP_CharacterPaul::CommandEnd );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey } , 0 , 1 , 2 , &ACPP_CharacterPaul::CommandEnd );
	SetSelfReLinkTree ( { 0, downkey, downkey, downkey } );

	// Skell Couch
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey } , downLPkey , 1 , 2 , &ACPP_CharacterPaul::CommandSitJab );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, downLPkey } , 0 , 1 , 2 , &ACPP_CharacterPaul::CommandUpCrouch );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, downLPkey } , downLPkey , 1 , 2 , &ACPP_CharacterPaul::CommandDownCrouch );
	SetSelfReLinkTree ( { 0 ,downkey, downkey, downkey, downLPkey, downLPkey } );
	this->SetLinkTree ( { 0, downkey, downkey, downkey, downLPkey, downLPkey } , { 0, downkey, downkey } );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, downLPkey, downLPkey } , 0 , 1 , 2 , &ACPP_CharacterPaul::CommandEnd );


	//this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, downLPkey, downLKkey } , LP , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandEnd );

 	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey } , downLKkey , 1 , 2 , &ACPP_CharacterPaul::CommandSitSpineKick );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, downLKkey } , 0 , 1 , 2 , & ACPP_CharacterPaul::CommandUpCrouch );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, downLKkey } , downLKkey , 1 , 2 , & ACPP_CharacterPaul::CommandDownCrouch );
	SetSelfReLinkTree ( { 0 ,downkey, downkey, downkey, downLKkey, downLKkey } );
	this->SetLinkTree ( { 0, downkey, downkey, downkey, downLKkey, downLKkey } , { 0, downkey, downkey } );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, downLKkey, downLKkey } , 0 , 1 , 2 , & ACPP_CharacterPaul::CommandEnd );

// 	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, downLKkey } , downLKkey , 0 , 0  , &ACPP_CharacterPaul::CommandDownCrouch );
// 	SetSelfReLinkTree ( { 0,downkey, downkey, downkey, downLKkey, downLKkey } );
	//this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, downLKkey, downLKkey } , LK , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandEnd );


	// Branch Add function 만들기 하나의 가지로 합쳐주는 함수이다.
	// 현재 겹치는 부분 함수들 목록
	// 짧게
	this->AddCommandBaseTree ( { 0 ,downkey, downkey } , 0 , 0 , 3 , &ACPP_CharacterPaul::CommandUpCrouch );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey,  0 } , 0 , 4 , 6 , &ACPP_CharacterPaul::CommandStar );
	SetSelfReLinkTree ( { 0,downkey, downkey, 0, 0 } );
	// 길게
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey } , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandUpCrouch );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, 0 } , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandStar );
	SetSelfReLinkTree ( { 0,downkey, downkey, downkey, 0, 0 } );
	// 서면서 때리기
	this->AddCommandBaseTree ( { 0,downkey, downkey, 0, 0} , LP , 0 , 0 , &ACPP_CharacterPaul::CommandJinJee );
	//this->AddCommandBaseTree ( { 0,downkey, downkey, 0, 0, LP } , LP , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandEnd );
	//SetSelfReLinkTree ( { 0,downkey, downkey, 0, 0, LP , LP} );
	this->AddCommandBaseTree ( { 0,downkey, downkey, downkey, 0, 0 } , LP , 0 , 0 , & ACPP_CharacterPaul::CommandJinJee );
	//this->AddCommandBaseTree ( { 0,downkey, downkey, downkey, 0, 0, LP } , LP , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandEnd );
	//SetSelfReLinkTree ( { 0,downkey, downkey, downkey, 0, 0, LP , LP } );

	// 붕권
// 	this->AddCommandBaseTree ( { 0, downkey } , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandStarWaitNext );
// 	this->AddCommandBaseTree ( { 0, downkey } , i3key , 0 , 0 , &ACPP_CharacterPaul::CommandStarWaitNext );
// 	this->AddCommandBaseTree ( { 0, downkey, i3key } , forwardkey , 0 , 0 , &ACPP_CharacterPaul::CommandStarTest2 );

	/**
	 * Punch
	 */
	// LeftRightCombo
	// LeftRight 1 
	this->AddCommandBaseTree ( { 0 } , LP , 0 , 3 , & ACPP_CharacterPaul::CommandLeadJab );
	this->AddCommandBaseTree ( { 0, LP } , 0 , 3 , 5 , & ACPP_CharacterPaul::CommandEnd );
	this->AddCommandBaseTree ( { 0, LP } , LP , 3 , 5 , & ACPP_CharacterPaul::CommandStarTest2 );
	this->AddCommandBaseTree ( { 0, LP, LP } , 0 , 0 , 3 , & ACPP_CharacterPaul::CommandEnd );
	this->AddCommandBaseTree ( { 0, LP, LP } , LP , 0 , 3 , & ACPP_CharacterPaul::CommandStarTest2 );
	this->AddCommandBaseTree ( { 0, LP, LP, LP } , 0 , 0 , 3 , & ACPP_CharacterPaul::CommandEnd );
	SetSelfReLinkTree ( { 0, LP, LP, LP} );

	this->AddCommandBaseTree ( { 0, LP } , RP , 3 , 5 , &ACPP_CharacterPaul::CommandCrossStaight );
	//this->AddCommandBaseTree ( { 0, LP, RP } , RP , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandEnd );
	//SetSelfReLinkTree ( { 0, LP, RP, RP } );

	this->AddCommandBaseTree ( { 0 } , RP , 0 , 3 , & ACPP_CharacterPaul::CommandCrossStaight );
	this->AddCommandBaseTree ( { 0, RP } , 0 , 3 , 5 , & ACPP_CharacterPaul::CommandEnd );
	this->AddCommandBaseTree ( { 0, RP } , RP , 3 , 5 , & ACPP_CharacterPaul::CommandStarTest2 );
	this->AddCommandBaseTree ( { 0, RP, RP } , 0 , 0 , 3 , & ACPP_CharacterPaul::CommandEnd );
	this->AddCommandBaseTree ( { 0, RP, RP } , RP , 0 , 3 , & ACPP_CharacterPaul::CommandStarTest2 );
	this->AddCommandBaseTree ( { 0, RP, RP, RP } , 0 , 0 , 3 , & ACPP_CharacterPaul::CommandEnd );
	SetSelfReLinkTree ( { 0, RP, RP, RP } );
	

	/**
	 * Kick
	 */

	this->AddCommandBaseTree ( { 0 } , LK , 0 , 3 , &ACPP_CharacterPaul::CommandJingun );
	this->AddCommandBaseTree ( { 0, LK } , LK , 3 , 5 , &ACPP_CharacterPaul::CommandStarTest2 );
	this->AddCommandBaseTree ( { 0, LK, LK } , 0 , 0 , 3 , & ACPP_CharacterPaul::CommandEnd );
	this->AddCommandBaseTree ( { 0, LK, LK } , LK , 3 , 5 , & ACPP_CharacterPaul::CommandStarTest2 );
	this->AddCommandBaseTree ( { 0, LK, LK, LK } , 0 , 0 , 3 , & ACPP_CharacterPaul::CommandEnd );
	SetSelfReLinkTree ( { 0, LK, LK, LK } );

	this->AddCommandBaseTree ( { 0 } , RK , 0 , 0 , &ACPP_CharacterPaul::CommandHighKick );
	this->AddCommandBaseTree ( { 0, RK } , RK , 0 , 0 , &ACPP_CharacterPaul::CommandStarTest2 );
	this->AddCommandBaseTree ( { 0, RK, RK } , 0 , 0 , 3 , & ACPP_CharacterPaul::CommandEnd );
	this->AddCommandBaseTree ( { 0, RK, RK } , RK , 3 , 5 , & ACPP_CharacterPaul::CommandStarTest2 );
	this->AddCommandBaseTree ( { 0, RK, RK, RK } , 0 , 0 , 3 , & ACPP_CharacterPaul::CommandEnd );
	SetSelfReLinkTree ( { 0, RK, RK, RK } );

//	this->AddCommandBaseTree ( { 0, LP } , 0 , 0 ,  0 , &ACPP_CharacterPaul::CommandStar );
// 	SetSelfReLinkTree ( { 0, LP, 0 } );
// 	// LeftRight 2
// 	this->AddCommandBaseTree ( { 0, LP, 0 } , RP , 2 , 4 , 0 , &ACPP_CharacterPaul::CommandStar );

	this->mCurrCommandTree = mBaseCommandTree;
	this->sCurrCommand = mBaseCommandTree[0];
}


FCommandTree* ACPP_CharacterPaul::CreateCommandTree (int32 keyValue, int32 timingStart , int32 timingEnd ,  void (ACPP_CharacterPaul::* fptr)() )
{
	FCommandTree* NewCommand = new FCommandTree ( );

	NewCommand->keyValue = keyValue;
	//NewCommand->PrevTrees;
	NewCommand->NextTrees = TMap<int32 , FCommandTree*> ( );
	NewCommand->timingStart = timingStart;
	NewCommand->timingEnd = timingEnd;
	NewCommand->action.BindUObject ( this , fptr );

	return NewCommand;
}

FCommandTree* ACPP_CharacterPaul::AddCommandTree ( TMap<int32 , FCommandTree*>& CurrCommandTree , int32 keyValue , int32 timingStart , int32 timingEnd ,  void(ACPP_CharacterPaul::* fptr)() )
{
	//  음 빈거 확인해야 하는데 잘안됨 확인할 필요있음
	if ( &CurrCommandTree && CurrCommandTree.Find ( keyValue ) )
	{
		if ( DebugMode )
			UE_LOG ( LogTemp , Warning , TEXT ( "Error: befor add CommandTree was settinged!! %i" ) , keyValue );

		return CurrCommandTree[keyValue];
	}
	CurrCommandTree.Add ( keyValue , this->CreateCommandTree ( keyValue, timingStart , timingEnd , fptr ) );

	CurrCommandTree[keyValue]->PrevKeys = {keyValue};
	CurrCommandTree[keyValue]->NextKeys.Add ( keyValue );
	return CurrCommandTree[keyValue];
}

FCommandTree* ACPP_CharacterPaul::AddCommandBaseTree ( TArray<int> arrayTreeCommand , int32 keyValue , int32 timingStart , int32 timingEnd ,  void(ACPP_CharacterPaul::* fptr)() )
{
	TMap<int32 , FCommandTree*> temp = mBaseCommandTree;
	TMap<int32 , FCommandTree*> tempParant = mBaseCommandTree;
	int lastkey = 0;

	for ( int commandkey : arrayTreeCommand )
	{
		tempParant = temp;
		if ( !temp.Find ( commandkey ) )
		{
			if ( DebugMode )
				UE_LOG ( LogTemp , Error , TEXT ( "[SetParentReLinkTree] No Tree Command " ) );
			return nullptr;
		}
		temp = temp[commandkey]->NextTrees;
		lastkey = commandkey;
	}
	if ( tempParant[lastkey]->NextTrees.Find ( keyValue ) )
	{
		if ( DebugMode )
			UE_LOG ( LogTemp , Warning , TEXT ( "Error: befor add CommandTree was settinged!! %i" ) , keyValue );
		return tempParant[lastkey]->NextTrees[lastkey];
	}
	tempParant[lastkey]->PrevKeys = arrayTreeCommand;
	tempParant[lastkey]->NextKeys.Add(keyValue);
	tempParant[lastkey]->NextTrees.Add ( keyValue , this->CreateCommandTree ( keyValue , timingStart , timingEnd , fptr ) );
	return tempParant[lastkey]->NextTrees[keyValue];
}


bool ACPP_CharacterPaul::SetSelfReLinkTree ( TArray<int32> arrayTreeCommand )
{
	TMap<int32 , FCommandTree*> temp = mBaseCommandTree;
	TMap<int32 , FCommandTree*> tempParant = mBaseCommandTree;
	int lastkey = 0;
	for ( int commandkey : arrayTreeCommand )
	{
		tempParant = temp;
		if ( !temp.Find ( commandkey ) )
		{
			if ( DebugMode )
				UE_LOG ( LogTemp , Error , TEXT ( "[SetParentReLinkTree] No Tree Command " ) );
			return false;

		}
		temp = temp[commandkey]->NextTrees;
		lastkey = commandkey;
	}
	tempParant[lastkey]->PrevKeys.Append(arrayTreeCommand);
	tempParant[lastkey]->NextKeys.Add ( lastkey );
	tempParant[lastkey]->NextTrees.Add ( lastkey , tempParant[lastkey] );
	return true;
}

bool ACPP_CharacterPaul::SetLinkTree ( TArray<int32> TargetTree , TArray<int32> ConnectTree )
{
	FCommandTree* tempTarget;
	FCommandTree* tempConnect;
	FCommandTree* ConnectParant;
	int32 ConnectLastKey;

	tempTarget = mBaseCommandTree[0];
	for (int32 commandkey : TargetTree)
	{
		tempTarget = tempTarget->NextTrees[commandkey];
	}

	tempConnect = mBaseCommandTree[0];
	for ( int32 commandkey : ConnectTree )
	{
		ConnectParant = tempConnect;
		tempConnect = tempConnect->NextTrees[commandkey];
		ConnectLastKey = commandkey;
	}
	tempTarget->NextTrees.Add( ConnectLastKey ,ConnectParant);

// 	TMap<int32 , FCommandTree*> temp = mBaseCommandTree;
// 	TMap<int32 , FCommandTree*> targetParant = mBaseCommandTree;
// 	TMap<int32 , FCommandTree*> tempConnect = mBaseCommandTree;
// 	TMap<int32 , FCommandTree*> ConnectParant = mBaseCommandTree;
// 	int targetLastkey = 0;
// 	int connectLastKey = 0;
// 	for ( int commandkey : TargetTree )
// 	{
// 		targetParant = temp;
// 		if ( !temp.Find ( commandkey ) )
// 		{
// 			if ( DebugMode )
// 				UE_LOG ( LogTemp , Error , TEXT ( "[SetLinkTree] No Tree Command " ) );
// 			return false;
// 		}
// 		temp = temp[commandkey]->NextTrees;
// 		targetLastkey = commandkey;
// 	}
// 	for ( int commandkey : ConnectTree )
// 	{
// 		ConnectParant = tempConnect;
// 		if ( !tempConnect.Find ( commandkey ) )
// 		{
// 			if ( DebugMode )
// 				UE_LOG ( LogTemp , Error , TEXT ( "[SetLinkTree] No Tree Command " ) );
// 			return false;
// 		}
// 		tempConnect = tempConnect[commandkey]->NextTrees;
// 		connectLastKey = commandkey;
// 	}
// 	temp.
// 	targetParant[targetLastkey]->NextKeys.Add ( connectLastKey );
// 	targetParant[targetLastkey]->NextTrees.Add ( connectLastKey , ConnectParant[connectLastKey] );
	return true;
}

/************************************************************************/
/*                커멘드 함수에 대한 정보                               */
/************************************************************************/

// Command Test 
void ACPP_CharacterPaul::CommandStarWaitNext ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandStarWaitNext Pressed" ) );

}
void ACPP_CharacterPaul::CommandStarTest2 ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandStarTest2 Pressed" ) );
	this->eCharacterState = ECharacterStateInteraction::GuardStand;
}

// Command Function

void ACPP_CharacterPaul::CommandIdle ( )
{
//   	if ( DebugMode )
//   		UE_LOG ( LogTemp , Warning , TEXT ( "CommandIdle Pressed" ) );

	this->bCrouched =false;
	//this->uCharacterMesh->SetRelativeScale3D ( FVector ( 1 , 1 , 1 ) );
	// this->uCharacterMesh->SetRelativeLocation ( FVector ( 0 , 0 , -90 ) );

	this->eCharacterState = ECharacterStateInteraction::GuardStand;
	if ( this->CountIdleFrame > 3 )
	{
		this->CountIdleFrame = 0;
		if ( DebugMode )
			UE_LOG ( LogTemp , Warning , TEXT ( "Clean Command Idle" ) );

		this->bCrouched = false;
		this->bJumpping = false;
		this->sCurrCommand = mBaseCommandTree[0];
		return;
	}
	CountIdleFrame++;
	CountStarFrame = 3;
	this->iCurrFrame = 0;
	this->sFrameStatus.FrameUsing = 0;
}

void ACPP_CharacterPaul::CommandStar ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandStar Pressed" ) );

	if ( this->CountStarFrame <= 0 )
	{
		this->CountStarFrame = 3;
		if ( DebugMode )
			UE_LOG ( LogTemp , Warning , TEXT ( "Star Clean Command" ) );

		this->sCurrCommand = mBaseCommandTree[0];
		this->sNextCommand = nullptr;
		this->currKeyValue = this->sCurrCommand->keyValue;
		return;
	}
	CountStarFrame--;
}

void ACPP_CharacterPaul::CommandEnd ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandEnd Pressed" ) );
	this->bCrouched = false;
	this->sCurrCommand = mBaseCommandTree[0];
	this->sNextCommand = nullptr;
	this->currKeyValue = this->sCurrCommand->keyValue;
	this->iCurrFrame = 0;
}


void ACPP_CharacterPaul::CommandMoveForward ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveForward Pressed" ) );

	eCharacterState = ECharacterStateInteraction::WalkForward;

	// 애니메이션으로 바꾼다면?
	this->SetToLocationFrame(FVector(0.5,0,0), 2);

	this->CountStarFrame = 5;
	this->sFrameStatus.FrameUsing = 1;
	this->iCurrFrame = 0;
}

void ACPP_CharacterPaul::CommandMoveForwarDash ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "MoveForwarDash Pressed" ) );

	eCharacterState = ECharacterStateInteraction::Run;

	this->SetToLocationFrame ( FVector ( 20 , 0 , 0 ) , 2 );
	this->sFrameStatus.FrameUsing = 1;
	this->iCurrFrame = 0;
}

void ACPP_CharacterPaul::CommandMoveBack ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveBack Pressed" ) );

	eCharacterState = ECharacterStateInteraction::GuardStand;

	this->SetToLocationFrame ( FVector ( -0.5 , 0 , 0 ) , 2 );
	this->CountStarFrame = 5;
	this->sFrameStatus.FrameUsing = 1;
	this->iCurrFrame = 0;
}

void ACPP_CharacterPaul::CommandMoveBackDash ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveBackDash Pressed" ) );

	eCharacterState = ECharacterStateInteraction::Move;

	this->SetToLocationFrame ( FVector ( -500 , 0 , 0 ) , 15 );
	this->sFrameStatus.FrameUsing = 20;
	this->iCurrFrame = 0;
}

void ACPP_CharacterPaul::CommandJump ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJump Pressed" ) );
	
	if ( this->bJumpping )
		return;
	eCharacterState = ECharacterStateInteraction::Air;
	LaunchCharacter ( FVector ( 0, 0, 400) , true , true );

	this->bJumpping = true;
	this->sFrameStatus.FrameUsing = 56;
	this->CountIdleFrame = 5;
	this->iCurrFrame = 0;
}

void ACPP_CharacterPaul::CommandMoveLateralUpDash ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveLateralPlus Pressed" ) );

	eCharacterState = ECharacterStateInteraction::Move;

	sAttackInfo.DamagePoint = EDamagePointInteraction::Special;
	sAttackInfo.DamageAmount = 0;

	this->sAttackInfo.ActionFrame = 1;

	this->SetToLocationFrame (FVector( 300 , -500 , 0 ), 20);

	// 애니매이션 실행 부분 있으면 만들기
	//PlayMontageFrameSystem ( uMtgMoveLateral );

	CountStarFrame = 5;

	this->sFrameStatus.FrameUsing = 25;
	this->iCurrFrame = 0;
}

void ACPP_CharacterPaul::CommandMoveLateralUpLoop( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveLateralUpLoop Pressed" ) );

	eCharacterState = ECharacterStateInteraction::Move;

	sAttackInfo.DamagePoint = EDamagePointInteraction::Special;
	sAttackInfo.DamageAmount = 0;

	this->SetToLocationFrame ( FVector ( 30 , -250 , 0 ) ,5 );

	// 애니매이션 실행 부분 없음

	this->sFrameStatus.FrameUsing = 5;
	this->iCurrFrame = 0;
}

void ACPP_CharacterPaul::CommandDownCrouch ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandDownCrouch Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::GuardSit;
	this->bCrouched = true;
	//this->GetCapsuleComponent ( )->SetWorldScale3D ( FVector ( 0.1 , 0.6 , 0.1 ) );
	//this->uCharacterMesh->SetRelativeLocation ( FVector ( 0 , 0 , -130 ) );
	this->sFrameStatus.FrameUsing = 10;
}

// 사용하지 않는다
void ACPP_CharacterPaul::CommandUpCrouch ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandUpCrouch Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::Up;

	CountStarFrame = 10;
	this->sFrameStatus.FrameUsing = 1;
}

void ACPP_CharacterPaul::CommandMoveLateralDownDash ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveLateralDown Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::Move;

	sAttackInfo.DamagePoint = EDamagePointInteraction::Special;
	sAttackInfo.DamageAmount = 0;

	this->sAttackInfo.ActionFrame = 1;

	this->SetToLocationFrame ( FVector ( 300 , 500 , 0 ) , 20 );

	// 애니매이션 실행 부분 있으면 만들기
	//PlayMontageFrameSystem ( uMtgMoveLateral );

	CountStarFrame = 5;

	this->sFrameStatus.FrameUsing = 25;
	this->iCurrFrame = 0;
}
void ACPP_CharacterPaul::CommandMoveLateralDownLoop ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveLateralDown Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::Move;

	sAttackInfo.DamagePoint = EDamagePointInteraction::Special;
	sAttackInfo.DamageAmount = 0;

	this->SetToLocationFrame ( FVector ( 30 , 250 , 0 ) , 5 );

	// 애니매이션 실행 부분 없음

	this->sFrameStatus.FrameUsing = 5;
	this->iCurrFrame = 0;
}

void ACPP_CharacterPaul::CommandLeadJab ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandLeadJab Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::AttackTop;

	SetActtacInfoSkell ( EDamagePointInteraction::Top , 5 , 10 , 20 , 0 , 8 , 1 , 8 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 90 , 5 , 50 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 300 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수
	
	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	sAttackInfo.debugColor = FColor ( 255 , 128 , 128 );
	// 추후 애니메이션 있을 시 애니메이션 이동으로 변경
	this->SetToLocationFrame( FVector(30, 0, 0) , 5);	

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgLeadJab );
	this->sNextCommand = this->mBaseCommandTree[0]->NextTrees[512]->NextTrees[512];


	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandCrossStaight ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandCrossStaight Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::AttackTop;

	SetActtacInfoSkell ( EDamagePointInteraction::Top , 5 , 10 , 20 , 0 , 6 , 0 , 6 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 90 , -5 , 60 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 160 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToLocationFrame ( FVector ( 20 , 0 , 0 ) , 5 );


	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( this->uMtgCrossStaight );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
	this->sNextCommand = this->mBaseCommandTree[0]->NextTrees[1024]->NextTrees[1024];
	
}

void ACPP_CharacterPaul::CommandJingun ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJingun Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;

	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 14 , 15 , 20 , 0 , -1 , -12 , -1 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 100 , 5 , 00 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 150 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToLocationPoint ( 10 , 0 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgJingun );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
	this->sNextCommand = this->mBaseCommandTree[0]->NextTrees[2048]->NextTrees[2048];
}

void ACPP_CharacterPaul::CommandHighKick ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJingun Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::AttackTop;

	SetActtacInfoSkell ( EDamagePointInteraction::Top , 17 , 12 , 20 , 0 , 14 , 4 , 57 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 120 , -5 , 60 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 150 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToLocationPoint ( 5 , 0 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgHighKick );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
	this->sNextCommand = this->mBaseCommandTree[0]->NextTrees[4096]->NextTrees[4096];
}

void ACPP_CharacterPaul::CommandBungGuan ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandBungGuan Pressed" ) );

	this->bCrouched = false;
	SetActtacInfoSkell ( EDamagePointInteraction::Top , 17 , 12 , 20 , 0 , 14 , 4 , 57 );

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;
	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 17 , 20, 20 , 0 ,0 , 0 ,0 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 200 , -5 , 60 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 500 , 0 , 20 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToLocationPoint ( 30 , 0 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgBungGuan );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandJinJee ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJinJee Pressed" ) );

	this->bCrouched = false;

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;

	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 20 , 5 , 12 , 0 , 15 , -14 , 0 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 50 , -5 , 10 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 10 , 0 , 300 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToLocationPoint ( 10 , 0 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgLeadJab );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandSitJab( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandSitJab Pressed" ) );

	this->bCrouched = false;

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;

	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 15 , 4 , 8 , 0 , 10 , -5 , 6 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 80 , -5 , 10 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 120 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToLocationPoint ( 10 , 0 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgSitJab );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandSitSpineKick ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandSitJab Pressed" ) );

	this->bCrouched = false;

	this->eCharacterState = ECharacterStateInteraction::AttackLower;

	SetActtacInfoSkell ( EDamagePointInteraction::Lower , 15 , 12 , 8 , 0 , 10 , -5 , 6 );


	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 120 , -5 , -40 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 140 , -5 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToLocationPoint ( 0 , 0 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgSitSpineKick );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

bool ACPP_CharacterPaul::CommandAllStop ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandAllStop Pressed" ) );
	
	this->eCharacterState = ECharacterStateInteraction::Idle;

	this->mCurrCommandTree = mBaseCommandTree[0]->NextTrees;
	this->sCurrCommand = mBaseCommandTree[0];
	this->sNextCommand = nullptr;

	sAttackInfo.ActionFrame = -1;
	this->sFrameStatus.FrameUsing = 1;
	return 0;
}

/************************************************************************/
/*               맞는 것을 정의한 공간 입니다                           */
/************************************************************************/
bool ACPP_CharacterPaul::HitDecision ( FAttackInfoInteraction attackInfoHit , ACPP_Tekken8CharacterParent* ownerHitPlayer )
{
	bMoveTo = false;

	if ( aMainCamera )
		aMainCamera->RequestZoomEffect ( attackInfoHit.skellEffectLocation , attackInfoHit.cameraZoom , attackInfoHit.cameraShake , attackInfoHit.cameraDelay );
	else
		UE_LOG(LogTemp, Warning, TEXT("is Emtyp ") );
	if ( attackInfoHit.DamagePoint == EDamagePointInteraction::Top && this->eCharacterState == ECharacterStateInteraction::GuardStand )
	{
		this->sFrameStatus.FrameBlockUsing = attackInfoHit.OppositeGuardFrame;
		//this->SetToWorldLocationPoint ( (attackInfoHit.KnockBackDirection - this->GetActorLocation ( )) / 2 + this->GetActorLocation ( ));
		LaunchCharacter( (attackInfoHit.KnockBackDirection - this->GetActorLocation ( )) / 2 + this->GetActorLocation ( ), true, true );
		// defense animation 추가하기
		PlayMontageFrameSystem ( uMtgDefence );
		// 디펜스 파티클

		if ( uNS_DefenceEffect && this->Hp > 0 )
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), uNS_DefenceEffect, attackInfoHit.skellEffectLocation );
		return false;
	}
	if ( attackInfoHit.DamagePoint == EDamagePointInteraction::Middle && this->eCharacterState == ECharacterStateInteraction::GuardStand )
	{
		this->sFrameStatus.FrameBlockUsing = attackInfoHit.OppositeGuardFrame;
		//this->SetToWorldLocationPoint ( (attackInfoHit.KnockBackDirection - this->GetActorLocation ( )) / 2 + this->GetActorLocation ( ) );
		LaunchCharacter ( (attackInfoHit.KnockBackDirection - this->GetActorLocation ( )) / 2 + this->GetActorLocation ( ) , true , true );
		// defense animation 추가하기
		if (this->bCrouched )
			PlayMontageFrameSystem ( uMtgSitDefence );
		else
			PlayMontageFrameSystem ( uMtgDefence );

		if ( uNS_DefenceEffect && this->Hp > 0 )
			UNiagaraFunctionLibrary::SpawnSystemAtLocation ( GetWorld ( ) , uNS_DefenceEffect , attackInfoHit.skellEffectLocation );

		return false;
	}
	if ( attackInfoHit.DamagePoint == EDamagePointInteraction::Lower && this->eCharacterState == ECharacterStateInteraction::GuardSit )
	{
		this->sFrameStatus.FrameBlockUsing = attackInfoHit.OppositeGuardFrame;
		//this->SetToWorldLocationPoint ( ( attackInfoHit.KnockBackDirection - this->GetActorLocation ( ) ) / 2 + this->GetActorLocation ( ) );
		LaunchCharacter ( (attackInfoHit.KnockBackDirection - this->GetActorLocation ( )) / 2 + this->GetActorLocation ( ) , true , true );
		// defense animation 추가하기
		PlayMontageFrameSystem ( uMtgSitDefence );

		if ( uNS_DefenceEffect && this->Hp > 0 )
			UNiagaraFunctionLibrary::SpawnSystemAtLocation ( GetWorld ( ) , uNS_DefenceEffect , attackInfoHit.skellEffectLocation );

		return false;
	}
	this->SetActorRotation ( UKismetMathLibrary::FindLookAtRotation ( this->GetActorLocation ( ) , this->aOpponentPlayer->GetActorLocation ( ) ) );
	this->sFrameStatus.FrameBlockUsing = attackInfoHit.OppositeHitFrame;
	//this->SetToWorldLocationPoint ( attackInfoHit.KnockBackDirection );
	LaunchCharacter ( attackInfoHit.KnockBackDirection , true , true );
	// heart animation 추가하기
	if ( this->Hp > 0 )
	{
		if ( attackInfoHit.DamagePoint == EDamagePointInteraction::Lower  )
			PlayMontageFrameSystem ( uMtgSitHit );
		else
			PlayMontageFrameSystem ( uMtgIdleHit );
	}
	// UI hit newHp 전달하기
	this->Hp -= attackInfoHit.DamageAmount;
	this->GameModeMH->UpdatePlayerHP(this,this->Hp);

	if (this->Hp < 0 )
		this->bFalling = true;
	// 좀 있다 이동 시키기
	this->eCharacterState = ECharacterStateInteraction::HitGround;
	//camera 효과 추가하기s

	if ( uNS_DefenceEffect )
		UNiagaraFunctionLibrary::SpawnSystemAtLocation ( GetWorld ( ) , uNS_HitEffect , attackInfoHit.skellEffectLocation );

	this->CommandAllStop();
	return true;
}

void  ACPP_CharacterPaul::CommentHitFrameExecute ( )
{
	float radius = 20.0f;
	// 스페셜 스킬 이동기 등
	if ( sAttackInfo.DamagePoint == EDamagePointInteraction::Special )
		return;

	// 맞는 부분 실행
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add ( UEngineTypes::ConvertToObjectType ( ECollisionChannel::ECC_Pawn ) );
	TArray<AActor*> ignoreActors;
	ignoreActors.Init ( this , 1 );
	TArray<AActor*> outActors;
	FVector sphereSpawnLocation = sAttackInfo.skellEffectLocation;
	UClass* seekClass = ACPP_Tekken8CharacterParent::StaticClass ( );

	// 맞은거 판별
	bool hit = UKismetSystemLibrary::SphereOverlapActors ( GetWorld ( ) , sphereSpawnLocation , radius , traceObjectTypes , seekClass , ignoreActors , outActors );;
	if ( hit )
	{
		for ( AActor* hitActor : outActors )
		{
			if ( hitActor->IsA<ACPP_Tekken8CharacterParent> ( ) )
			{
				ACPP_Tekken8CharacterParent* hitCharacter = Cast<ACPP_Tekken8CharacterParent> ( hitActor );
				//때려보기
				if ( hitCharacter->HitDecision ( sAttackInfo , this ) )
					sFrameStatus.FrameBlockUsing = sAttackInfo.OwnerHitFrame;
				else
					sFrameStatus.FrameBlockUsing = sAttackInfo.OwnerGuardFrame;
			}
		}
	}
	else
	{
		// 아무도 안맞았을때
		sFrameStatus.FrameBlockUsing = sAttackInfo.OwnerMissFrame;
	}

	if ( sAttackInfo.DamagePoint == EDamagePointInteraction::Top )
		sAttackInfo.debugColor = FColor ( 255 , 0 , 0 );
	else if ( sAttackInfo.DamagePoint == EDamagePointInteraction::Middle )
		sAttackInfo.debugColor = FColor ( 255 , 125 , 0 );
	else if ( sAttackInfo.DamagePoint == EDamagePointInteraction::Lower )
		sAttackInfo.debugColor = FColor ( 0 , 0 , 255 );

	if (DebugMode )
		DrawDebugSphere ( GetWorld ( ) , sAttackInfo.skellEffectLocation , radius , 26 , sAttackInfo.debugColor , false , 1.0f );
}



// void ACPP_CharacterPaul::HowtoUseSphereOverlapActors ( )
// {
// 	/**
// 	 * @use SphereOverlapActors
// 	 * @url https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/Kismet/UKismetSystemLibrary/SphereOverlapActors?application_version=5.4
// 	 */
// 	 // Set what actors to seek out from it's collision channel
// 	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
// 	traceObjectTypes.Add ( UEngineTypes::ConvertToObjectType ( ECollisionChannel::ECC_Pawn ) );
// 
// 	// Ignore any specific actors
// 	TArray<AActor*> ignoreActors;
// 	// Ignore self or remove this line to not ignore any
// 	ignoreActors.Init ( this , 1 );
// 
// 	// Array of actors that are inside the radius of the sphere
// 	TArray<AActor*> outActors;
// 
// 	// Total radius of the sphere
// 	float radius = 750.0f;
// 	// Sphere's spawn loccation within the world
// 	FVector sphereSpawnLocation = GetActorLocation ( );
// 	// Class that the sphere should hit against and include in the outActors array (Can be null)
// 	UClass* seekClass = ACharacter::StaticClass ( ); // NULL;
// 	UKismetSystemLibrary::SphereOverlapActors ( GetWorld ( ) , sphereSpawnLocation , radius , traceObjectTypes , seekClass , ignoreActors , outActors );
// 
// }

/*
* @ tamplelate skell
void ACPP_CharacterPaul::CommandLeadJab ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandLeadJab Pressed" ) );

	attackInfo.DamagePoint = EDamagePointInteraction::Top;
	attackInfo.DamageAmount = 0;

	attackInfo.ActionFrame = 0;
	attackInfo.RetrieveFrame = 0;

	attackInfo.MissFrame = 0;
	attackInfo.HitFrame = 0;
	attackInfo.GrardFrame = 0;
	attackInfo.CounterFrame = 0;

	attackInfo.skellEffectLocation = this->RelativePointVector ( 0 , 0 , 0 );
	attackInfo.KnockBackDirection = this->RelativePointVector ( 0 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	attackInfo.cameraShake = 0;
	attackInfo.cameraZoom = 0;
	attackInfo.cameraDelay = 0;

	attackInfo.debugColor = FColor ( 0 , 0 , 0 );
	this->SetToLocationPoint ( 0 , 0 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtg );

	this->sFrameStatus.FrameUsing = attackInfo.ActionFrame;
}
*/