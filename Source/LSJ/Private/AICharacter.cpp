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
	collisionLH->SetupAttachment(GetMesh(),FName(TEXT("middle_0_l" ) ) );
	//collisionLH->SetAttachSocketName(GetMesh()->)

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
}

// Called every frame
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
