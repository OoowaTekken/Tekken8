// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateAttackLF.h"
#include "AICharacterAnimInstance.h"
#include "AICharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CPP_CharacterPaul.h"
void UAIStateAttackLF::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	animInstace->PlayeAttackLFMontage ( );
	aiCharacter =Cast<AAICharacter>(owner); 
}

void UAIStateAttackLF::Execute ( const float& deltatime )
{
	//밀쳐내기
	FVector start = aiCharacter->GetActorLocation();
	FVector end = aiCharacter->collisionLF->GetComponentLocation();
	FHitResult hitResult;
	DrawDebugLine ( GetWorld ( ) , start , end , FColor ( 255 , 0 , 0 ) , false , .1f , 0 , 12.3f );
	if ( GetWorld ( )->LineTraceSingleByObjectType ( hitResult , start , end , FCollisionObjectQueryParams ( EObjectTypeQuery::ObjectTypeQuery1 ) ) )
	{
		if ( hitResult.GetActor ( )->IsA ( ACPP_CharacterPaul::StaticClass ( ) ) )
		{
			//UE_LOG ( LogTemp , Error , TEXT ( "%s" ) , *hitResult.GetActor ( )->GetName ( ) );
			//hitResult.GetActor ( )->SetActorLocation( hitResult.GetActor ( )->GetActorLocation() + 1000.0f*deltatime * aiCharacter->GetActorForwardVector ( ));
		}
	}
	//owner->GetMovementComponent()->AddInputVector(owner->GetActorForwardVector()*deltatime,true);
}

void UAIStateAttackLF::Exit ( )
{
	Super::Exit ( );
}
