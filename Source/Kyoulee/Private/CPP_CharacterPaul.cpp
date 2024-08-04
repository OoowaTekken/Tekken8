// Fill out your copyright notice in the Description page of Project Settings.
#define MYDEBUGMODE 0;
#define MYDEBUGMODEHIT  1;
#include "CPP_CharacterPaul.h"

#include "GameMode_MH.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameMode_MH.h"

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


// Called every frame
void ACPP_CharacterPaul::Tick ( float DeltaTime )
{
	Super::Tick ( DeltaTime );

	// 주의 모든 작업은 FrameSystem에서 하는걸 권장합니다.
	this->fCurrTimeForFrame += DeltaTime;

	if ( this->fCurrTimeForFrame > this->fFrameTime )
	{
		this->currKeyValue = this->GetCurrInputKeyValue ( );
		this->fCurrTimeForFrame = 0;
		sFrameStatus.FrameBlockUsing--;
		sFrameStatus.FrameUsing--;
		attackInfo.ActionFrame--;
		if ( attackInfo.ActionFrame == 0 )
		{
			this->CommentHitFrameExecute ();
		}
		iCurrFrame++;

		AnimationFrame ( );
		if ( this->sFrameStatus.FrameBlockUsing < 0 )
			this->FrameSystem ( );
 	}
}

/**
 * @title Frame 시스템을 위한 함수입니다.
 */
void ACPP_CharacterPaul::FrameSystem ( )
{
	if ( !this->mCurrCommandTree.Find ( currKeyValue ) )
	{
		this->CommandIdle ( );
		return;
	}
	if ( currKeyValue  ==  InputKeyValue(0,true,0,0,0))
		iCurrFrame = 0;
	FCommandTree* temptree = this->mCurrCommandTree[currKeyValue];
	if ( !(temptree->timingStart <= iCurrFrame) )
	{
		this->CommandIdle ( );
		return;
	}
	if ( temptree->timingEnd && iCurrFrame > temptree->timingEnd )
	{
		this->CommandIdle ( );
		return;
	}
	this->CountIdleFrame = 0;
	iCurrFrame = 0;
	//UE_LOG ( LogTemp , Warning , TEXT ( "input : %i " ) , currKeyValue );
	if ( sFrameStatus.FrameUsing <= 0 )
	{
		temptree->action.Execute ( );
		if ( mCurrCommandTree[currKeyValue]->NextTrees.IsEmpty ( ) )
		{
			mCurrCommandTree = mBaseCommandTree[0]->NextTrees;
			sCurrCommand = mBaseCommandTree[0];
		}
		else
		{
			sCurrCommand = mCurrCommandTree[currKeyValue];
			mCurrCommandTree = mCurrCommandTree[currKeyValue]->NextTrees;
		}
	}
}

void ACPP_CharacterPaul::SetAttackInfoOwnerOpposite ( )
{
	attackInfo.OwnerHitFrame = attackInfo.RetrieveFrame + attackInfo.HitFrame < 0 ? attackInfo.HitFrame * -1 : 0;
	attackInfo.OwnerGuardFrame = attackInfo.RetrieveFrame + (attackInfo.GrardFrame < 0 ? attackInfo.GrardFrame * -1 : 0);
	attackInfo.OwnerCounterFrame = attackInfo.RetrieveFrame + attackInfo.CounterFrame < 0 ? attackInfo.CounterFrame * -1 : 0;
	attackInfo.OwnerMissFrame = attackInfo.RetrieveFrame + attackInfo.MissFrame < 0 ? attackInfo.MissFrame * -1 : 0;

	attackInfo.OppositeHitFrame = attackInfo.RetrieveFrame + attackInfo.HitFrame > 0 ? attackInfo.HitFrame : 0;
	attackInfo.OppositeGuardFrame = attackInfo.RetrieveFrame + attackInfo.GrardFrame > 0 ? attackInfo.GrardFrame : 0;
	attackInfo.OppositeCounterFrame = attackInfo.RetrieveFrame + attackInfo.CounterFrame > 0 ? attackInfo.CounterFrame : 0;
}

