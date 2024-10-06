// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BloodlustBrawlCharacter.h"
#include "C_CaptainBarbarossa.generated.h"

/**
 * 
 */
UCLASS()
class BLOODLUSTBRAWL_API AC_CaptainBarbarossa : public ABloodlustBrawlCharacter
{
	GENERATED_BODY()
	
public:


protected:
	virtual void SpecialAttack(const FInputActionValue& Value) override;
};
