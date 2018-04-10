// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "DestructibleTerrain.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOX_API ADestructibleTerrain : public APaperCharacter
{
	GENERATED_BODY()

		ADestructibleTerrain(const FObjectInitializer& ObjectInitializer);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual void Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);

	void updateSprite();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprites)
		class UPaperFlipbook* NormalStateSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprites)
		class UPaperFlipbook* CrackedStateSprite;

	UPROPERTY(EditDefaultsOnly, Category = "TerrainCondition")
		float Health;

	float InitialHP;

	
};
