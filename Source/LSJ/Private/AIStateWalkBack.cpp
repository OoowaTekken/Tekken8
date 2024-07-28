// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateWalkBack.h"
#include "AICharacterAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"



void UAIStateWalkBack::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
}

void UAIStateWalkBack::Execute ( )
{
	owner->GetMovementComponent ( )->AddInputVector ( owner->GetActorForwardVector ( ) * -5.0f );
	Exit ( );
}

void UAIStateWalkBack::Exit ( )
{
	Super::Exit ( );
}

void UAIStateWalkBack::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent ( DeltaTime , TickType , ThisTickFunction );
	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass ( GetWorld ( ) , ACharacter::StaticClass ( ) , outActors );
	FRotator lookPlayerRotator = UKismetMathLibrary::FindLookAtRotation ( owner->GetActorLocation ( ) , outActors[2]->GetActorLocation ( ) );
	owner->SetActorRotation ( lookPlayerRotator );
	UE_LOG ( LogTemp , Warning , TEXT ( "UAIStateWalkBack" ) );
	// ...
}
