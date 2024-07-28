// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_CharacterPaul.h"

// Sets default values
ACPP_CharacterPaul::ACPP_CharacterPaul()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMeshComponent* tempCharacterMesh = this->GetMesh();
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (tempSkeletalMesh.Succeeded())
		tempCharacterMesh->SetSkeletalMeshAsset(tempSkeletalMesh.Object);
	tempCharacterMesh->SetRelativeLocation(FVector(0,0,-90));
	tempCharacterMesh->SetRelativeRotation(FRotator(0,0,-90));

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

FInputKey ACPP_CharacterPaul::GetCurrInputKey ( )
{
	return this->CurrInputKey;
}

int32 ACPP_CharacterPaul::GetCurrInputKeyValue()
{
	int32 KeyValue = 
		//this->CurrInputKey.bKey1		* 0b0000000000000001 +
		this->CurrInputKey.bKey2		* 0b0000000000000010 +
		//this->CurrInputKey.bKey3		* 0b0000000000000100 +
		this->CurrInputKey.bKey4		* 0b0000000000001000 +
		//this->CurrInputKey.bKey5		* 0b0000000000010000 +
		this->CurrInputKey.bKey6		* 0b0000000000100000 +
		//this->CurrInputKey.bKey7		* 0b0000000001000000 +
		this->CurrInputKey.bKey8		* 0b0000000010000000 +
		//this->CurrInputKey.bKey9		* 0b0000000100000000 +
		this->CurrInputKey.bLeftPunch	* 0b0000001000000000 +
		this->CurrInputKey.bRightPunch	* 0b0000010000000000 +
		this->CurrInputKey.bLeftKick	* 0b0000100000000000 +
		this->CurrInputKey.bRightKick	* 0b0001000000000000;

	return KeyValue;
}

bool ACPP_CharacterPaul::SetCurrInputKeyValue (int32 value)
{
	//this->CurrInputKey.bKey1		= value & 0b0000000000000001;
	this->CurrInputKey.bKey2		= value & 0b0000000000000010;
	//this->CurrInputKey.bKey3		= value & 0b0000000000000100;
	this->CurrInputKey.bKey4		= value & 0b0000000000001000;
	//this->CurrInputKey.bKey5		= value & 0b0000000000010000;
	this->CurrInputKey.bKey6		= value & 0b0000000000100000;
	//this->CurrInputKey.bKey7		= value & 0b0000000001000000;
	this->CurrInputKey.bKey8		= value & 0b0000000010000000;
	//this->CurrInputKey.bKey9		= value & 0b0000000100000000;
	this->CurrInputKey.bLeftPunch	= value & 0b0000001000000000;
	this->CurrInputKey.bRightPunch	= value & 0b0000010000000000;
	this->CurrInputKey.bLeftKick	= value & 0b0000100000000000;
	this->CurrInputKey.bRightKick	= value & 0b0001000000000000;

	return true;
}

bool ACPP_CharacterPaul::SetCurrInputKey ( FInputKey value )
{
	this->CurrInputKey = value;
	return true;
}

int32 ACPP_CharacterPaul::InputKeyValue(int ArrowKey, bool LeftArm, bool RightArm, bool LeftKick, bool RightKick)
{
	int32 inputValue = 0;
	
	inputValue = (ArrowKey ? 1 << (ArrowKey - 1) : 0) + 
	this->CurrInputKey.bLeftPunch * 0b0000001000000000 +
	this->CurrInputKey.bRightPunch * 0b0000010000000000 +
	this->CurrInputKey.bLeftKick * 0b0000100000000000 +
	this->CurrInputKey.bRightKick * 0b0001000000000000;
	return inputValue;
}


void ACPP_CharacterPaul::SettingCommandTree()
{
 	this->AddCommandTree(mBaseCommandTree, InputKeyValue(1, 0, 0, 0, 0), 4, 1, 2, &ACPP_CharacterPaul::Command1);
 	this->AddCommandTree(mBaseCommandTree, InputKeyValue(2, 0, 0, 0, 0), 4, 1, 2, &ACPP_CharacterPaul::Command2);
 	this->AddCommandTree(mBaseCommandTree, InputKeyValue(3, 0, 0, 0, 0), 4, 1, 2, &ACPP_CharacterPaul::Command3);
 	this->AddCommandTree(mBaseCommandTree, InputKeyValue(4, 0, 0, 0, 0), 4, 1, 2, &ACPP_CharacterPaul::Command4);
	this->AddCommandTree(mBaseCommandTree, InputKeyValue(6, 0, 0, 0, 0), 4, 1, 2, &ACPP_CharacterPaul::Command6);
 	this->AddCommandTree(mBaseCommandTree, InputKeyValue(7, 0, 0, 0, 0), 4, 1, 2, &ACPP_CharacterPaul::Command7);
 	this->AddCommandTree(mBaseCommandTree, InputKeyValue(8, 0, 0, 0, 0), 4, 1, 2, &ACPP_CharacterPaul::Command8);
 	this->AddCommandTree(mBaseCommandTree, InputKeyValue(9, 0, 0, 0, 0), 4, 1, 2, &ACPP_CharacterPaul::Command9);
}


