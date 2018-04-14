// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleTerrain.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Components/BoxComponent.h"


// Sets default values
ADestructibleTerrain::ADestructibleTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TerrainBase"));
	}

	activeFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ActiveFlipbook"));
	activeFlipbook->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	collisionBox->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}

// Called when the game starts or when spawned
void ADestructibleTerrain::BeginPlay()
{
	Super::BeginPlay();
	InitialHP = Health;
	activeFlipbook->SetFlipbook(NormalStateSprite);
}

// Called every frame
void ADestructibleTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ADestructibleTerrain::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (Health <= 0.f)
	{
		return 0.f;
	}

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;

		if (Health <= 0)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "health < 0");
			Destroy();
			return ActualDamage;
		}

	}

	updateSprite();
	return ActualDamage;
}

void ADestructibleTerrain::updateSprite()
{
	if (Health <= InitialHP * 0.5f) {
		activeFlipbook->SetFlipbook(CrackedStateSprite);
	}
}

