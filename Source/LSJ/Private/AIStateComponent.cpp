// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "CPP_CharacterPaul.h"
// Sets default values for this component's properties
UAIStateComponent::UAIStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAIStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
}


// Called every frame
void UAIStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAIStateComponent::Enter ( class UAICharacterAnimInstance* pAnimInstance )
{
	SetComponentTickEnabled ( true );
	animInstace = pAnimInstance;
	player = Cast<ACharacter>(UGameplayStatics::GetActorOfClass ( GetWorld ( ) , ACPP_CharacterPaul::StaticClass ( ) ));
}

void UAIStateComponent::Execute ( const float& deltatime )
{

}

void UAIStateComponent::Exit ( )
{
	SetComponentTickEnabled ( false );
	//if( OnStateComplete .IsBound())
	OnStateComplete.Broadcast ( );
}

void UAIStateComponent::SetStateOwner (ACharacter* pOwner )
{
	owner = pOwner;
}