bool ACPP_CharacterPaul::PlayMontageFrameSystem ( UAnimMontage* montage )
{
	if ( !uAnim || !montage )
		return false;
	UE_LOG(LogTemp,Warning,TEXT("Montage Start" ));
	uAnim->Montage_Play ( montage, this->attackInfo.ActionFrame );

	return true;
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

void ACPP_CharacterPaul::AnimationFrame ( )
{
	
	FVector dir = (this->ToLocation - this->GetActorLocation ( )) / 60;
	AddMovementInput(dir);
	// this->SetActorLocation ( this->GetActorLocation ( ) + dir );

	// 인풋이 있을 경우 상대를 바라본다 
	if ( currKeyValue )
		this->SetActorRotation ( UKismetMathLibrary::FindLookAtRotation ( this->GetActorLocation ( ) , this->aOpponentPlayer->GetActorLocation ( ) ) );
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

/**
 * @title 인풋처리를 위한 함수 입니다.
 */


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

// Command Setting Function

void ACPP_CharacterPaul::SettingCommandTree ( )
{
	this->AddCommandTree ( mBaseCommandTree , 0 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandIdle );
	SetSelfReLinkTree ( { 0 } );
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
	/*                          MoveMent                                    */
	/************************************************************************/
	/**
	 *  ForwardKey
	 */
	this->AddCommandBaseTree ( { 0 } , forwardkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandMoveForward );
	SetSelfReLinkTree ( { 0,forwardkey } );
	// MoveForward Dash
	this->AddCommandBaseTree ( { 0, forwardkey } , 0 , 0 , 3 , 0 , &ACPP_CharacterPaul::CommandStar );
	// dash 3프레임후 초기화
	SetSelfReLinkTree ( { 0,forwardkey,0 } );
	// DASH while
	this->AddCommandBaseTree ( { 0, forwardkey, 0 } , forwardkey , 0 , 3 , 0 , &ACPP_CharacterPaul::CommandMoveForwarDash );
	SetSelfReLinkTree ( { 0,forwardkey,0, forwardkey } );

	/**
	 *  BackKey
	 */
	this->AddCommandBaseTree ( { 0 } , backkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandMoveBack );
	SetSelfReLinkTree ( { 0,backkey } );
	// MoveBack Dash
	this->AddCommandBaseTree ( { 0, backkey } , 0 , 0 , 3 , 0 , &ACPP_CharacterPaul::CommandStar );
	// dash 3프레임후 초기화
	SetSelfReLinkTree ( { 0,backkey,0 } );
	// DASH while
	this->AddCommandBaseTree ( { 0, backkey, 0 } , backkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandMoveBackDash );
	//SetSelfReLinkTree ( { 0,backkey, 0 , backkey } );

	/**
	 * UPKey
	 */
	 // Move Lateral Plus
	this->AddCommandBaseTree ( { 0 } , upkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandStar );
	// 3frame wait
	this->AddCommandBaseTree ( { 0, upkey } , 0 , 0 , 5 , 0 , &ACPP_CharacterPaul::CommandMoveLateralUp );
	this->AddCommandBaseTree ( { 0, upkey, 0 } , 0 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandStar );
	SetSelfReLinkTree ( { 0,upkey,0,0 } );
	// Move while Lateral
	this->AddCommandBaseTree ( { 0, upkey, 0, 0 } , upkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandMoveLateralUp );
	SetSelfReLinkTree ( { 0,upkey,0,0, upkey } );

	// Jump
	this->AddCommandBaseTree ( { 0 } , upkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandStar );
	this->AddCommandBaseTree ( { 0 , upkey } , upkey , 4 , 0 , 0 , &ACPP_CharacterPaul::CommandJump );
	this->AddCommandBaseTree ( { 0 , upkey, upkey } , upkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandEnd );
	SetSelfReLinkTree ( { 0,upkey,0,0, upkey, upkey, upkey } );


	/**
	 * DownKey
	 */
	 // Move Lateral Minus
	this->AddCommandBaseTree ( { 0 } , downkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandStar );
	// 3frame wait
	this->AddCommandBaseTree ( { 0 ,downkey } , 0 , 0 , 5 , 0 , &ACPP_CharacterPaul::CommandMoveLateralDown );
	this->AddCommandBaseTree ( { 0 ,downkey, 0 } , 0 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandStar );
	SetSelfReLinkTree ( { 0,downkey, 0, 0 } );
	// Move while Lateral
	this->AddCommandBaseTree ( { 0 ,downkey, 0, 0 } , downkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandMoveLateralDown );
	SetSelfReLinkTree ( { 0,downkey, 0, 0, downkey } );

	// Crouch
	this->AddCommandBaseTree ( { 0 } , downkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandStar );
	this->AddCommandBaseTree ( { 0 ,downkey } , downkey , 3 , 0 , 0 , &ACPP_CharacterPaul::CommandDownCrouch );
	this->AddCommandBaseTree ( { 0 ,downkey } , downkey , 3 , 0 , 0 , &ACPP_CharacterPaul::CommandDownCrouch );

	// While Crouch
	this->AddCommandBaseTree ( { 0 ,downkey, downkey } , downkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandDownCrouch );
	SetSelfReLinkTree ( { 0,downkey, downkey, downkey } );

	// skell Couch
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey } , downLPkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandSitJab );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, downLPkey } , downLPkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandDownCrouch );
	SetSelfReLinkTree ( { 0,downkey, downkey, downkey, downLPkey, downLPkey } );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, downLPkey } , LP , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandEnd );

	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey } , downLKkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandSitSpineKick );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, downLKkey } , downLKkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandDownCrouch );
	SetSelfReLinkTree ( { 0,downkey, downkey, downkey, downLKkey, downLKkey } );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, downLKkey } , LK , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandEnd );


	// Branch Add function 만들기 하나의 가지로 합쳐주는 함수이다.
	// 현재 겹치는 부분 함수들 목록
	// 짧게
	this->AddCommandBaseTree ( { 0 ,downkey, downkey } , 0 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandUpCrouch );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey,  0 } , 0 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandStar );
	SetSelfReLinkTree ( { 0,downkey, downkey, 0, 0 } );
	// 길게
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey } , 0 , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandUpCrouch );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, 0 } , 0 , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandStar );
	SetSelfReLinkTree ( { 0,downkey, downkey, downkey, 0, 0 } );
	// 서면서 때리기
	this->AddCommandBaseTree ( { 0,downkey, downkey, 0, 0} , LP , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandJinJee );
	this->AddCommandBaseTree ( { 0,downkey, downkey, 0, 0, LP } , LP , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandEnd );
	SetSelfReLinkTree ( { 0,downkey, downkey, 0, 0, LP , LP} );
	this->AddCommandBaseTree ( { 0,downkey, downkey, downkey, 0, 0 } , LP , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandJinJee );
	this->AddCommandBaseTree ( { 0,downkey, downkey, downkey, 0, 0, LP } , LP , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandEnd );
	SetSelfReLinkTree ( { 0,downkey, downkey, downkey, 0, 0, LP , LP } );

	// 붕권
