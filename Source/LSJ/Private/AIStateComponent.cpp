﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AICharacter.h"
// Sets default values for this component's properties
UAIStateComponent::UAIStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	attackCount = 0;
	// ...
}


// Called when the game starts
void UAIStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UAIStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAIStateComponent::AddAttackCount ( int32 amount )
{
	attackCount += amount;
}

int32 UAIStateComponent::GetAttackCount ( )
{
	return attackCount;
}

void UAIStateComponent::Enter ( class UAICharacterAnimInstance* pAnimInstance )
{
	SetComponentTickEnabled ( true );
	animInstace = pAnimInstance;
}

void UAIStateComponent::Execute ( const float& deltatime )
{

}

void UAIStateComponent::Exit ( )
{
	SetComponentTickEnabled ( false );
	attackCount = 0;
	OnStateComplete.Broadcast ( );
}

void UAIStateComponent::SetStateOwner ( AAICharacter* pOwner )
{
	owner = pOwner;
}
