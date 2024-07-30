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
	lookPlayerRotator = UKismetMathLibrary::FindLookAtRotation ( owner->GetActorLocation ( ) , player->GetActorLocation ( ) );
	animInstace->PlayerWalkForwardMontage();
	//GetWorld( )->GetFirstPlayerController()
}

void UAIStateWalkForward::Execute ( )
{
	owner->GetMovementComponent()->AddInputVector(owner->GetActorForwardVector());
}

void UAIStateWalkForward::Exit ( )
{
	Super::Exit ( );
}
void UAIStateWalkForward::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent ( DeltaTime , TickType , ThisTickFunction );
	owner->SetActorRotation ( FMath::RInterpConstantTo ( owner->GetActorRotation ( ) , lookPlayerRotator , DeltaTime , 0.1f ) );
	Execute ( );
	UE_LOG ( LogTemp , Warning , TEXT ( "UAIStateWalkForward" ) );
	// ...
}