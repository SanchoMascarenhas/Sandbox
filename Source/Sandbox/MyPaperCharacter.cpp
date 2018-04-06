// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPaperCharacter.h"

AMyPaperCharacter::AMyPaperCharacter(const class FObjectInitializer& ObjectInitializer)
{
	Health = 100;
	MaxHealth = 100;
}

float AMyPaperCharacter::GetHealth() const
{
	return Health;
}

bool AMyPaperCharacter::IsAlive() const
{
	return Health > 0;
}