FCommandTree* ACPP_CharacterPaul::CreateCommandTree(int32 timingStart, int32 timingEnd, int32 timingAction, void (ACPP_CharacterPaul::* fptr)())
{
	FCommandTree *NewCommand = new FCommandTree();
	
	//NewCommand->PrevTrees;
	//NewCommand->NextTrees;
	NewCommand->timingStart = timingStart;
	NewCommand->timingEnd = timingEnd;
	NewCommand->timingAction = timingAction;
	NewCommand->action.BindUObject(this, fptr);

	//NewCommand->action.Execute();
	return NewCommand;
}

FCommandTree* ACPP_CharacterPaul::AddCommandTree(TMap<int32, FCommandTree*> &CurrCommandTree,int32 keyValue, int32 timingStart, int32 timingEnd, int32 timingAction, void(ACPP_CharacterPaul::* fptr)())
{
//  음 빈거 확인해야 하는데 잘안됨 확인할 필요있음
// 	if (!CurrCommandTree && nullptr == CurrCommandTree[keyValue])
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Error: befor add CommandTree was settinged!! %i"), keyValue);
// 		return nullptr;
// 	}

	CurrCommandTree.Add(keyValue, this->CreateCommandTree(timingStart,timingEnd, timingAction, fptr));
	return CurrCommandTree[keyValue];
}

// Called to bind functionality to input
void ACPP_CharacterPaul::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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
 * @title 인풋처리를 위한 함수 입니다.
 */

void ACPP_CharacterPaul::SettingMove (int32 ArrowNum, bool Value )
{

}

/**
 * @title Frame 시스템을 위한 함수입니다.
 */
void ACPP_CharacterPaul::FrameSystem()
{
	 FVector dir = (this->ToLocation - this->GetActorLocation()) / 60;
	 this->SetActorLocation ( this->GetActorLocation ( ) + dir );

	if (!(fCurrFrame % 5))
	{
		//UE_LOG(LogTemp, Warning, TEXT("test frame"));dff
		FCommandTree** temptree = this->mBaseCommandTree.Find ( this->GetCurrInputKeyValue ( ) );
		if( temptree )
		{ 
			if ( *temptree && &(*temptree)->action )
			{ 
				//UE_LOG(LogTemp, Warning, TEXT("input : %i") , this->GetCurrInputKeyValue ( ) );
				(*temptree)->action.Execute();
			}
			else
			{
				//UE_LOG ( LogTemp , Warning , TEXT ( "no Action :" )  );

			}
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
		this->GetActorForwardVector() * x * 10 +
		this->GetActorRightVector() * y * 10 +
		this->GetActorUpVector() * z * 10
	);
	this->ToLocation = moveToPoint;
}





// Command Funtion

void ACPP_CharacterPaul::Command1()
{
	UE_LOG(LogTemp, Warning, TEXT("Command1 Pressed"));
	this->SetToLocationPoint(-5, 0, 0);
}

void ACPP_CharacterPaul::Command2()
{
	UE_LOG(LogTemp, Warning, TEXT("Command2 Pressed"));
	this->SetToLocationPoint(0, 0, -10);
}

void ACPP_CharacterPaul::Command3()
{
	UE_LOG(LogTemp, Warning, TEXT("Command3 Pressed"));
	this->SetToLocationPoint(5, 0, 0);
}

void ACPP_CharacterPaul::Command4()
{
	UE_LOG(LogTemp, Warning, TEXT("Command4 Pressed"));
	this->SetToLocationPoint(-10, 0, 0);
}

void ACPP_CharacterPaul::Command6()
{
	UE_LOG(LogTemp, Warning, TEXT("Command6 Pressed"));
	this->SetToLocationPoint(10, 0, 0);
}

void ACPP_CharacterPaul::Command7()
{
	UE_LOG(LogTemp, Warning, TEXT("Command7 Pressed"));
	this->SetToLocationPoint(-10, 0, 10);
}

void ACPP_CharacterPaul::Command8()
{
	UE_LOG(LogTemp, Warning, TEXT("Command8 Pressed"));
	this->SetToLocationPoint(0, 0, 10);
}

void ACPP_CharacterPaul::Command9()
{
	UE_LOG(LogTemp, Warning, TEXT("Command9 Pressed"));
	this->SetToLocationPoint(10, 0, 10);

}

