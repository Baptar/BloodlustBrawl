// Fill out your copyright notice in the Description page of Project Settings.


#include "C_CommonCactus.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AC_CommonCactus::AC_CommonCactus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CactusMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	CactusCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
		
	RootComponent = CactusMesh;
	CactusCapsule->SetupAttachment(CactusMesh);

	CactusCapsule->OnComponentBeginOverlap.AddDynamic(this, &AC_CommonCactus::BeginOverlap);
	CactusCapsule->SetGenerateOverlapEvents(false);

	ProjectileMovementComponent->SetAutoActivate(false);
	RotatingMovementComponent->SetAutoActivate(false);
	CactusMesh->SetVisibility(false);

}

// Called when the game starts or when spawned
void AC_CommonCactus::BeginPlay()
{
	Super::BeginPlay();
	
}

void AC_CommonCactus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bUsed)
		ProjectileMovementComponent->Velocity = FVector(CharacterForward.X * 500.0f, CharacterForward.Y * 500.0f, ProjectileMovementComponent->Velocity.Z);
}


void AC_CommonCactus::OnActivation(AActor& CactusCharacrer)
{
	bUsed = true;
	CactusCapsule->SetGenerateOverlapEvents(true);
	CharacterForward = CactusCharacrer.GetActorForwardVector();
	CactusMesh->SetVisibility(true);
	ProjectileMovementComponent->Activate();
	RotatingMovementComponent->Activate();
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			bUsed = false;
			CactusCapsule->SetGenerateOverlapEvents(false);
			CactusMesh->SetVisibility(false);
			CharacterForward = FVector::ZeroVector;
			ProjectileMovementComponent->Deactivate();
			RotatingMovementComponent->Deactivate();
			AttachToActor(&CactusCharacrer, FAttachmentTransformRules::KeepWorldTransform);
			SetActorRelativeLocation(FVector(80.0f,23.0f,0.0f));
			SetActorRelativeRotation(FRotator::ZeroRotator);
		}, 6.0f, false);
}

void AC_CommonCactus::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* Char = Cast<ACharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Display, TEXT("Should Launch Character"));
		if (Char != MyCharacter)
		{
				const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Char->GetActorLocation(), FVector(GetActorLocation().X, GetActorLocation().Y, Char->GetActorLocation().Z));
        		FVector ForwardVect = UKismetMathLibrary::GetForwardVector(LookAtRot);
        		ForwardVect.X *= -LaunchPower;
        		ForwardVect.Y *= -LaunchPower;
        		ForwardVect.Z *= UKismetMathLibrary::Abs(ForwardVect.Z * LaunchPower);
        		Char->LaunchCharacter(ForwardVect + FVector(0.0f, 0.0f, 50.0f), false, false);
		}
	}
}

