// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "C_BaseCamera.h"

#include "BloodlustBrawlCharacter.generated.h"


UCLASS(config=Game)
class ABloodlustBrawlCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Jump to Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SpecialAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* TriggerComp;

public:
	ABloodlustBrawlCharacter();

	virtual void DeathEvent();
	

protected:

	float Delta;
	bool bCanAttack = true;

	AC_BaseCamera * BaseCam;

	UPROPERTY(EditAnywhere)
		bool bSwordAttack;
	
	UPROPERTY(EditAnywhere)
		UAnimMontage* MeleeAttackAnim;

	UPROPERTY(EditAnywhere)
		UAnimMontage* DeathAnim;

	UPROPERTY(EditAnywhere)
		int PowerAttack = 750;
	
	UPROPERTY(EditAnywhere)
		int MeleeAttackDistanceBase = 45;
	
	UPROPERTY(EditAnywhere)
		float DelayBetweenAttack = 1.0f;

	UPROPERTY(EditAnywhere)
		USoundBase* PloufSound;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);			

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for looking input */
	void StartedLook(const FInputActionValue& Value);

	/** Called for looking input */
	void CompletedLook(const FInputActionValue& Value);

	/** Called for attacking input */
	virtual void MeleeAttack(const FInputActionValue& Value);

	/** Called for attacking input */
	virtual void SpecialAttack(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;	
};

