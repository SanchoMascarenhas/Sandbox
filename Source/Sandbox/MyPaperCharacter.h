// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "MyPaperCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOX_API AMyPaperCharacter : public APaperCharacter
{
	GENERATED_BODY()
	//
	
public:

	AMyPaperCharacter(const class FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
		float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
		bool IsAlive() const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition")
		float Health;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition")
		float MaxHealth;

	
	
	
};
