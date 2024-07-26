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

int32 ACPP_CharacterPaul::GetCurrInputKey()
{
	int32 KeyValue = 
		this->CurrInputKey.bKey1		* 0b0000000000000001 +
		this->CurrInputKey.bKey2		* 0b0000000000000010 +
		this->CurrInputKey.bKey3		* 0b0000000000000100 +
		this->CurrInputKey.bKey4		* 0b0000000000001000 +
		//this->CurrInputKey.bKey5		* 0b0000000000010000 +
		this->CurrInputKey.bKey6		* 0b0000000000100000 +
		this->CurrInputKey.bKey7		* 0b0000000001000000 +
		this->CurrInputKey.bKey8		* 0b0000000010000000 +
		this->CurrInputKey.bKey9		* 0b0000000100000000 +
		this->CurrInputKey.bLeftArm		* 0b0000001000000000 +
		this->CurrInputKey.bRightArm	* 0b0000010000000000 +
		this->CurrInputKey.bLeftKick	* 0b0000100000000000 +
		this->CurrInputKey.bRightKick	* 0b0001000000000000;

	return KeyValue;
}

bool ACPP_CharacterPaul::SetCurrInputKey(int32 value)
{
	this->CurrInputKey.bKey1		= value & 0b0000000000000001;
	this->CurrInputKey.bKey2		= value & 0b0000000000000010;
	this->CurrInputKey.bKey3		= value & 0b0000000000000100;
	this->CurrInputKey.bKey4		= value & 0b0000000000001000;
	//this->CurrInputKey.bKey5		= value & 0b0000000000010000;
	this->CurrInputKey.bKey6		= value & 0b0000000000100000;
	this->CurrInputKey.bKey7		= value & 0b0000000001000000;
	this->CurrInputKey.bKey8		= value & 0b0000000010000000;
	this->CurrInputKey.bKey9		= value & 0b0000000100000000;
	this->CurrInputKey.bLeftArm		= value & 0b0000001000000000;
	this->CurrInputKey.bRightArm	= value & 0b0000010000000000;
	this->CurrInputKey.bLeftKick	= value & 0b0000100000000000;
	this->CurrInputKey.bRightKick	= value & 0b0001000000000000;

	return true;
}

int32 ACPP_CharacterPaul::ShowInputKey(int ArrowKey, bool LeftArm, bool RightArm, bool LeftKick, bool RightKick)
{
	int32 inputValue = 1 << ArrowKey;
	
	inputValue = (ArrowKey ? 1 << (ArrowKey - 1) : 0) + 
	this->CurrInputKey.bLeftArm * 0b0000001000000000 +
	this->CurrInputKey.bRightArm * 0b0000010000000000 +
	this->CurrInputKey.bLeftKick * 0b0000100000000000 +
	this->CurrInputKey.bRightKick * 0b0001000000000000;
	return int32();
}


void ACPP_CharacterPaul::SettingCommandTree()
{
	//this->mBaseCommandTree.Add(ShowInputKey(1, false, false, false, false), CreatCommandTree(1,1,1, ));

	this->mBaseCommandTree[ShowInputKey(1, false, false, false, false)];
}


FCommandTree* ACPP_CharacterPaul::CreatCommandTree(int32 timingStart, int32 timingEnd, int32 timingAction, void (* fptr)())
{
	FCommandTree *NewCommand = new FCommandTree();
	
	//NewCommand->PrevTrees;
	//NewCommand->NextTrees;
	NewCommand->timingStart = timingStart;
	NewCommand->timingEnd = timingEnd;
	NewCommand->timingAction = timingAction;
	NewCommand->ActionFunction = fptr;
	NewCommand->ActionFunction();
	return NewCommand;
}

// Called when the game starts or when spawned
void ACPP_CharacterPaul::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACPP_CharacterPaul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 주의 모든 작업은 FrameSystem에서 하는걸 권장합니다.
	
	if (this->fFrameTime < this->fCurrTimeForFrame)
	{
		this->fCurrTimeForFrame = 0;
		this->FrameSystem();
	}
	
}

// Called to bind functionality to input
void ACPP_CharacterPaul::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACPP_CharacterPaul::FrameSystem()
{
	
}

