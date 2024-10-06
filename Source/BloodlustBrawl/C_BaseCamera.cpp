// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BaseCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "C_GameInstance.h"

// Sets default values
AC_BaseCamera::AC_BaseCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));

	CameraBoom->SetupAttachment(RootComponent);
	FollowCamera->SetupAttachment(CameraBoom);

	CameraBoom->TargetArmLength = 1000.0f;
}

// Called when the game starts or when spawned
void AC_BaseCamera::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnPoints, FoundActors);
	
	if (UC_GameInstance* MyGameInstance = Cast<UC_GameInstance>(GetGameInstance()))
	{
		// Get Player Number
		//PlayerNumber = MyGameInstance->GetPlayerNumber();

		for (int i = 0; i < 4; i++)
		{
			// Get Which Player Character I want
			int ActualChar = MyGameInstance->PlayerCharacterArray[i];
			if (ActualChar != -1)
			{
				APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), i);
				if (PController == nullptr)
				{
					UGameplayStatics::CreatePlayer(GetWorld(), i, true);
				}

				//FActorSpawnParameters SpawnInfo;

				// Take The Class that the player want
				TSubclassOf<APawn> MyCharClass = MyCharacterArrayClass[ActualChar];
				 
				//Spawn it
				UGameplayStatics::GetPlayerController(GetWorld(), i)->Possess(GetWorld()->SpawnActor<APawn>(MyCharClass, FoundActors[i]->GetActorTransform()));
				UGameplayStatics::GetPlayerController(GetWorld(), i)->SetViewTargetWithBlend(this, 0.0f, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, false);
				CharArray.Add(UGameplayStatics::GetPlayerCharacter(GetWorld(), i));
			}
			
		}
	}
	CameraPosX = CameraBoom->GetComponentLocation().X;
	CameraPosY = CameraBoom->GetComponentLocation().Y;
	CameraPosZ = CameraBoom->GetComponentLocation().Z;
}

UCameraComponent* AC_BaseCamera::GetCamera() const
{
	return FollowCamera;
}

// Called every frame
void AC_BaseCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Delta = DeltaTime;

	GetLongestDistance();
	SetCenterPoint();
	CameraZoom();
	SetCameraPosition();
}

void AC_BaseCamera::CameraZoom() const
{
	float lenght = CameraBoom->TargetArmLength;
	CameraBoom->TargetArmLength = FMath::FInterpTo(lenght, FMath::Max(FMath::Min(LongestDistance, MaxDistanceZoom), MinDistanceZoom), Delta, 1.0f);
}

void AC_BaseCamera::SetCameraPosition()
{
	CameraPosY = FMath::FInterpTo(CameraPosY, CenterPosition.Y + YOffset, Delta, 0.5f);
	CameraPosX = FMath::FInterpTo(CameraPosX, CenterPosition.X + XOffset, Delta, 0.5f);
	
	CameraBoom->SetWorldLocation(FVector(CameraPosX, CameraPosY, CameraPosZ));
}

void AC_BaseCamera::SetCenterPoint()
{
	if (CharArray.Num() > 0)
	{
		FVector SumVector = FVector(0.0f, 0.0f, 0.0f);
		int AliveChar = CharArray.Num();
		for (ACharacter* MyChar : CharArray)
		{
			if (MyChar != nullptr)
			{
				SumVector += MyChar->GetActorLocation();
			}
		}
		CenterPosition = SumVector / FVector(static_cast<float>(FMath::Max(AliveChar, 1)), static_cast<float>(FMath::Max(AliveChar, 1)), static_cast<float>(FMath::Max(AliveChar, 1)));
	}
}

void AC_BaseCamera::GetLongestDistance()
{
	float MaxValue = 0.0f;
	
	for (int i = 0; i < CharArray.Num() - 1; i++)
	{
		for (int j = i + 1; j < CharArray.Num(); j++)
		{
			if (IsValid(CharArray[i]) & IsValid(CharArray[j]))
				if (CharArray[i]->GetDistanceTo(CharArray[j]) > MaxValue) MaxValue = CharArray[i]->GetDistanceTo(CharArray[j]);
		}
	}
	LongestDistance = MaxValue - 1000.0f;
}