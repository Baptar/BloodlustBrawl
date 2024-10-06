// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

#include "C_BaseCamera.generated.h"

UCLASS()
class BLOODLUSTBRAWL_API AC_BaseCamera : public AActor
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
public:	
	// Sets default values for this actor's properties
	AC_BaseCamera();

protected:

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray< TSubclassOf<APawn>> MyCharacterArrayClass = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//TSubclassOf<APawn> MyCharacter;

	UPROPERTY(EditAnywhere)
		float MaxDistanceZoom = 800.0f;

	UPROPERTY(EditAnywhere)
		float MinDistanceZoom = 0.0f;

	UPROPERTY(EditAnywhere)
		float YOffset = -1500.0f;

	UPROPERTY(EditAnywhere)
		float XOffset = -1500.0f;

	float Delta;
	float CameraPosY = 0.0f;
	float CameraPosX = 0.0f;
	float CameraPosZ = 0.0f;
	float LongestDistance = 0.0f;
	FVector CenterPosition;

	void GetLongestDistance();
	void SetCenterPoint();
	void CameraZoom() const;
	void SetCameraPosition();


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	TArray<ACharacter*> CharArray;

	UCameraComponent* GetCamera() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
