// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleTerrain.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"


ADestructibleTerrain::ADestructibleTerrain(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/**/

	/**/

	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ExplorerBase"));
	}

	// Set the size of our collision capsule.
	//GetCapsuleComponent()->bGenerateOverlapEvents = true;
}

void ADestructibleTerrain::BeginPlay()
{
	GetSprite()->SetFlipbook(NormalStateSprite);
	InitialHP = Health;
}

void ADestructibleTerrain::Tick(float DeltaSeconds)
{
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
			Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
			return ActualDamage;
		}

	}

	updateSprite();
	return ActualDamage;
}

void ADestructibleTerrain::Die(float KillingDamage, FDamageEvent const & DamageEvent, AController * Killer, AActor * DamageCauser)
{
}

void ADestructibleTerrain::updateSprite()
{
	if (Health < InitialHP * 0.5f) {
		GetSprite()->SetSpriteColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

