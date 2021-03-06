// Fill out your copyright notice in the Description page of Project Settings.

#include "Explorer.h"
#include "Engine.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "DestructibleTerrain.h"

enum Actions { IDLE, WALK, JUMP, CROUCH, FALLING, POWERDROP};

AExplorer::AExplorer(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> WalkingAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> KickAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> SmashAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> JumpAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> FallingAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> CrouchAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> PowerdropAnimationAsset;
		FConstructorStatics() :
			WalkingAnimationAsset(TEXT("/Game/Sprite/Player/Flipbooks/Walk")),
			IdleAnimationAsset(TEXT("/Game/Sprite/Player/Flipbooks/Idle")),
			KickAnimationAsset(TEXT("/Game/Sprite/Player/Flipbooks/Kick")),
			SmashAnimationAsset(TEXT("/Game/Sprite/Player/Flipbooks/Smash")),
			JumpAnimationAsset(TEXT("/Game/Sprite/Player/Flipbooks/Jump")),
			FallingAnimationAsset(TEXT("/Game/Sprite/Player/Flipbooks/Falling")),
			CrouchAnimationAsset(TEXT("/Game/Sprite/Player/Flipbooks/Crouch")),
			PowerdropAnimationAsset(TEXT("/Game/Sprite/Player/Flipbooks/Powerdrop"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	WalkingAnimation = ConstructorStatics.WalkingAnimationAsset.Get();
	IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();
	KickAnimation = ConstructorStatics.KickAnimationAsset.Get();
	SmashAnimation = ConstructorStatics.SmashAnimationAsset.Get();
	JumpAnimation = ConstructorStatics.JumpAnimationAsset.Get();
	FallingAnimation = ConstructorStatics.FallingAnimationAsset.Get();
	CrouchAnimation = ConstructorStatics.CrouchAnimationAsset.Get();
	PowerdropAnimation = ConstructorStatics.PowerdropAnimationAsset.Get();


	GetSprite()->SetFlipbook(IdleAnimation);
	spriteRotation = FRotator(0.f, 0.f, 0.f).Quaternion();
	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ExplorerBase"));
	}

	// Set the size of our collision capsule.
	GetCapsuleComponent()->bGenerateOverlapEvents = true;
	GetCapsuleComponent()->SetCapsuleHalfHeight(48.0f);
	GetCapsuleComponent()->SetCapsuleRadius(27.0f);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AExplorer::OnHit);

	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	GetCharacterMovement()->GravityScale = 2.5f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 100.0f;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->ProjectionMode = ECameraProjectionMode::Perspective;
	CameraComponent->FieldOfView = 90.0f;
	CameraComponent->RelativeLocation = FVector(0.0f, 1000.0f, 0.0f);
	CameraComponent->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	CameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	ANIMATION = IDLE;
	isJumping = isCrouching = isPowerDroping = false;
	PowerDropDamage = 25.0f;
}

void AExplorer::BeginPlay()
{
	Super::BeginPlay();
}

void AExplorer::Tick(float DeltaSeconds)
{
	
	Super::Tick(DeltaSeconds);
	UpdateAnimation();
}

void AExplorer::Movement(float Value)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	if (!(this->GetCharacterMovement()->IsFalling() || isJumping)) {
		if (Value < 0) {
			spriteRotation = FRotator(0.f, 180.f, 0.f).Quaternion();
			ANIMATION = WALK;
		}
		else if (Value > 0) {
			spriteRotation = FRotator(0.f, 0.f, 0.f).Quaternion();
			ANIMATION = WALK;
		}
		else if (isCrouching) {
			ANIMATION = CROUCH;
		}
		else {
			ANIMATION = IDLE;
		}
		return;
	}
	if (this->GetCharacterMovement()->IsFalling()) {
		if (isPowerDroping)
			ANIMATION = POWERDROP;
		else
			ANIMATION = JUMP;
	}
	
}


void AExplorer::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	InputComponent->BindAction("Jump", IE_Pressed, this, &AExplorer::Jump);
	InputComponent->BindAction("Jump", IE_Repeat, this, &AExplorer::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AExplorer::StopJumping);
	InputComponent->BindAction("Crouch/PowerDrop", IE_Pressed, this, &AExplorer::Crouching);
	InputComponent->BindAction("Crouch/PowerDrop", IE_Released, this, &AExplorer::StopCrouching);
	InputComponent->BindAxis("HorizontalMovement", this, &AExplorer::Movement);
}

void AExplorer::UpdateAnimation()
{

	switch (ANIMATION) {
	case JUMP:
		this->GetSprite()->SetFlipbook(JumpAnimation);
		break;
	case IDLE:
		this->GetSprite()->SetFlipbook(IdleAnimation);
		break;
	case WALK:
		this->GetSprite()->SetFlipbook(WalkingAnimation);
		break; 
	case CROUCH:
		this->GetSprite()->SetFlipbook(CrouchAnimation);
		break;
	case POWERDROP:
		this->GetSprite()->SetFlipbook(PowerdropAnimation);;
		break;
	}

	GetSprite()->SetRelativeRotation(spriteRotation);

}

void AExplorer::Crouching()
{	
	if (this->GetCharacterMovement()->IsFalling()) {
		//PerformPowerDrop();
		isPowerDroping = true;
	}
	else {
		isCrouching = true;
		this->Crouch();
	}
}

void AExplorer::StopCrouching()
{
	if (isJumping) {
		ANIMATION = JUMP;
	}
	/**/
	isCrouching = false;
	isPowerDroping = false;
	this->UnCrouch();
	/**/
}

void AExplorer::Jump() {
	Super::Jump();
	ANIMATION = JUMP;
	isJumping = true;
}

void AExplorer::StopJumping() {
	Super::StopJumping();
	isJumping = false;
}


void AExplorer::PerformPowerDrop()
{

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Powerdrop");

	TArray<AActor*> Overlaps;
	this->GetCapsuleComponent()->GetOverlappingActors(Overlaps, AActor::StaticClass());
	for (int32 i = 0; i < Overlaps.Num(); i++)
	{
		if (Overlaps[i] && Overlaps[i] != this)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Overlaps[i]->GetActorLabel());
			FPointDamageEvent DmgEvent;
			DmgEvent.Damage = PowerDropDamage;

			Overlaps[i]->TakeDamage(DmgEvent.Damage, DmgEvent, GetController(), this);
		}
	}

}

void AExplorer::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "hit");
	ADestructibleTerrain* land = Cast<ADestructibleTerrain>(OtherActor);
	if(isPowerDroping)
		if (land) {
			FPointDamageEvent DmgEvent;
			DmgEvent.Damage = PowerDropDamage;

			land->TakeDamage(DmgEvent.Damage, DmgEvent, GetController(), this);
		}
}