// 	this->AddCommandBaseTree ( { 0, downkey } , 0 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandStar );
// 	SetSelfReLinkTree ( { 0,downkey, downkey, downkey, 0 } );
// 	this->AddCommandBaseTree ( { 0, downkey, 0 } , i3key , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandStar );
// 	this->AddCommandBaseTree ( { 0, downkey, 0, i3key } , forwardkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandStar );

	/**
	 * Punch
	 */
	// LeftRightCombo
	// LeftRight 1 
	this->AddCommandBaseTree ( { 0 } , LP , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandLeadJab );
	this->AddCommandBaseTree ( { 0, LP} , LP , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandEnd );
	SetSelfReLinkTree ( {0, LP, LP} );
	this->AddCommandBaseTree ( { 0, LP } , RP , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandCrossStaight );
	this->AddCommandBaseTree ( { 0, LP, RP } , RP , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandEnd );
	SetSelfReLinkTree ( { 0, LP, RP, RP } );

	this->AddCommandBaseTree ( { 0 } , RP , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandCrossStaight );
	this->AddCommandBaseTree ( { 0, RP } , RP , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandEnd );
	SetSelfReLinkTree ( { 0, RP, RP } );
	

	/**
	 * Kick
	 */

	this->AddCommandBaseTree ( { 0 } , LK , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandJingun );
	this->AddCommandBaseTree ( { 0, LK } , LK , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandEnd );
	SetSelfReLinkTree ( { 0, LK, LK } );

	this->AddCommandBaseTree ( { 0 } , RK , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandHighKick );
	this->AddCommandBaseTree ( { 0, RK } , RK , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandEnd );
	SetSelfReLinkTree ( { 0, RK, RK } );

