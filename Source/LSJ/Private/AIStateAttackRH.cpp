// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateAttackRH.h"
#include "AICharacterAnimInstance.h"
#include "CPP_CharacterPaul.h"
#include "AICharacter.h"
#include "Components/SphereComponent.h"
void UAIStateAttackRH::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter(pAnimInstance);
	animInstace->PlayeAttackRHMontage ();
	aiCharacter = Cast<AAICharacter> ( owner );
}

void UAIStateAttackRH::Execute ( const float& deltatime )
{
	//밀쳐내기
	FVector start = aiCharacter->GetActorLocation ( );
	FVector end = aiCharacter->collisionRH->GetComponentLocation ( );
	FHitResult hitResult;
	DrawDebugLine ( GetWorld ( ) , start , end , FColor ( 255 , 0 , 0 ) , false , .1f , 0 , 12.3f );
	if ( GetWorld ( )->LineTraceSingleByObjectType ( hitResult , start , end , FCollisionObjectQueryParams ( EObjectTypeQuery::ObjectTypeQuery1 ) ) )
	{
		if ( hitResult.GetActor ( )->IsA ( ACPP_CharacterPaul::StaticClass ( ) ) )
		{
			auto* player= Cast<ACPP_CharacterPaul>((hitResult.GetActor()));
			//UE_LOG ( LogTemp , Error , TEXT ( "%s" ) , *hitResult.GetActor ( )->GetName ( ) );
			//hitResult.GetActor ( )->SetActorLocation( hitResult.GetActor ( )->GetActorLocation() + 1000.0f*deltatime * aiCharacter->GetActorForwardVector ( ));
		}
	}
}

void UAIStateAttackRH::Exit ( )
{
	Super::Exit ( );
}
