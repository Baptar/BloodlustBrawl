// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "C_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLOODLUSTBRAWL_API UC_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UC_GameInstance();

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<int> PlayerCharacterArray;

	UPROPERTY(BlueprintReadWrite)
		UAudioComponent* MySound;

	UFUNCTION(BlueprintCallable)
		void PlayMyMusicSound();

	int GetPlayerNumber();

private:

	UPROPERTY(EditAnywhere)
		USoundBase* MyStartMusic;

};