//	this->AddCommandBaseTree ( { 0, LP } , 0 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandStar );
// 	SetSelfReLinkTree ( { 0, LP, 0 } );
// 	// LeftRight 2
// 	this->AddCommandBaseTree ( { 0, LP, 0 } , RP , 2 , 4 , 0 , &ACPP_CharacterPaul::CommandStar );

	this->mCurrCommandTree = mBaseCommandTree;
	this->sCurrCommand = mBaseCommandTree[0];
}


FCommandTree* ACPP_CharacterPaul::CreateCommandTree ( int32 timingStart , int32 timingEnd , int32 timingAction , void (ACPP_CharacterPaul::* fptr)() )
{
	FCommandTree* NewCommand = new FCommandTree ( );

	//NewCommand->PrevTrees;
	NewCommand->NextTrees = TMap<int32 , FCommandTree*> ( );
	NewCommand->timingStart = timingStart;
	NewCommand->timingEnd = timingEnd;
	NewCommand->timingAction = timingAction;
	NewCommand->action.BindUObject ( this , fptr );

	return NewCommand;
}
FCommandTree* ACPP_CharacterPaul::AddCommandTree ( TMap<int32 , FCommandTree*>& CurrCommandTree , int32 keyValue , int32 timingStart , int32 timingEnd , int32 timingAction , void(ACPP_CharacterPaul::* fptr)() )
{
	//  음 빈거 확인해야 하는데 잘안됨 확인할 필요있음
	if ( &CurrCommandTree && CurrCommandTree.Find ( keyValue ) )
	{
		if ( DebugingMode )
			UE_LOG ( LogTemp , Warning , TEXT ( "Error: befor add CommandTree was settinged!! %i" ) , keyValue );

		return CurrCommandTree[keyValue];
	}

	CurrCommandTree.Add ( keyValue , this->CreateCommandTree ( timingStart , timingEnd , timingAction , fptr ) );
	return CurrCommandTree[keyValue];
}

FCommandTree* ACPP_CharacterPaul::AddCommandBaseTree ( TArray<int> arrayTreeCommand , int32 keyValue , int32 timingStart , int32 timingEnd , int32 timingAction , void(ACPP_CharacterPaul::* fptr)() )
{
	TMap<int32 , FCommandTree*> temp = mBaseCommandTree;
	TMap<int32 , FCommandTree*> tempParant = mBaseCommandTree;
	int lastkey = 0;
	for ( int commandkey : arrayTreeCommand )
	{
		tempParant = temp;
		if ( !temp.Find ( commandkey ) )
		{
			if ( DebugingMode )
				UE_LOG ( LogTemp , Error , TEXT ( "[SetParentReLinkTree] No Tree Command " ) );

			return nullptr;
		}
		temp = temp[commandkey]->NextTrees;
		lastkey = commandkey;
	}
	if ( tempParant[lastkey]->NextTrees.Find ( keyValue ) )
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Error: befor add CommandTree was settinged!! %i" ) , keyValue );
		return tempParant[lastkey]->NextTrees[lastkey];
	}

	tempParant[lastkey]->NextTrees.Add ( keyValue , this->CreateCommandTree ( timingStart , timingEnd , timingAction , fptr ) );
	return tempParant[lastkey]->NextTrees[keyValue];
}

void ACPP_CharacterPaul::SetSelfReLinkTree ( TArray<int32> arrayTreeCommand )
{
	TMap<int32 , FCommandTree*> temp = mBaseCommandTree;
	TMap<int32 , FCommandTree*> tempParant = mBaseCommandTree;
	int lastkey = 0;
	for ( int commandkey : arrayTreeCommand )
	{
		tempParant = temp;
		if ( !temp.Find ( commandkey ) )
		{
			if ( DebugingMode )
				UE_LOG ( LogTemp , Error , TEXT ( "[SetParentReLinkTree] No Tree Command " ) );

			return;
		}
		temp = temp[commandkey]->NextTrees;
		lastkey = commandkey;
	}
	tempParant[lastkey]->NextTrees.Add ( lastkey , tempParant[lastkey] );
}


// Called to bind functionality to input
void ACPP_CharacterPaul::SetupPlayerInputComponent ( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent ( PlayerInputComponent );
}

// Command Funtion

