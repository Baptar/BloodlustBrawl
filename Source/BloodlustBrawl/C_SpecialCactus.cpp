// Fill out your copyright notice in the Description page of Project Settings.


#include "C_SpecialCactus.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AC_SpecialCactus::AC_SpecialCactus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CactusMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	CactusCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	DefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));

	RootComponent = DefaultScene;
	CactusMesh->SetupAttachment(GetRootComponent());
	CactusCapsule->SetupAttachment(CactusMesh);

	CactusMesh->SetVisibility(false);
	CactusCapsule->SetGenerateOverlapEvents(false);
	CactusCapsule->OnComponentBeginOverlap.AddDynamic(this, &AC_SpecialCactus::BeginOverlap);
}

// Called when the game starts or when spawned
void AC_SpecialCactus::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat != nullptr)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AC_SpecialCactus::SpawnCactus);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
	}
	if (CurveFloatRemove != nullptr)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AC_SpecialCactus::RemoveCactus);
		TimelineRemove.AddInterpFloat(CurveFloatRemove, TimelineProgress);
	}
}

void AC_SpecialCactus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
	TimelineRemove.TickTimeline(DeltaTime);
}

void AC_SpecialCactus::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* Char = Cast<ACharacter>(OtherActor))
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Char->GetActorLocation(), FVector(GetActorLocation().X, GetActorLocation().Y, Char->GetActorLocation().Z));
		FVector ForwardVect = UKismetMathLibrary::GetForwardVector(LookAtRot);
		ForwardVect.X *= -LaunchPower;
		ForwardVect.Y *= -LaunchPower;
		ForwardVect.Z *= UKismetMathLibrary::Abs(ForwardVect.Z * LaunchPower);

		Char->LaunchCharacter(ForwardVect + FVector(0.0f, 0.0f, 50.0f), false, false);
	}
}

void AC_SpecialCactus::Appear()
{
	CactusMesh->SetVisibility(true);
	BeforeLoc = CactusMesh->GetComponentLocation();
	Timeline.Play();
	if (AppearSound) UGameplayStatics::PlaySoundAtLocation(this, AppearSound, GetActorLocation());

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		CactusCapsule->SetGenerateOverlapEvents(true);
		FTimerHandle TimerHandleBis;
		GetWorld()->GetTimerManager().SetTimer(TimerHandleBis, [&]()
		{
			Disappear();
		}, 5.0f, false);
	}, Timeline.GetTimelineLength(), false);
}

void AC_SpecialCactus::Disappear()
{
	Timeline.Reverse();
	if (DisappearSound) UGameplayStatics::PlaySoundAtLocation(this, DisappearSound, GetActorLocation());

	FTimerHandle TimerHandleBis;
	GetWorld()->GetTimerManager().SetTimer(TimerHandleBis, [&]()
	{
		CactusMesh->SetVisibility(false);
		CactusCapsule->SetGenerateOverlapEvents(false);
		if (CactusChar)
		{
			CactusChar->CactusChildArray[CactusNumber]->AttachToComponent(CactusChar->GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			CactusChar->CactusChildArray[CactusNumber]->SetRelativeLocation(FVector(0.0f, 0.0f, -250.0f));
			CactusChar->bIsAvailableArray[CactusNumber] = true;
		}
	}, Timeline.GetTimelineLength(), false);
}

void AC_SpecialCactus::SpawnCactus(float Value)
{
	CactusMesh->SetWorldLocation(FVector(BeforeLoc.X, BeforeLoc.Y, FMath::FInterpTo(BeforeLoc.Z, BeforeLoc.Z + 180, Value, 1.0f)));
}

void AC_SpecialCactus::RemoveCactus(float Value)
{
	CactusMesh->SetWorldLocation(FVector(BeforeLoc.X, BeforeLoc.Y, FMath::FInterpTo(BeforeLoc.Z + 180, BeforeLoc.Z , Value, 1.0f)));
}




