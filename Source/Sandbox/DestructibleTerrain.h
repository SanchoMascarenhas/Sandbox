// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleTerrain.generated.h"

UCLASS()
class SANDBOX_API ADestructibleTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleTerrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprites")
		class UPaperFlipbookComponent* activeFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprites")
		class UPaperFlipbook* NormalStateSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprites")
		class UPaperFlipbook* CrackedStateSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxComponent")
		class UBoxComponent* collisionBox;

	UPROPERTY(EditDefaultsOnly, Category = "TerrainCondition")
		float Health;

	float InitialHP;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	void updateSprite();

	
};
