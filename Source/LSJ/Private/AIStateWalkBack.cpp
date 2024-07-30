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
	lookPlayerRotator = UKismetMathLibrary::FindLookAtRotation ( owner->GetActorLocation ( ) , player->GetActorLocation ( ) );
	animInstace->PlayerWalkBackMontage ( );
}

void UAIStateWalkBack::Execute ( )
{
	lookPlayerRotator = UKismetMathLibrary::FindLookAtRotation ( GetOwner ( )->GetActorLocation ( ) , player->GetActorLocation ( ) );
	owner->GetMovementComponent ( )->AddInputVector ( owner->GetActorForwardVector ( ) * -1.0f );
}

void UAIStateWalkBack::Exit ( )
{
	Super::Exit ( );
}

void UAIStateWalkBack::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent ( DeltaTime , TickType , ThisTickFunction );
	Execute ( );
	owner->SetActorRotation ( FMath::RInterpConstantTo ( owner->GetActorRotation ( ) , lookPlayerRotator , DeltaTime , 200.0f ) );
	//애니메이션 에서 종료
	UE_LOG ( LogTemp , Warning , TEXT ( "UAIStateWalkBack" ) );
	// ...
}
