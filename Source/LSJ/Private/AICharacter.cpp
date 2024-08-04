// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AICharacterController.h"
#include "AICharacterAnimInstance.h"
#include "AIStateBackDash.h"
#include "AIStateRun.h"
#include "AIStateWalkBack.h"
#include "AIStateWalkForward.h"
#include "AIStateIdle.h"
#include "Components/CapsuleComponent.h"
#include "AIStateAttackLF.h"
#include "AIStateAttackRH.h"
#include "Components/SphereComponent.h"
#include "AIStateHit.h"
#include "AIStateComboLaserAttack.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "AIStateHitFalling.h"


// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> meshFinder(TEXT("/Script/Engine.SkeletalMesh'/Game/Jaebin/Kazuya/T-Pose_Final/T-Pose2_UE.T-Pose2_UE'" ) );
	if ( meshFinder.Succeeded ( ) )
	{
		GetMesh ( )->SetSkeletalMesh (meshFinder.Object );
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> animFinder ( TEXT ( "/Script/Engine.AnimBlueprint'/Game/LSJ/Blueprint/ABP_AICharacter3.ABP_AICharacter3_C'" ) );
	if ( animFinder.Succeeded ( ) )
	{
		GetMesh ( )->SetAnimInstanceClass ( animFinder.Class );
	}
	GetMesh ( )->SetRelativeScale3D ( FVector ( 0.1f , 0.1f , 0.1f ) );
	GetMesh( )->SetRelativeLocation(FVector(0,0,-80.f));
	GetMesh ( )->SetRelativeRotation ( FRotator (  0, -90.f , 0 ) );

	collisionLH = CreateDefaultSubobject<USphereComponent> ( TEXT ( "collisionLH" ) );
	collisionRH = CreateDefaultSubobject<USphereComponent> ( TEXT ( "collisionRH" ) );
	collisionLF = CreateDefaultSubobject<USphereComponent> ( TEXT ( "collisionLF" ) );
	collisionRF = CreateDefaultSubobject<USphereComponent> ( TEXT ( "collisionRF" ) );
	collisionLH->SetupAttachment(GetMesh(),TEXT("middle_02_l" ));
	collisionLH->SetSphereRadius(92.f);
	collisionLH->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	collisionRH->SetupAttachment ( GetMesh ( ) ,TEXT ( "middle_02_r" ));
	collisionRH->SetSphereRadius ( 92.f );
	collisionRH->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	collisionLF->SetupAttachment ( GetMesh ( ) , TEXT ( "ball_l" ) );
	collisionLF->SetSphereRadius ( 92.f );
	collisionLF->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	collisionRF->SetupAttachment ( GetMesh ( ) , TEXT ( "ball_r" ) );
	collisionRF->SetSphereRadius ( 92.f );
	collisionRF->SetCollisionEnabled ( ECollisionEnabled::NoCollision );

	//몸체 콜리전
	//collisionTop = CreateDefaultSubobject<UBoxComponent> ( TEXT ( "collisionTop" ) );
	//collisionMiddle = CreateDefaultSubobject<UBoxComponent> ( TEXT ( "collisionMiddle" ) );
	//collisionLower = CreateDefaultSubobject<UBoxComponent> ( TEXT ( "collisionLower" ) );
	//collisionTop->SetupAttachment ( GetMesh ( ) , TEXT ( "head" ) );
	//collisionTop->SetBoxExtent ( FVector (311.232643 ,166.634513 , 240.066655) );
	//collisionMiddle->SetupAttachment ( GetMesh ( ) , TEXT ( "spine_01" ) );
	//collisionMiddle->SetBoxExtent ( (FVector ( 311.232643 ,  315.176916 ,  240.066655)) );
	//collisionLower->SetupAttachment ( GetMesh ( ) , TEXT ( "root" ) );
	//collisionLower->SetBoxExtent ( FVector (311.232643 , 404.006705 ,  330.919514) );
	//collisionLower->SetRelativeLocation(FVector( 0.000000 ,199.999997 ,299.999996));
	//콜리전 설정
	//플레이어랑만 충돌

	stateBackDash = CreateDefaultSubobject<UAIStateBackDash>(TEXT("stateBackDash"));
	stateBackDash->SetStateOwner(this);
	stateRun = CreateDefaultSubobject<UAIStateRun> ( TEXT ( "stateRun" ) );
	stateRun->SetStateOwner ( this );
	stateWalkBack = CreateDefaultSubobject<UAIStateWalkBack> ( TEXT ( "stateWalkBack" ) );
	stateWalkBack->SetStateOwner ( this );
	stateWalkForward = CreateDefaultSubobject<UAIStateWalkForward> ( TEXT ( "stateWalkForward" ) );
	stateWalkForward->SetStateOwner ( this );
	stateIdle = CreateDefaultSubobject<UAIStateIdle> ( TEXT ( "stateIdle" ) );
	stateIdle->SetStateOwner ( this );
	stateAttackLF = CreateDefaultSubobject<UAIStateAttackLF> ( TEXT ( "stateAttackLF" ) );
	stateAttackLF->SetStateOwner ( this );
	stateAttackRH = CreateDefaultSubobject<UAIStateAttackRH> ( TEXT ( "stateAttackRH" ) );
	stateAttackRH->SetStateOwner ( this );
	stateHit=CreateDefaultSubobject<UAIStateHit> ( TEXT ( "stateHit" ) );
	stateHit->SetStateOwner ( this );
	stateHitFalling = CreateDefaultSubobject<UAIStateHitFalling> ( TEXT ( "stateHitFalling" ) );
	stateHitFalling->SetStateOwner ( this );
	stateComboLaserAttack = CreateDefaultSubobject<UAIStateComboLaserAttack> ( TEXT ( "stateComboLaserAttack" ));
	FAttackInfoInteraction attack1;
	attack1.KnockBackDirection = FVector (250.f,0.f,0.f); //-0.5 뒤로 밀려난다 5*50 = 250.0f
	attack1.DamageAmount = 10;
	attack1.DamagePoint = EDamagePointInteraction::Middle;
	attack1.HitFrame=23;
	attack1.RetrieveFrame = 43;
	attack1.OwnerGuardFrame = -12;
	attack1.OppositeHitFrame = 101;
	attack1.OppositeCounterFrame = 101;
	FAttackInfoInteraction attack2;
	attack2.KnockBackDirection = FVector ( 110.f * 50.f , 0.f , 10.f * 50.f); //-11.f , 0.f , 10.f 
	attack2.DamageAmount = 12;
	attack2.DamagePoint = EDamagePointInteraction::Middle;
	attack2.HitFrame = 49;
	attack2.RetrieveFrame = 75;
	attack2.OwnerGuardFrame = -12;
	attack2.OppositeHitFrame = 101;
	attack2.OppositeCounterFrame = 101;
	stateComboLaserAttack->attackInfoArray.Add(attack1);
	stateComboLaserAttack->attackInfoArray.Add(attack2);
	stateComboLaserAttack->SetStateOwner ( this );
	
	//중력적용
	//GetCharacterMovement()->bApplyGravityWhileJumping = true;

	AIControllerClass = AAICharacterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	stateWalkForward->SetComponentTickEnabled ( false );
	stateWalkBack->SetComponentTickEnabled ( false );
	animInstance = Cast<UAICharacterAnimInstance> (GetMesh()->GetAnimInstance());
	if ( animInstance )
	{
		
		//animInstance->OnMontageEnded.AddDynamic ( this , &AAICharacter::HandleOnMontageEnded );
	}
	
	if ( IsPlayer1 )
		GetCapsuleComponent ( )->SetCollisionProfileName ( FName ( TEXT ( "Player1Capsule" ) ) );
	else
		GetCapsuleComponent ( )->SetCollisionProfileName ( FName ( TEXT ( "Player2Capsule" ) ) );
	//GetRootComponent()->SetCollisionName
	//FTimerHandle handle;
	//GetWorld ( )->GetTimerManager ( ).SetTimer ( handle ,FTimerDelegate::CreateLambda ([this]() {
	//	ChangeState ( stateWalkBack );
	//	}) , 10.0f ,false);
	collisionLF->OnComponentBeginOverlap.AddDynamic ( this , &AAICharacter::OnCollisionLFBeginOverlap );
	collisionRF->OnComponentBeginOverlap.AddDynamic ( this , &AAICharacter::OnCollisionRFBeginOverlap );
	collisionLH->OnComponentBeginOverlap.AddDynamic ( this , &AAICharacter::OnCollisionLHBeginOverlap );
	collisionRH->OnComponentBeginOverlap.AddDynamic ( this , &AAICharacter::OnCollisionRHBeginOverlap );
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic ( this , &AAICharacter::OnHitBeginOverlap );
	
	// Blackboard 컴포넌트에 접근하여 값을 설정
	aiController = Cast<AAICharacterController> ( GetController ( ) );
	check(aiController);
	//카메라 좌측인지 오른쪽인지 체크
	float cameraDirection = FVector::DotProduct ( UGameplayStatics::GetPlayerPawn ( GetWorld ( ) , 0 )->GetActorRightVector ( ) , GetActorLocation() );
	if ( cameraDirection > 0 )
	{
		aiController->SetBehaviorTree(1);
	}
	else
	{
		aiController->SetBehaviorTree(2);
	}
	blackboardComp = aiController->GetBlackboardComponent ( );
	check (blackboardComp);
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateState( DeltaTime );
	
	//회전
	if ( bLookTarget )
	{
		targetRotator.Roll = GetActorRotation ( ).Roll;
		targetRotator.Pitch = GetActorRotation ( ).Pitch;
		SetActorRotation ( FMath::RInterpTo ( GetActorRotation ( ) , targetRotator , DeltaTime , 50.0f ) );
		if ( bLookTarget && FMath::Abs ( targetRotator.Yaw - GetActorRotation ( ).Yaw ) < 0.1 )
			bLookTarget = false;
	}

	if ( GetCharacterMovement ( )->IsFalling ( ) )
	{
		FVector Gravity = FVector ( 0 , 0 , -980 ); // 기본 중력 값
		AddMovementInput ( Gravity * DeltaTime );
	}
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::ChangeState ( IAIStateInterface* NewState )
{
	//hit 했을때 필요할거 같다.
	//if ( NewState == stateHit && currentState != stateHit ) {
	//	currentState->Exit ( );
	//}

	currentState = NewState;

	if ( currentState ) {
		currentState->Enter (animInstance);
	}
}

void AAICharacter::UpdateState(const float& deltatime)
{
	if ( currentState )
		currentState->Execute ( deltatime );
}

void AAICharacter::ExitCurrentState ( ECharacterStateInteraction state)
{
	if ( currentState ) {
		currentState->Exit ( );
	}
	if( state == ECharacterStateInteraction::HitFalling )
		ChangeState(stateIdle);
}

void AAICharacter::OnAttackCollisionLF ( )
{
	collisionLF->SetCollisionEnabled ( ECollisionEnabled::QueryOnly);
}

void AAICharacter::OnAttackCollisionRF ( )
{
	collisionRF->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
}

void AAICharacter::OnAttackCollisionLH ( )
{
	collisionLH->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
	//float radius = 20.0f;
	//TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	//traceObjectTypes.Add ( UEngineTypes::ConvertToObjectType ( ECollisionChannel::ECC_Pawn ) );
	//TArray<AActor*> ignoreActors;
	//ignoreActors.Init ( this , 1 );
	//TArray<AActor*> outActors;
	//FVector sphereSpawnLocation = collisionLH->GetComponentLocation();
	//UClass* seekClass = ACPP_Tekken8CharacterParent::StaticClass ( );
	//bool hit = UKismetSystemLibrary::SphereOverlapActors ( GetWorld ( ) , sphereSpawnLocation , radius , traceObjectTypes , seekClass , ignoreActors , outActors );;
	//if ( hit )
	//{
	//	for ( AActor* hitActor : outActors )
	//	{
	//		if ( hitActor->IsA<ACPP_Tekken8CharacterParent> ( ) )
	//		{
	//			ACPP_Tekken8CharacterParent* hitCharacter = Cast<ACPP_Tekken8CharacterParent> ( hitActor );
	//			//hitCharacter->HitDecision ( SendAttackInfo ( ) , this );
	//			//DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
	//			//if ( hitCharacter->HitDecision ( SendAttackInfo () , this ) )
	//	/*			sFrameStatus.FrameBlockUsing = attackInfo.OwnerGuardFrame;
	//			else
	//				sFrameStatus.FrameBlockUsing = attackInfo.OwnerGuardFrame;*/
	//		}
	//	}
	//}
}

void AAICharacter::OnAttackCollisionRH ( )
{
	collisionRH->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
	//float radius = 20.0f;
	//TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	//traceObjectTypes.Add ( UEngineTypes::ConvertToObjectType ( ECollisionChannel::ECC_Pawn ) );
	//TArray<AActor*> ignoreActors;
	//ignoreActors.Init ( this , 1 );
	//TArray<AActor*> outActors;
	//FVector sphereSpawnLocation = collisionRH->GetComponentLocation ( );
	//UClass* seekClass = ACPP_Tekken8CharacterParent::StaticClass ( );
	//bool hit = UKismetSystemLibrary::SphereOverlapActors ( GetWorld ( ) , sphereSpawnLocation , radius , traceObjectTypes , seekClass , ignoreActors , outActors );;
	//if ( hit )
	//{
	//	for ( AActor* hitActor : outActors )
	//	{
	//		if ( hitActor->IsA<ACPP_Tekken8CharacterParent> ( ) )
	//		{
	//			ACPP_Tekken8CharacterParent* hitCharacter = Cast<ACPP_Tekken8CharacterParent> ( hitActor );
	//			//hitCharacter->HitDecision ( SendAttackInfo ( ) , this );
	//			//if ( hitCharacter->HitDecision ( SendAttackInfo () , this ) )
	//	/*			sFrameStatus.FrameBlockUsing = attackInfo.OwnerGuardFrame;
	//			else
	//				sFrameStatus.FrameBlockUsing = attackInfo.OwnerGuardFrame;*/
	//		}
	//	}
	//}
}

void AAICharacter::OffAttackCollisionLF ( )
{
	collisionLF->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	currentState->AddAttackCount ( 1 );
	IsAttacked = false;
}

void AAICharacter::OffAttackCollisionRF ( )
{
	collisionRF->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	currentState->AddAttackCount ( 1 );
	IsAttacked = false;
}

void AAICharacter::OffAttackCollisionLH ( )
{
	collisionLH->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	currentState->AddAttackCount ( 1 );
	IsAttacked = false;
}

void AAICharacter::OffAttackCollisionRH ( )
{
	collisionRH->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	currentState->AddAttackCount(1);
	IsAttacked = false;
}

void AAICharacter::OnHitBeginOverlap ( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	//if ( SweepResult.GetActor ( ) == aOpponentPlayer )
	//{
	//	aOpponentPlayer->HitDecision ( SendAttackInfo ( ) , this );
	//	DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
	//	IsAttacked = true;
	//}
}

void AAICharacter::OnCollisionLHBeginOverlap ( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if ( IsAttacked )
		return;
	if (SweepResult.GetActor() == aOpponentPlayer)
	{
		aOpponentPlayer->HitDecision(SendAttackInfo(),this);
		DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
		IsAttacked = true;
	}

	//DrawDebugSphere(GetWorld(),SweepResult.ImpactPoint,92.0f,2,FColor::Blue,false,5.f);
}

void AAICharacter::OnCollisionRHBeginOverlap ( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if ( IsAttacked )
		return;
	if ( SweepResult.GetActor ( ) == aOpponentPlayer )
	{
		aOpponentPlayer->HitDecision ( SendAttackInfo ( ) , this );
		DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
		IsAttacked = true;
	}
	//카메라가 제자리로 안돌아간다
	//float InZoomAmount 어떤 값을 줘도 줌확대가 된다.
	// 확대할 위치 , 줌 정도 0.5 기본 , 흔들림정도 , 흔들림 시간
		//aiCharacter->aMainCamera->RequestZoomEffect(aiCharacter->GetActorLocation(),0.5f,1.0f,3.0f);
	//aMainCamera->RequestZoomEffect ( GetActorLocation ( ) , 0.5f , 100.0f , 0.5f );
	//DrawDebugSphere(GetWorld(),SweepResult.ImpactPoint,92.0f,2,FColor::Blue,false,5.f);
}

void AAICharacter::OnCollisionRFBeginOverlap ( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if ( IsAttacked )
		return;
	if ( SweepResult.GetActor ( ) == aOpponentPlayer )
	{
		aOpponentPlayer->HitDecision ( SendAttackInfo ( ) , this );
		DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
		IsAttacked = true;
	}
}

void AAICharacter::OnCollisionLFBeginOverlap ( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if ( IsAttacked )
		return;
	if ( SweepResult.GetActor ( ) == aOpponentPlayer )
	{
		aOpponentPlayer->HitDecision ( SendAttackInfo ( ) , this );
		DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
		IsAttacked = true;
	}
}
//공격 받았을 때
bool AAICharacter::HitDecision ( FAttackInfoInteraction attackInfo , ACPP_Tekken8CharacterParent* ownerHitPlayer )
{
	//현재 상태와 공격정보를 확인해서 데미지 처리
	//공격 받았다면 Hit State 상태 처리 attackInfo를 전달 return true;
	//가드라면 ... return false;
	//attackInfo
	if ( blackboardComp )
	{
		ExitCurrentState ( ECharacterStateInteraction::HitGround );
		if ( attackInfo.KnockBackDirection.Z > 0 || currentState==stateHitFalling)
		{
			stateHitFalling->SetAttackInfo ( attackInfo );
			blackboardComp->SetValueAsBool ( TEXT ( "IsHitFalling" ) , true ); // 원하는 값을 설정
		}
		else
		{
			stateHit->SetAttackInfo ( attackInfo );
			blackboardComp->SetValueAsBool ( TEXT ( "IsHit" ) , true ); // 원하는 값을 설정
		}
		
		OnHit.Broadcast ( );
	}

	//ChangeState( stateHit );
	return true;
}

FAttackInfoInteraction AAICharacter::SendAttackInfo ( )
{
	auto* state = Cast<UAIStateComponent>(currentState);
	// 적절한 상태 클래스에 따라 상태 변경
	if ( state->IsA(UAIStateAttackLF::StaticClass()))
	{

	}
	else if ( state->IsA ( UAIStateAttackRH::StaticClass()))
	{

	}
	else if( state->IsA ( UAIStateComboLaserAttack::StaticClass()))
	{
	
	}
	return state->attackInfoArray[state->GetAttackCount ( )];
}

void AAICharacter::CurrentAttackCountToZero ( )
{
	currentState->AddAttackCount(-1*currentState->GetAttackCount());
}

void AAICharacter::LookTarget (const float& deltaTime , FRotator& rotator )
{
	SetActorRotation ( FMath::RInterpConstantTo ( GetActorRotation ( ) , rotator , deltaTime , 200.0f ) );
}