void ACPP_CharacterPaul::CommandIdle ( )
{
	//if ( DebugingMode )
	//	UE_LOG ( LogTemp , Warning , TEXT ( "CommandIdle Pressed" ) );

	this->bCrouched =false;
	//this->uCharacterMesh->SetRelativeScale3D ( FVector ( 1 , 1 , 1 ) );
	this->uCharacterMesh->SetRelativeLocation ( FVector ( 0 , 0 , -90 ) );

	if ( this->CountIdleFrame > 3 )
	{
		this->CountIdleFrame = 0;
		if ( DebugingMode )
			UE_LOG ( LogTemp , Warning , TEXT ( "Clean Command" ) );

		this->mCurrCommandTree = mBaseCommandTree[0]->NextTrees;
		this->sCurrCommand = mBaseCommandTree[0];
		return;
	}
	CountIdleFrame++;
	CountStarFrame = 3;
}

void ACPP_CharacterPaul::CommandStar ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandStar Pressed" ) );

	if ( this->CountStarFrame <= 0 )
	{
		this->CountStarFrame = 0;
		if ( DebugingMode )
			UE_LOG ( LogTemp , Warning , TEXT ( "Star Clean Command" ) );

		this->mCurrCommandTree = mBaseCommandTree[0]->NextTrees;
		this->sCurrCommand = mBaseCommandTree[0];
		return;
	}
	CountStarFrame--;
}

void ACPP_CharacterPaul::CommandEnd ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandEnd Pressed" ) );
}


void ACPP_CharacterPaul::CommandMoveForward ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveForward Pressed" ) );

	this->SetToLocationPoint ( 30 , 0 , 0 );

	this->CountStarFrame = 5;
}

void ACPP_CharacterPaul::CommandMoveForwarDash ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "MoveForwarDash Pressed" ) );

	this->SetToLocationPoint ( 200 , 0 , 0 );
}

void ACPP_CharacterPaul::CommandMoveBack ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveBack Pressed" ) );

	this->SetToLocationPoint ( -30 , 0 , 0 );
	this->CountStarFrame = 5;
}

void ACPP_CharacterPaul::CommandMoveBackDash ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveBackDash Pressed" ) );

	this->SetToLocationPoint ( -200 , 0 , 0 );
}

void ACPP_CharacterPaul::CommandJump ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJump Pressed" ) );
	
	
	//this->GetMesh ( )->AddImpulse ( FVector::UpVector * 100 );
	//this->uCharacterMesh->SetRelativeScale3D ( FVector ( 0.8 , 0.8 , 0.8 ) );
	this->SetToLocationPoint ( 0 , 0 , 130 );
	
	this->sFrameStatus.FrameUsing = 25;
}

void ACPP_CharacterPaul::CommandMoveLateralUp ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveLateralPlus Pressed" ) );
// 
	this->bCrouched = false;

	attackInfo.DamagePoint = EDamagePointInteraction::Special;
	attackInfo.DamageAmount = 0;

	this->attackInfo.ActionFrame = 1;

	this->SetToLocationPoint ( 30 , -150 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgMoveLateral );

	CountStarFrame = 5;

	this->sFrameStatus.FrameUsing = 3;
}

void ACPP_CharacterPaul::CommandDownCrouch ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandDownCrouch Pressed" ) );

	this->bCrouched = true;
	//this->GetCapsuleComponent ( )->SetWorldScale3D ( FVector ( 0.1 , 0.6 , 0.1 ) );
	//this->uCharacterMesh->SetRelativeLocation ( FVector ( 0 , 0 , -130 ) );
	CountStarFrame = 3;
}

void ACPP_CharacterPaul::CommandUpCrouch ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandUpCrouch Pressed" ) );

	CountStarFrame = 10;
}

void ACPP_CharacterPaul::CommandMoveLateralDown ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveLateralDown Pressed" ) );

	attackInfo.DamagePoint = EDamagePointInteraction::Special;
	attackInfo.DamageAmount = 0;

	this->attackInfo.ActionFrame = 1;

	CountStarFrame = 8;

	this->SetToLocationPoint ( 30 , 150 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgMoveLateral );

	this->sFrameStatus.FrameUsing = 3;
}

