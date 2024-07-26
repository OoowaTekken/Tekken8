// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_InputControl.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

// Sets default values
ACPP_InputControl::ACPP_InputControl()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_InputControl::BeginPlay()
{
	Super::BeginPlay();

// 	APlayerController* tempPlayerController = Cast<APlayerController> ( this->Controller );
// 
// 	if ( tempPlayerController )
// 	{
// 		UEnhancedInputLocalPlayerSubsystem* tempSubSys
// 			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem> ( tempPlayerController->GetLocalPlayer( ) );
// 		if ( tempSubSys )
// 			tempSubSys->AddMappingContext ( uIMC_Player , 0 );
// 	}
}

// Called every frame
void ACPP_InputControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_InputControl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


// 	UEnhancedInputComponent* enhancedInputComp = Cast<UEnhancedInputComponent> ( PlayerInputComponent );
// 
// 	if ( enhancedInputComp )
// 	{
// 		// Player 1
// 		enhancedInputComp->BindAction ( this->IA_CharaterMove , ETriggerEvent::Started , this , &ACPP_InputControl::OnMyActionMove );
// 
// 	}
}

