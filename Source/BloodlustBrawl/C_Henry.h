// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BloodlustBrawlCharacter.h"
#include "C_Henry.generated.h"

/**
 * 
 */
UCLASS()
class BLOODLUSTBRAWL_API AC_Henry : public ABloodlustBrawlCharacter
{
	GENERATED_BODY()
	
public:


protected:
	virtual void SpecialAttack(const FInputActionValue& Value) override;
};
