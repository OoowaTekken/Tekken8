// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateWalkCross.h"
#include "AICharacterAnimInstance.h"
#include "AICharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
void UAIStateWalkCross::Enter ( UAICharacterAnimInstance* pAnimInstance )
{	
	Super::Enter ( pAnimInstance );

	if ( nullptr==owner->aOpponentPlayer )
	{
		return;
	}
	//속도가 처음에 크고 끝에는 작아지게 하면 좋을 것 같다.
	//처음 적정 값 OrbitAroundTarget ( owner->aOpponentPlayer , 100.0f );
	OrbitAroundTarget (330.0f );
	//int randNum = FMath::RandRange(0,1);
	if ( false==bClockwise )
	{
		direction = -1.0f;
		animInstace->PlayCrossWalkCounterclockwiseMontage ( );
	}
		
	else
	{
		direction = 1.0f;
		animInstace->PlayCrossWalkClockwiseMontage ( );
	}
	owner->GetCurrentMontage ( )->GetSectionStartAndEndTime ( 0 ,startFrame,endFrame);
	//animInstace->StateWalkForward(true);

}

void UAIStateWalkCross::Execute ( const float& deltatime )
{
	if ( nullptr == owner||nullptr == owner->aOpponentPlayer)
	{
		return;
	}

	//원의 지름 이동
	MoveStep(deltatime);

	//상대방을 쳐다보자
	FVector opponentPlayerRotator = owner->aOpponentPlayer->GetMesh ( )->GetComponentLocation ( );
	opponentPlayerRotator.Z = owner->GetActorLocation ( ).Z;
	FRotator lookRotator = (opponentPlayerRotator - owner->GetActorLocation()).Rotation ( );
	owner->SetActorRotation ( FMath::RInterpTo ( owner->GetActorRotation ( ) , lookRotator , deltatime , 100.0f ) );
}

void UAIStateWalkCross::Exit ( )
{
	owner->SetStateIdle ( );
	//animInstace->StateWalkForward ( false );
	Super::Exit ( );
	
}

void UAIStateWalkCross::OrbitAroundTarget (float Speed )
{
	if ( !owner->aOpponentPlayer )
	{
		return;
	}
	OrbitSpeed = Speed;
	// 적 캐릭터와 현재 캐릭터 사이의 거리로 반경을 설정

	FVector Center = owner->aOpponentPlayer->GetMesh()->GetComponentLocation();
	Center.Z = owner->GetActorLocation ( ).Z;
	OrbitRadius = FVector::Dist ( Center , owner->GetActorLocation());

	// 현재 위치를 기준으로 초기 각도를 설정
	FVector Direction = (owner->GetActorLocation ( ) - Center).GetSafeNormal ( );
	CurrentAngle = FMath::RadiansToDegrees ( FMath::Atan2 ( Direction.Y , Direction.X ) );
}

void UAIStateWalkCross::MoveStep ( float DeltaTime )
{
	if ( !owner->aOpponentPlayer )
	{
		return;
	}
	//속도가 처음에 크고 끝에는 작아지게 하면 좋을 것 같다.
	OrbitSpeed =FMath::Lerp(OrbitSpeed,0, startFrame+=DeltaTime);

	// 현재 각도를 시간에 따라 업데이트
	CurrentAngle += OrbitSpeed * DeltaTime * direction;
	if ( CurrentAngle > 360.0f )
	{
		CurrentAngle -= 360.0f;
	}

	// 적 캐릭터와 현재 캐릭터 사이의 거리로 반경을 설정
	FVector Center = owner->aOpponentPlayer->GetMesh ( )->GetBoneLocation((TEXT("head")) );
	Center.Z = owner->GetActorLocation ( ).Z;
	float RadAngle = FMath::DegreesToRadians ( CurrentAngle);
	if( OrbitRadius > 115.0f )
		OrbitRadius -= reduceRange * DeltaTime;
	//가장 가까이에서 한번으로 최소 60도 이동

	//위치 테스트값
	FVector OrbitLocation = Center + FVector ( FMath::Cos ( RadAngle )* 115 , FMath::Sin ( RadAngle) * 115 , 0.0f );
	//위치 원래값
	//FVector OrbitLocation = Center + FVector ( FMath::Cos ( RadAngle ) * OrbitRadius , FMath::Sin ( RadAngle ) * OrbitRadius , 0.0f );
	//GEngine->AddOnScreenDebugMessage ( -1 , 5.f , FColor::Red , FString::Printf ( TEXT ( "GetSectionTimeLeftFromPos(0)" ) ,  ) );
	// SetActorLocation을 사용하여 이동
	owner->SetActorLocation ( OrbitLocation );
	DrawDebugLine ( GetWorld ( ) , Center , Center+FVector(0,0,10000.0f) , FColor ( 255 , 0 , 0 ) , false , .1f , 0 , 12.3f );
	//AddMovementInput 거리가 일정하게만 유지되고 여러 조절이 안됨
	//owner->AddMovementInput ( MoveDirection ,1.0f,true);
}