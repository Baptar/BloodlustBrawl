// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "C_Cactoro.h"
#include "Components/TimelineComponent.h"

#include "C_SpecialCactus.generated.h"

UCLASS()
class BLOODLUSTBRAWL_API AC_SpecialCactus : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CactusMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CactusCapsule;
	
	
public:	
	// Sets default values for this actor's properties
	AC_SpecialCactus();

protected:

	UPROPERTY(EditAnywhere)
		USoundBase* AppearSound;

	UPROPERTY(EditAnywhere)
		USoundBase* DisappearSound;

	FTimeline Timeline;
	FTimeline TimelineRemove;
	FVector BeforeLoc;

	UPROPERTY(EditAnywhere, Category = "Components")
		UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere, Category = "Components")
		UCurveFloat* CurveFloatRemove;

	UPROPERTY(EditAnywhere)
		float LaunchPower = 1700.0f;

	UFUNCTION()
		void SpawnCactus(float Value);

	UFUNCTION()
		void RemoveCactus(float Value);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Overlap
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

public:	

	AC_Cactoro* CactusChar;
	int CactusNumber = -1;

	void Appear();
	void Disappear();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
