// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_CharacterPaul.h"
#include  "Kismet/KismetMathLibrary.h"
// Sets default values
ACPP_CharacterPaul::ACPP_CharacterPaul()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->uCharacterMesh = this->GetMesh ( );

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (tempSkeletalMesh.Succeeded())
		uCharacterMesh->SetSkeletalMeshAsset(tempSkeletalMesh.Object);
	uCharacterMesh->SetRelativeLocation(FVector(0,0,-90));
	uCharacterMesh->SetRelativeRotation(FRotator(0,0,-90));

}
// Called when the game starts or when spawned
void ACPP_CharacterPaul::BeginPlay ( )
{
	Super::BeginPlay ( );

	// 커멘드를 설정합니다.
	this->SettingCommandTree ( );
	// 위치 세팅
	this->ToLocation = this->GetActorLocation ( ) + this->GetActorForwardVector ( ) * 100;
}
// Called every frame
void ACPP_CharacterPaul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 주의 모든 작업은 FrameSystem에서 하는걸 권장합니다.
	this->fCurrTimeForFrame += DeltaTime;

	if (this->fCurrTimeForFrame > this->fFrameTime)
	{
		this->fCurrTimeForFrame = 0;
		this->FrameSystem();
	}
	
}



/**
 * @title Frame 시스템을 위한 함수입니다.
 */
void ACPP_CharacterPaul::FrameSystem()
{
	 FVector dir = (this->ToLocation - this->GetActorLocation()) / 60;
	 int32 currKeyValue = this->GetCurrInputKeyValue ( );
	 this->SetActorLocation ( this->GetActorLocation ( ) + dir );
	 if ( currKeyValue )
		 this->SetActorRotation ( UKismetMathLibrary::FindLookAtRotation ( this->GetActorLocation ( ) , this->aOpponentPlayer->GetActorLocation ( ) ) );
	if (!(fCurrFrame % 2))
	{
		if (this->mCurrCommandTree.Find( currKeyValue ))
		{
			FCommandTree* temptree = this->mCurrCommandTree[currKeyValue];
			//UE_LOG ( LogTemp , Warning , TEXT ( "input : %i " ) , this->GetCurrInputKeyValue ( ) );
			if( temptree != nullptr )
			{
				temptree->action.Execute ( );

				if (this->mCurrCommandTree.Find(currKeyValue) )
					mCurrCommandTree = this->mCurrCommandTree[currKeyValue]->NextTrees;
				else
					mCurrCommandTree = mBaseCommandTree;
			}
		}
		else
		{
			this->CommandIdle();
		}
	}
	if (fCurrFrame > 60)
		fCurrFrame = 0;
	fCurrFrame++;
}


void ACPP_CharacterPaul::SetToLocationPoint(float x, float y, float z)
{
	FVector moveToPoint = this->GetActorLocation() +
	(
		this->GetActorForwardVector() * x +
		this->GetActorRightVector() * y +
		this->GetActorUpVector() * z
	);
	this->ToLocation = moveToPoint;
}

void ACPP_CharacterPaul::SetToLocationPoint ( FVector vector)
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
		LeftArm		* 0b0000001000000000 +
		RightArm	* 0b0000010000000000 +
		LeftKick	* 0b0000100000000000 +
		RightKick	* 0b0001000000000000;
	return inputValue;
}


void ACPP_CharacterPaul::SettingMove ( int32 ArrowNum , bool Value )
{

}

// Command Setting Function