void ACPP_CharacterPaul::CommandLeadJab ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandLeadJab Pressed" ) );

	attackInfo.DamagePoint = EDamagePointInteraction::Top;
	attackInfo.DamageAmount = 5;

	attackInfo.ActionFrame = 10;
	attackInfo.RetrieveFrame = 5;

	attackInfo.MissFrame = 0;
	attackInfo.HitFrame = 1;
	attackInfo.GrardFrame = 8;
	attackInfo.CounterFrame = 8;
	
	attackInfo.skellEffectLocation = this->RelativePointVector ( 90 , 5 , 50 );
	attackInfo.KnockBackDirection = this->RelativePointVector ( 140 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수
	
	attackInfo.cameraShake = 0;
	attackInfo.cameraZoom = 0;
	attackInfo.cameraDelay = 0;

	attackInfo.debugColor = FColor ( 255 , 128 , 128 );
	this->SetToLocationPoint ( 30 , 0 , 0 );


	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgLeadJab );

	this->sFrameStatus.FrameUsing = attackInfo.ActionFrame;

}

void ACPP_CharacterPaul::CommandCrossStaight ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandCrossStaight Pressed" ) );

	attackInfo.DamagePoint = EDamagePointInteraction::Top;
	attackInfo.DamageAmount = 10;

	attackInfo.ActionFrame = 10;
	attackInfo.RetrieveFrame = 5;

	attackInfo.MissFrame = 0;
	attackInfo.HitFrame = 0;
	attackInfo.GrardFrame = 6;
	attackInfo.CounterFrame = 6;


	attackInfo.skellEffectLocation = this->RelativePointVector ( 120 , -5 , 60 );
	attackInfo.KnockBackDirection = this->RelativePointVector ( 160 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	attackInfo.cameraShake = 0;
	attackInfo.cameraZoom = 0;
	attackInfo.cameraDelay = 0;

	this->SetToLocationPoint ( 20 , 0 , 0 );


	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( this->uMtgCrossStaight );

	this->sFrameStatus.FrameUsing = attackInfo.ActionFrame;
}



void ACPP_CharacterPaul::CommandJingun ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJingun Pressed" ) );

	attackInfo.DamagePoint = EDamagePointInteraction::Middle;
	attackInfo.DamageAmount = 14;

	attackInfo.ActionFrame = 15;
	attackInfo.RetrieveFrame = 5;

	attackInfo.MissFrame = 0;
	attackInfo.HitFrame = -1;
	attackInfo.GrardFrame = -12;
	attackInfo.CounterFrame = -1;
	attackInfo.skellEffectLocation = this->RelativePointVector ( 170 , 5 , 00 );
	attackInfo.KnockBackDirection = this->RelativePointVector ( 180 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	attackInfo.cameraShake = 0;
	attackInfo.cameraZoom = 0;
	attackInfo.cameraDelay = 0;

	this->SetToLocationPoint ( 10 , 0 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgJingun );

	this->sFrameStatus.FrameUsing = attackInfo.ActionFrame;
}

void ACPP_CharacterPaul::CommandHighKick ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJingun Pressed" ) );

	attackInfo.DamagePoint = EDamagePointInteraction::Top;
	attackInfo.DamageAmount = 17;

	attackInfo.ActionFrame = 12;
	attackInfo.RetrieveFrame = 5;

	attackInfo.MissFrame = 0;
	attackInfo.HitFrame = 4;
	attackInfo.GrardFrame = 14;
	attackInfo.CounterFrame = 57;
	attackInfo.skellEffectLocation = this->RelativePointVector ( 160 , -5 , 60 );
	attackInfo.KnockBackDirection = this->RelativePointVector ( 180 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	attackInfo.cameraShake = 0;
	attackInfo.cameraZoom = 0;
	attackInfo.cameraDelay = 0;

	this->SetToLocationPoint ( 5 , 0 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgHighKick );

	this->sFrameStatus.FrameUsing = attackInfo.ActionFrame;
}

void ACPP_CharacterPaul::CommandBungGuan ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandBungGuan Pressed" ) );

	this->bCrouched = false;

	attackInfo.DamagePoint = EDamagePointInteraction::Middle;
	attackInfo.DamageAmount = 17;

	attackInfo.ActionFrame = 0;
	attackInfo.RetrieveFrame = 0;

	attackInfo.MissFrame = 0;
	attackInfo.HitFrame = 0;
	attackInfo.GrardFrame = 0;
	attackInfo.CounterFrame = 0;

	attackInfo.skellEffectLocation = this->RelativePointVector ( 200 , -5 , 60 );
	attackInfo.KnockBackDirection = this->RelativePointVector ( 500 , 0 , 20 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	attackInfo.cameraShake = 0;
	attackInfo.cameraZoom = 0;
	attackInfo.cameraDelay = 0;

	this->SetToLocationPoint ( 30 , 0 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgBungGuan );

	this->sFrameStatus.FrameUsing = attackInfo.ActionFrame;
}

