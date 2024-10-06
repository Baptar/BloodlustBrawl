// Fill out your copyright notice in the Description page of Project Settings.


#include "C_FinnTheFrog.h"
#include "GameFramework/CharacterMovementComponent.h"

void AC_FinnTheFrog::SpecialAttack(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Finn The Frog Special Attack"));
	
	FCollisionShape MySphere = FCollisionShape::MakeSphere(SphereRadius); // 5M Radius
	
	TArray<FHitResult> OutResults;
	DrawDebugSphere(GetWorld(), GetActorLocation(), MySphere.GetSphereRadius(), SphereRadius, FColor::Blue, true);


	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	
	if (GetWorld()->SweepMultiByObjectType(OutResults, GetActorLocation(), GetActorLocation(), FQuat::Identity, TraceObjectTypes, MySphere, Params))
	{
		for (FHitResult MyResult : OutResults)
		{
			if (ACharacter* CharToGravity = Cast<ACharacter>(MyResult.GetActor()))
			{
				CharToGravity->GetCharacterMovement()->GravityScale = 0.0f;
				CharToGravity->LaunchCharacter(FVector(0.0f, 0.0f, 15.0f), false, false);
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
					{
						CharToGravity->GetCharacterMovement()->GravityScale = 1.75f;
					}, 5.0f, false);
			}
		}
	}
}