void ACPP_CharacterPaul::SettingCommandTree ( )
{
	FCommandTree* mTempTarget = new FCommandTree();

	this->AddCommandTree ( mBaseCommandTree , 0 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandIdle );
	mBaseCommandTree[0]->NextTrees.Add ( InputKeyValue ( 0 , 0 , 0 , 0 , 0 ) , mBaseCommandTree[0] );

	/**
	 * MoveMent 
	 */
	// MoveForward;
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees , 32 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandMoveForward );
	mBaseCommandTree[0]->NextTrees[32]->NextTrees.Add ( 32, mBaseCommandTree[0]->NextTrees[32] );
	
	// MoveForward Dash
 	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees[32]->NextTrees , 0, 0, 0, 0 , &ACPP_CharacterPaul::CommandIdle );
	// dash 3프레임후 초기화
	mBaseCommandTree[0]->NextTrees[32]->NextTrees[0]->NextTrees.Add(0, mBaseCommandTree[0]->NextTrees[32]->NextTrees[0]);
	// DASH while
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees[32]->NextTrees[0]->NextTrees , 32 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandMoveForwarDash );
	mBaseCommandTree[0]->NextTrees[32]->NextTrees[0]->NextTrees[32]->NextTrees.Add ( 32 , mBaseCommandTree[0]->NextTrees[32]->NextTrees[0]->NextTrees[32] );

	// MoveBack;
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees , 8 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandMoveBack );
	mBaseCommandTree[0]->NextTrees[8]->NextTrees.Add ( 8 , mBaseCommandTree[0]->NextTrees[8] );

	// MoveBack Dash
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees[8]->NextTrees , 0 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandIdle );
	// dash 3프레임후 초기화
	mBaseCommandTree[0]->NextTrees [8]->NextTrees[0]->NextTrees.Add ( 0 , mBaseCommandTree[0]->NextTrees[8]->NextTrees[0] );
	// DASH while
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees[8]->NextTrees[0]->NextTrees , 8 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandMoveBackDash );
	mBaseCommandTree[0]->NextTrees[8]->NextTrees[0]->NextTrees[8]->NextTrees.Add ( 8 , mBaseCommandTree[0]->NextTrees[8]->NextTrees[0]->NextTrees[8] );


	// Move Lateral Plus
	int32 upkey = InputKeyValue ( 8 , 0 , 0 , 0 , 0 );
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees , upkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandIdle );
	// 3frame wait
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees[upkey]->NextTrees , 0 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandMoveLateralUp );
	mBaseCommandTree[0]->NextTrees[upkey]->NextTrees[0]->NextTrees.Add ( 0 , mBaseCommandTree[0]->NextTrees[upkey]->NextTrees[0] );
	// Move while Lateral
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees[upkey]->NextTrees[0]->NextTrees, upkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandMoveLateralUp );
	mBaseCommandTree[0]->NextTrees[upkey]->NextTrees[0]->NextTrees[upkey]->NextTrees.Add ( upkey , mBaseCommandTree[0]->NextTrees[upkey]->NextTrees[0]->NextTrees[upkey] );


	// Jump
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees , upkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandIdle );
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees[upkey]->NextTrees , upkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandJump );
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees[upkey]->NextTrees[upkey]->NextTrees , upkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandIdle );
	mBaseCommandTree[0]->NextTrees[upkey]->NextTrees[upkey]->NextTrees.Add ( upkey , mBaseCommandTree[0]->NextTrees[upkey]->NextTrees[upkey] );

	// Move Lateral Minus
	int32 downkey = InputKeyValue ( 2 , 0 , 0 , 0 , 0 );
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees , downkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandIdle );
	// 3frame wait
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees[downkey]->NextTrees , 0 , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandMoveLateralDown );
	mBaseCommandTree[0]->NextTrees[downkey]->NextTrees[0]->NextTrees.Add ( 0 , mBaseCommandTree[0]->NextTrees[downkey]->NextTrees[0] );
	// Move while Lateral
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees[downkey]->NextTrees[0]->NextTrees , downkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandMoveLateralDown );
	mBaseCommandTree[0]->NextTrees[downkey]->NextTrees[0]->NextTrees[downkey]->NextTrees.Add ( downkey , mBaseCommandTree[0]->NextTrees[downkey]->NextTrees[0]->NextTrees[downkey] );

	// Crouch
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees , downkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandIdle );
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees[downkey]->NextTrees , downkey , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandDownCrouch );
	mBaseCommandTree[0]->NextTrees[downkey]->NextTrees[downkey]->NextTrees.Add ( downkey , mBaseCommandTree[0]->NextTrees[downkey]->NextTrees[downkey] );

	// LeftRightCombo
	int32 LP = InputKeyValue ( 0 , true , 0 , 0 , 0 );
	int32 RP = InputKeyValue ( 0 , 0 , true , 0 , 0 );
	// LeftRight 1 
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees , LP , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandLeftRightCombo_1 );
	// LeftRight 2
	this->AddCommandTree ( mBaseCommandTree[0]->NextTrees[LP]->NextTrees , RP , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandLeftRightCombo_2 );

	//mTempTarget.Add ( InputKeyValue ( 6 , 0 , 0 , 0 , 0 ) , mTempTree[InputKeyValue ( 6 , 0 , 0 , 0 , 0 )] );

	// 	this->AddCommandTree ( mBaseCommandTree , InputKeyValue ( 4 , 0 , 0 , 0 , 0 ) , 4 , 1 , 2 , &ACPP_CharacterPaul::CommandMoveForward );
	// 	this->AddCommandTree ( mBaseCommandTree , InputKeyValue ( 4 , 0 , 0 , 0 , 0 ) , 4 , 1 , 2 , &ACPP_CharacterPaul::CommandMoveForwarDash );
	// 	this->AddCommandTree ( mBaseCommandTree , InputKeyValue ( 4 , 0 , 0 , 0 , 0 ) , 4 , 1 , 2 , &ACPP_CharacterPaul::CommandMoveForwarDash );
	// 	this->AddCommandTree ( mBaseCommandTree , InputKeyValue ( 6 , 0 , 0 , 0 , 0 ) , 4 , 1 , 2 , &ACPP_CharacterPaul::CommandMoveBack );
	// 	this->AddCommandTree ( mBaseCommandTree , InputKeyValue ( 8 , 0 , 0 , 0 , 0 ) , 4 , 1 , 2 , &ACPP_CharacterPaul::CommandMoveBackDash );
	// 	this->AddCommandTree ( mBaseCommandTree , InputKeyValue ( 6 , 0 , 0 , 0 , 0 ) , 4 , 1 , 2 , &ACPP_CharacterPaul::CommandJump );
	// 	this->AddCommandTree ( mBaseCommandTree , InputKeyValue ( 8 , 0 , 0 , 0 , 0 ) , 4 , 1 , 2 , &ACPP_CharacterPaul::CommandMoveLateralPlus );
	// 	this->AddCommandTree ( mBaseCommandTree , InputKeyValue ( 6 , 0 , 0 , 0 , 0 ) , 4 , 1 , 2 , &ACPP_CharacterPaul::CommandDownCrouch );
	// 	this->AddCommandTree ( mBaseCommandTree , InputKeyValue ( 8 , 0 , 0 , 0 , 0 ) , 4 , 1 , 2 , &ACPP_CharacterPaul::CommandMoveLateralMinus );
	this->mCurrCommandTree = mBaseCommandTree;
}