void ACPP_CharacterPaul::CommandJinJee ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJinJee Pressed" ) );

	this->bCrouched = false;


	attackInfo.DamagePoint = EDamagePointInteraction::Middle;
	attackInfo.DamageAmount = 20;

	attackInfo.ActionFrame = 5;
	attackInfo.RetrieveFrame = 35;

	attackInfo.MissFrame = 0;
	attackInfo.HitFrame = 15;
	attackInfo.GrardFrame = -14;
	attackInfo.CounterFrame = 0;

	attackInfo.skellEffectLocation = this->RelativePointVector ( 80 , -5 , 10 );
	attackInfo.KnockBackDirection = this->RelativePointVector ( 200 , 0 , 300 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	attackInfo.cameraShake = 0;
	attackInfo.cameraZoom = 0;
	attackInfo.cameraDelay = 0;

	this->SetToLocationPoint ( 10 , 0 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgLeadJab );

	this->sFrameStatus.FrameUsing = attackInfo.ActionFrame;
}


void ACPP_CharacterPaul::CommandSitJab( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandSitJab Pressed" ) );

	this->bCrouched = false;


	attackInfo.DamagePoint = EDamagePointInteraction::Middle;
	attackInfo.DamageAmount = 15;

	attackInfo.ActionFrame = 4;
	attackInfo.RetrieveFrame = 15;

	attackInfo.MissFrame = 0;
	attackInfo.HitFrame = 10;
	attackInfo.GrardFrame = -5;
	attackInfo.CounterFrame = 6;

	attackInfo.skellEffectLocation = this->RelativePointVector ( 80 , -5 , 10 );
	attackInfo.KnockBackDirection = this->RelativePointVector ( 120 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	attackInfo.cameraShake = 0;
	attackInfo.cameraZoom = 0;
	attackInfo.cameraDelay = 0;

	this->SetToLocationPoint ( 10 , 0 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgSitJab );

	this->sFrameStatus.FrameUsing = attackInfo.ActionFrame;
}

void ACPP_CharacterPaul::CommandSitSpineKick ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandSitJab Pressed" ) );

	this->bCrouched = false;


	attackInfo.DamagePoint = EDamagePointInteraction::Lower;
	attackInfo.DamageAmount = 15;

	attackInfo.ActionFrame = 4;
	attackInfo.RetrieveFrame = 15;

	attackInfo.MissFrame = 0;
	attackInfo.HitFrame = 10;
	attackInfo.GrardFrame = -5;
	attackInfo.CounterFrame = 6;

	attackInfo.skellEffectLocation = this->RelativePointVector ( 140 , -5 , -40 );
	attackInfo.KnockBackDirection = this->RelativePointVector ( 160 , -5 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	attackInfo.cameraShake = 0;
	attackInfo.cameraZoom = 0;
	attackInfo.cameraDelay = 0;

	this->SetToLocationPoint ( 10 , 0 , 0 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgSitSpineKick );

	this->sFrameStatus.FrameUsing = attackInfo.ActionFrame;
}

bool ACPP_CharacterPaul::CommandAllStop ( )
{
	if ( DebugingMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandAllStop Pressed" ) );
	this->mCurrCommandTree = mBaseCommandTree[0]->NextTrees;
	return 0;
}

/************************************************************************/
/*               맞는 것을 정의한 공간 입니다                           */
/************************************************************************/
bool ACPP_CharacterPaul::HitDecision ( FAttackInfoInteraction attackInfoHit , ACPP_Tekken8CharacterParent* ownerHitPlayer )
{
	if ( aMainCamera )
		aMainCamera->RequestZoomEffect ( attackInfoHit.skellEffectLocation , attackInfoHit.cameraZoom , attackInfoHit.cameraShake , attackInfoHit.cameraDelay );
	else
		UE_LOG(LogTemp, Warning, TEXT("is Emtyp ") );
	if ( attackInfoHit.DamagePoint == EDamagePointInteraction::Top && this->eCharacterState == ECharacterStateInteraction::GuardStand )
	{
		this->sFrameStatus.FrameBlockUsing = attackInfoHit.OppositeGuardFrame;
		this->SetToWorldLocationPoint ( attackInfoHit.KnockBackDirection / 4 );
		// defense animation 추가하기
		PlayMontageFrameSystem ( uMtgDefence );

		return false;
	}
	if ( attackInfoHit.DamagePoint == EDamagePointInteraction::Middle && this->eCharacterState == ECharacterStateInteraction::GuardStand )
	{
		this->sFrameStatus.FrameBlockUsing = attackInfoHit.OppositeGuardFrame;
		this->SetToWorldLocationPoint ( attackInfoHit.KnockBackDirection / 4 );
		// defense animation 추가하기
		if (this->bCrouched )
			PlayMontageFrameSystem ( uMtgSitDefence );
		else
			PlayMontageFrameSystem ( uMtgDefence );

		return false;
	}
	if ( attackInfoHit.DamagePoint == EDamagePointInteraction::Lower && this->eCharacterState == ECharacterStateInteraction::GuardSit )
	{
		this->sFrameStatus.FrameBlockUsing = attackInfoHit.OppositeGuardFrame;
		this->SetToWorldLocationPoint ( attackInfoHit.KnockBackDirection / 4 );
		// defense animation 추가하기
		PlayMontageFrameSystem ( uMtgSitDefence );
		return false;
	}
	this->SetActorRotation ( UKismetMathLibrary::FindLookAtRotation ( this->GetActorLocation ( ) , this->aOpponentPlayer->GetActorLocation ( ) ) );
	this->sFrameStatus.FrameBlockUsing = attackInfoHit.OppositeHitFrame;
	this->SetToWorldLocationPoint ( attackInfoHit.KnockBackDirection );
	// heart animation 추가하기
	if ( attackInfoHit.DamagePoint == EDamagePointInteraction::Lower )
		PlayMontageFrameSystem ( uMtgSitHit);
	else
		PlayMontageFrameSystem ( uMtgIdleHit );
		
	// 좀 있다 이동 시키기
	//camera 효과 추가하기s
	this->CommandAllStop();
	return true;
}


void  ACPP_CharacterPaul::CommentHitFrameExecute ( )
{
	float radius = 20.0f;
	if ( attackInfo.DamagePoint == EDamagePointInteraction::Special )
		return;
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add ( UEngineTypes::ConvertToObjectType ( ECollisionChannel::ECC_Pawn ) );
	TArray<AActor*> ignoreActors;
	ignoreActors.Init ( this , 1 );
	TArray<AActor*> outActors;
	FVector sphereSpawnLocation = attackInfo.skellEffectLocation;
	UClass* seekClass = ACPP_Tekken8CharacterParent::StaticClass ( );

	bool hit = UKismetSystemLibrary::SphereOverlapActors ( GetWorld ( ) , sphereSpawnLocation , radius , traceObjectTypes , seekClass , ignoreActors , outActors );;
	if ( hit )
	{
		for ( AActor* hitActor : outActors )
		{
			if ( hitActor->IsA<ACPP_Tekken8CharacterParent> ( ) )
			{
				ACPP_Tekken8CharacterParent* hitCharacter = Cast<ACPP_Tekken8CharacterParent> ( hitActor );
				if ( hitCharacter->HitDecision ( attackInfo , this ) )
					sFrameStatus.FrameBlockUsing = attackInfo.OwnerGuardFrame;
				else
					sFrameStatus.FrameBlockUsing = attackInfo.OwnerGuardFrame;
			}
		}
	}
	else
	{
		sFrameStatus.FrameBlockUsing = attackInfo.OwnerMissFrame;
	}

	if ( attackInfo.DamagePoint == EDamagePointInteraction::Top )
		attackInfo.debugColor = FColor ( 255 , 0 , 0 );
	else if ( attackInfo.DamagePoint == EDamagePointInteraction::Middle )
		attackInfo.debugColor = FColor ( 255 , 125 , 0 );
	else if ( attackInfo.DamagePoint == EDamagePointInteraction::Lower )
		attackInfo.debugColor = FColor ( 0 , 0 , 255 );


	DrawDebugSphere ( GetWorld ( ) , attackInfo.skellEffectLocation , radius , 26 , attackInfo.debugColor , false , 1.0f );
}

//void ACPP_CharacterPaul::HowtoUseSphereOverlapActors ( )
//{
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
//}

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