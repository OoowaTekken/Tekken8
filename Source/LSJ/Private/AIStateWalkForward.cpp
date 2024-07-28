// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateWalkForward.h"
#include "AICharacterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"

void UAIStateWalkForward::Enter (UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter(pAnimInstance);
	animInstace = pAnimInstance;
	
	//GetWorld( )->GetFirstPlayerController()
}

void UAIStateWalkForward::Execute ( )
{
	owner->GetMovementComponent()->AddInputVector(owner->GetActorForwardVector()*5.0f);
	Exit ( );
}

void UAIStateWalkForward::Exit ( )
{
	Super::Exit ( );
}
void UAIStateWalkForward::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent ( DeltaTime , TickType , ThisTickFunction );
	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass ( GetWorld ( ) , ACharacter::StaticClass ( ) , outActors );
	FRotator lookPlayerRotator = UKismetMathLibrary::FindLookAtRotation ( owner->GetActorLocation ( ) , outActors[2]->GetActorLocation ( ) );
	owner->SetActorRotation ( lookPlayerRotator );
	UE_LOG ( LogTemp , Warning , TEXT ( "UAIStateWalkForward" ) );
	// ...
}