FCommandTree* ACPP_CharacterPaul::CreateCommandTree ( int32 timingStart , int32 timingEnd , int32 timingAction , void (ACPP_CharacterPaul::* fptr)() )
{
	FCommandTree* NewCommand = new FCommandTree ( );

	//NewCommand->PrevTrees;
	NewCommand->NextTrees = TMap<int32 , FCommandTree*>();
	NewCommand->timingStart = timingStart;
	NewCommand->timingEnd = timingEnd;
	NewCommand->timingAction = timingAction;
	NewCommand->action.BindUObject ( this , fptr );

	//NewCommand->action.Execute();
	return NewCommand;
}

FCommandTree* ACPP_CharacterPaul::AddCommandTree ( TMap<int32 , FCommandTree*>& CurrCommandTree , int32 keyValue , int32 timingStart , int32 timingEnd , int32 timingAction , void(ACPP_CharacterPaul::* fptr)() )
{
	//  음 빈거 확인해야 하는데 잘안됨 확인할 필요있음
	if (&CurrCommandTree && CurrCommandTree.Find(keyValue) )
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Error: befor add CommandTree was settinged!! %i" ) , keyValue );
		return CurrCommandTree[keyValue];
	}

	CurrCommandTree.Add ( keyValue , this->CreateCommandTree ( timingStart , timingEnd , timingAction , fptr ) );
	return CurrCommandTree[keyValue];
}

// Called to bind functionality to input
void ACPP_CharacterPaul::SetupPlayerInputComponent ( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent ( PlayerInputComponent );
}

// Command Funtion

void ACPP_CharacterPaul::CommandIdle ( )
{
	//UE_LOG ( LogTemp , Warning , TEXT ( "CommandIdle Pressed" ) );

	this->uCharacterMesh->SetRelativeScale3D ( FVector ( 1 , 1 , 1 ) );
	this->uCharacterMesh->SetRelativeLocation ( FVector ( 0 , 0 , -90 ) );

	if ( this->CountIdle3Frame > 3 )
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Clean Command" ) );
		this->mCurrCommandTree = mBaseCommandTree;
		this->CountIdle3Frame = 0;
		return;
	}
	CountIdle3Frame++;
}

