// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BloodlustBrawlCharacter.h"
#include "C_FinnTheFrog.generated.h"

/**
 * 
 */
UCLASS()
class BLOODLUSTBRAWL_API AC_FinnTheFrog : public ABloodlustBrawlCharacter
{
	GENERATED_BODY()
	
public: 


protected:
	
	UPROPERTY(EditAnywhere)
		float SphereRadius = 400.0f;

	virtual void SpecialAttack(const FInputActionValue& Value) override ;
};
