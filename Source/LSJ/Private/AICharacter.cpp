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
	}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateState( DeltaTime );
	
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::ChangeState ( IAIStateInterface* NewState )
{
	//hit 했을때 필요할거 같다.
	//if ( currentState) {
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

void AAICharacter::ExitCurrentState ( )
{
	if ( currentState ) {
		currentState->Exit ( );
	}
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
}

void AAICharacter::OnAttackCollisionRH ( )
{
	collisionRH->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
}

void AAICharacter::OffAttackCollisionLF ( )
{
	collisionLF->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	IsAttacked = false;
}

void AAICharacter::OffAttackCollisionRF ( )
{
	collisionRF->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	IsAttacked = false;
}

void AAICharacter::OffAttackCollisionLH ( )
{
	collisionLH->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	IsAttacked = false;
}

void AAICharacter::OffAttackCollisionRH ( )
{
	collisionRH->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	IsAttacked = false;
}

void AAICharacter::OnCollisionLHBeginOverlap ( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if ( IsAttacked )
		return;
	//DrawDebugSphere(GetWorld(),SweepResult.ImpactPoint,92.0f,2,FColor::Blue,false,5.f);
	DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 ,  0.5f);
	IsAttacked = true;
}

void AAICharacter::OnCollisionRHBeginOverlap ( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if ( IsAttacked )
		return;
	//DrawDebugSphere(GetWorld(),SweepResult.ImpactPoint,92.0f,2,FColor::Blue,false,5.f);
	DrawDebugSphere ( GetWorld ( ) , collisionRH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
	IsAttacked = true;
}

void AAICharacter::OnCollisionRFBeginOverlap ( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if ( IsAttacked )
		return;
	//DrawDebugSphere(GetWorld(),SweepResult.ImpactPoint,92.0f,2,FColor::Blue,false,5.f);
	DrawDebugSphere ( GetWorld ( ) , collisionRF->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
	IsAttacked = true;
}

void AAICharacter::OnCollisionLFBeginOverlap ( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if ( IsAttacked )
		return;
	//DrawDebugSphere(GetWorld(),SweepResult.ImpactPoint,92.0f,2,FColor::Blue,false,5.f);
	DrawDebugSphere ( GetWorld ( ) , collisionLF->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
	IsAttacked = true;
}
