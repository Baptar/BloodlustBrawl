// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BloodlustBrawlCharacter.h"
#include "C_CommonCactus.h"
#include "Components/ChildActorComponent.h"
#include "Components/WidgetComponent.h"

#include "C_Cactoro.generated.h"

/**
 * 
 */
UCLASS()
class BLOODLUSTBRAWL_API AC_Cactoro : public ABloodlustBrawlCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* Child0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* Child1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* Child2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* CactusNumberWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildMunition1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildMunition2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildMunition3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildMunition4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildMunition5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildMunition6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildMunition7;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildMunition8;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildMunition9;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildMunition10;
	
public:

	AC_Cactoro();
	
	TArray<UChildActorComponent*> CactusChildArray;
	TArray<UChildActorComponent*> CactusMunitionChildArray;
    
    TArray<bool> bIsAvailableArray = { true, true, true };
    
	UFUNCTION(BlueprintCallable)
	int CactusNumberAvailable();

protected:

	//Cactus Class 
	UPROPERTY(EditAnywhere, Category = "Components")
		TSubclassOf<AActor> CactusActor;

	UPROPERTY(EditAnywhere, Category = "Components")
		TSubclassOf<AActor> CactusMunitionActor;
	
	virtual void SpecialAttack(const FInputActionValue& Value) override;
	virtual void MeleeAttack(const FInputActionValue& Value) override;
	virtual void DeathEvent() override;
	virtual void BeginPlay() override;
	
};
