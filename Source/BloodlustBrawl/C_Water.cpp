// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Water.h"
#include "BloodlustBrawlCharacter.h"

// Sets default values
AC_Water::AC_Water()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Init StaticMesh
	WaterPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	WaterPlane->SetupAttachment(GetRootComponent());
	WaterPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Init Box Collision
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetupAttachment(WaterPlane);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AC_Water::BeginOverlap);
	BoxComponent->SetBoxExtent(FVector(50.0f, 50.0f, 32.0f), true);

}

// Called when the game starts or when spawned
void AC_Water::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/*void AC_Water::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/


void AC_Water::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (ABloodlustBrawlCharacter* MyChar = Cast<ABloodlustBrawlCharacter>(OtherActor))
	{
		MyChar->DeathEvent();
	}
}
