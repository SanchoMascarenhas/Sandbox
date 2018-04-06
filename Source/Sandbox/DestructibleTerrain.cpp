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
}

void ADestructibleTerrain::Tick(float DeltaSeconds)
{
	GetSprite()->SetFlipbook(NormalStateSprite);
}

