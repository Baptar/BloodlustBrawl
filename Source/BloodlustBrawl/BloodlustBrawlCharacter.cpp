// Copyright Epic Games, Inc. All Rights Reserved.

#include "BloodlustBrawlCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/HitResult.h"
#include "C_BaseCamera.h"


//////////////////////////////////////////////////////////////////////////
// ABloodlustBrawlCharacter

ABloodlustBrawlCharacter::ABloodlustBrawlCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(35.f, 72.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->MaxAcceleration = 1500.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ABloodlustBrawlCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_BaseCamera::StaticClass(), FoundActors);
	
	if (AC_BaseCamera* Cam = Cast<AC_BaseCamera>(FoundActors[0])) BaseCam = Cam;

	//if (APlayerController* MyPlayerController = Cast<APlayerController>(GetController())) DisableInput(MyPlayerController);
}

void ABloodlustBrawlCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Delta = DeltaTime;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABloodlustBrawlCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABloodlustBrawlCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABloodlustBrawlCharacter::Look);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Started, this, &ABloodlustBrawlCharacter::StartedLook);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, &ABloodlustBrawlCharacter::CompletedLook);

		//Melee Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ABloodlustBrawlCharacter::MeleeAttack);

		// Special Attack
		EnhancedInputComponent->BindAction(SpecialAttackAction, ETriggerEvent::Started, this, &ABloodlustBrawlCharacter::SpecialAttack);
	}
}

void ABloodlustBrawlCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (BaseCam->GetCamera())
	{
		// find out which way is forward
		const FRotator Rotation = BaseCam->GetCamera()->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABloodlustBrawlCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(FVector(0.0f, 0.0f, 0.0f), FVector(LookAxisVector.X, LookAxisVector.Y, 0.0f));
	TargetRot.Yaw -= 45.0f;
	FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, Delta, 8.0f);

	SetActorRotation(NewRot);
}

void ABloodlustBrawlCharacter::StartedLook(const FInputActionValue& Value)
{
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
}

void ABloodlustBrawlCharacter::CompletedLook(const FInputActionValue& Value)
{
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

void ABloodlustBrawlCharacter::MeleeAttack(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
		bCanAttack = false;
		// Play Attack Animation
		if (MeleeAttackAnim) PlayAnimMontage(MeleeAttackAnim);

		// Basic Attack
		if (bSwordAttack)
		{
			// Check Attack Collision after animation delay
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
			{
				FCollisionShape MySphere = FCollisionShape::MakeSphere(50.0f); // 5M Radius
        
				TArray<FHitResult> OutResults;
				FVector SweepStart = GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 35.0f);
				FVector SweepEnd = SweepStart + UKismetMathLibrary::GetRightVector(GetMesh()->GetComponentRotation()) * MeleeAttackDistanceBase;
				DrawDebugSphere(GetWorld(), SweepStart, MySphere.GetSphereRadius(), 50, FColor::Purple, true);
				DrawDebugSphere(GetWorld(), SweepEnd, MySphere.GetSphereRadius(), 50, FColor::Purple, true);

				FCollisionQueryParams Params;
				Params.AddIgnoredActor(this);
        
				TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
				TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
        
				if (GetWorld()->SweepMultiByObjectType(OutResults, SweepStart, SweepEnd, FQuat::Identity, TraceObjectTypes, MySphere, Params))
				{
					for (FHitResult MyResult : OutResults)
					{
						if (ACharacter* CharToLaunch = Cast<ACharacter>(MyResult.GetActor()))
						{
							FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MyResult.GetActor()->GetActorLocation());
							FVector ForwardVect = UKismetMathLibrary::GetForwardVector(LookAtRot) * PowerAttack;
							FVector LaunchDirection = ForwardVect + FVector(0.0f, 0.0f, 200.0f);
							CharToLaunch->LaunchCharacter(LaunchDirection, false, false);
						}
					}
				}
			}, 0.37f, false);
		}

		// Can Attack After delay
		FTimerHandle TimerHandleCanAttack;
		GetWorld()->GetTimerManager().SetTimer(TimerHandleCanAttack, [&](){ bCanAttack = true;}, DelayBetweenAttack, false);
	}
}

void ABloodlustBrawlCharacter::SpecialAttack(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Base Character Special Attack"));
}

void ABloodlustBrawlCharacter::DeathEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Death Event"));
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_BaseCamera::StaticClass(), FoundActors);

	if (APlayerController* MyPlayerController = Cast<APlayerController>(GetController()))
		DisableInput(MyPlayerController);

	if (DeathAnim) PlayAnimMontage(DeathAnim);
	if (PloufSound) UGameplayStatics::PlaySoundAtLocation(this, PloufSound, GetActorLocation());

	if (IsValid(FoundActors[0]))
		if (AC_BaseCamera* MyCameraBase = Cast<AC_BaseCamera>(FoundActors[0]))
		{
			MyCameraBase->CharArray.Remove(this);
		}
}