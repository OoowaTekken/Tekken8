// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_CharacterPaul.h"
// Sets default values
ACPP_CharacterPaul::ACPP_CharacterPaul()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USkeletalMeshComponent* tempMesh = this->GetMesh();
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple'"));
	if (tempSkeletalMesh.Succeeded())
		tempMesh->SetSkeletalMeshAsset(tempSkeletalMesh.Object);
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

}

// Called to bind functionality to input
void ACPP_CharacterPaul::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