void ACPP_CharacterPaul::CommandStar ( )
{
	UE_LOG ( LogTemp , Warning , TEXT ( "CommandIdle Pressed" ) );
}

void ACPP_CharacterPaul::CommandMoveForward()
{
	UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveForward Pressed" ) );
	this->SetToLocationPoint(30, 0, 0);
	this->CountIdle3Frame = 0;
}

void ACPP_CharacterPaul::CommandMoveForwarDash()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveForwarDash Pressed"));
	this->SetToLocationPoint ( 200 , 0 , 0 );
	this->CountIdle3Frame = 0;

}

void ACPP_CharacterPaul::CommandMoveBack()
{
	UE_LOG(LogTemp, Warning, TEXT("CommandMoveBack Pressed"));
	this->SetToLocationPoint ( -30 , 0 , 0 );
}

void ACPP_CharacterPaul::CommandMoveBackDash ()
{
	UE_LOG(LogTemp, Warning, TEXT("CommandMoveBackDash Pressed"));
	this->SetToLocationPoint ( -200 , 0 , 0 );
}

void ACPP_CharacterPaul::CommandJump ()
{
	UE_LOG(LogTemp, Warning, TEXT("CommandJump Pressed"));

	this->uCharacterMesh->SetRelativeScale3D ( FVector ( 0.8 , 0.8 , 0.8 ) );
	this->uCharacterMesh->SetRelativeLocation ( FVector ( 0 , 0 , 130 ) );
}

void ACPP_CharacterPaul::CommandMoveLateralUp ()
{
	UE_LOG(LogTemp, Warning, TEXT("CommandMoveLateralPlus Pressed"));
	
	FVector dir = this->aOpponentPlayer->GetActorLocation() - this->GetActorLocation();
	dir.Normalize();
	FQuat Q = FQuat(dir,0);

	this->SetToLocationPoint ( 30 , -150 , 0 );
}

void ACPP_CharacterPaul::CommandDownCrouch ()
{
	UE_LOG(LogTemp, Warning, TEXT("CommandDownCrouch Pressed"));
	this->uCharacterMesh->SetRelativeScale3D ( FVector ( 0.8 , 0.8 , 0.8 ) );
	this->uCharacterMesh->SetRelativeLocation ( FVector ( 0 , 0 , -130 ) );
}

void ACPP_CharacterPaul::CommandMoveLateralDown ()
{
	UE_LOG(LogTemp, Warning, TEXT("CommandMoveLateralDown Pressed"));
	
	this->SetToLocationPoint(30, 150, 0);

}

void ACPP_CharacterPaul::CommandLeftRightCombo_1 ( )
{
	UE_LOG ( LogTemp , Warning , TEXT ( "CommandLeftRightCombo_1 Pressed" ) );
	FSkellInfo skellinfo;

	skellinfo.skellHitDecision = eHitDecision::Top;
	skellinfo.skellDamage = 5;
	skellinfo.skellEffectLocation = this->RelativePointVector ( 50 , 5 , 80 );
	skellinfo.skellNuckbuck = FVector( -10, 0, 0 );
	skellinfo.cameraShake = 0;
	skellinfo.cameraZoom = 0;

	this->SetToLocationPoint ( 30 , 0 , 0 );

	DrawDebugSphere(GetWorld(), skellinfo.skellEffectLocation, 12, 26, FColor(0,255,0), true, 2);
}

void ACPP_CharacterPaul::CommandLeftRightCombo_2 ( )
{
	UE_LOG ( LogTemp , Warning , TEXT ( "CommandLeftRightCombo_2 Pressed" ) );
	FSkellInfo skellinfo;

	skellinfo.skellHitDecision = eHitDecision::Top;
	skellinfo.skellDamage = 12;
	skellinfo.skellEffectLocation = this->RelativePointVector( 70 , -5 , 60 );
	skellinfo.skellNuckbuck = FVector ( -10 , 0 , 0 );
	skellinfo.cameraShake = 0;
	skellinfo.cameraZoom = 0;

	DrawDebugSphere ( GetWorld ( ) , skellinfo.skellEffectLocation , 12 , 26 , FColor ( 225 , 0 , 225 ) , true , 2 );
}
