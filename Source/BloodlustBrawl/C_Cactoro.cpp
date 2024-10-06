// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Cactoro.h"
#include "C_SpecialCactus.h"
#include "GameFramework/CharacterMovementComponent.h"


AC_Cactoro::AC_Cactoro()
{
	Child0 = CreateDefaultSubobject<UChildActorComponent>(FName(TEXT("Child0")));
	Child1 = CreateDefaultSubobject<UChildActorComponent>(FName(TEXT("Child1")));
	Child2 = CreateDefaultSubobject<UChildActorComponent>(FName(TEXT("Child2")));
	ChildMunition1 = CreateDefaultSubobject<UChildActorComponent>(FName(TEXT("ChildMunition1")));
	ChildMunition2 = CreateDefaultSubobject<UChildActorComponent>(FName(TEXT("ChildMunition2")));
	ChildMunition3 = CreateDefaultSubobject<UChildActorComponent>(FName(TEXT("ChildMunition3")));
	ChildMunition4 = CreateDefaultSubobject<UChildActorComponent>(FName(TEXT("ChildMunition4")));
	ChildMunition5 = CreateDefaultSubobject<UChildActorComponent>(FName(TEXT("ChildMunition5")));
	ChildMunition6 = CreateDefaultSubobject<UChildActorComponent>(FName(TEXT("ChildMunition6")));
	ChildMunition7 = CreateDefaultSubobject<UChildActorComponent>(FName(TEXT("ChildMunition7")));
	ChildMunition8 = CreateDefaultSubobject<UChildActorComponent>(FName(TEXT("ChildMunition8")));
	ChildMunition9 = CreateDefaultSubobject<UChildActorComponent>(FName(TEXT("ChildMunition9")));
	ChildMunition10 = CreateDefaultSubobject<UChildActorComponent>(FName(TEXT("ChildMunition10")));
	
	CactusNumberWidget = CreateDefaultSubobject<UWidgetComponent>(FName(TEXT("WidgetCactus")));

	Child0->SetupAttachment(GetRootComponent());
	Child1->SetupAttachment(GetRootComponent());
	Child2->SetupAttachment(GetRootComponent());
	ChildMunition1->SetupAttachment(GetRootComponent());
	ChildMunition2->SetupAttachment(GetRootComponent());
	ChildMunition3->SetupAttachment(GetRootComponent());
	ChildMunition4->SetupAttachment(GetRootComponent());
	ChildMunition5->SetupAttachment(GetRootComponent());
	ChildMunition6->SetupAttachment(GetRootComponent());
	ChildMunition7->SetupAttachment(GetRootComponent());
	ChildMunition8->SetupAttachment(GetRootComponent());
	ChildMunition9->SetupAttachment(GetRootComponent());
	ChildMunition10->SetupAttachment(GetRootComponent());
	CactusNumberWidget->SetupAttachment(GetRootComponent());

	Child0->SetRelativeLocation(FVector(0.0f, 0.0f, -250.0f));
	Child1->SetRelativeLocation(FVector(0.0f, 0.0f, -250.0f));
	Child2->SetRelativeLocation(FVector(0.0f, 0.0f, -250.0f));
}

void AC_Cactoro::BeginPlay()
{
	Super::BeginPlay();

	Child0->SetChildActorClass(CactusActor);
	Child1->SetChildActorClass(CactusActor);
	Child2->SetChildActorClass(CactusActor);

	ChildMunition1->SetChildActorClass(CactusMunitionActor);
	ChildMunition2->SetChildActorClass(CactusMunitionActor);
	ChildMunition3->SetChildActorClass(CactusMunitionActor);
	ChildMunition4->SetChildActorClass(CactusMunitionActor);
	ChildMunition5->SetChildActorClass(CactusMunitionActor);
	ChildMunition6->SetChildActorClass(CactusMunitionActor);
	ChildMunition7->SetChildActorClass(CactusMunitionActor);
	ChildMunition8->SetChildActorClass(CactusMunitionActor);
	ChildMunition9->SetChildActorClass(CactusMunitionActor);
	ChildMunition10->SetChildActorClass(CactusMunitionActor);
	
	CactusChildArray = {Child0 , Child1, Child2};
	CactusMunitionChildArray = {ChildMunition1, ChildMunition2, ChildMunition3, ChildMunition4, ChildMunition5, ChildMunition6, ChildMunition7, ChildMunition8, ChildMunition9, ChildMunition10};

	for (int i = 0; i < CactusChildArray.Num(); i++)
	{
		if (AC_SpecialCactus* Cactus = Cast<AC_SpecialCactus>(CactusChildArray[i]->GetChildActor()))
		{
			Cactus->CactusChar = this;
			Cactus->CactusNumber = i;
		}
	}

	for (int i = 0; i < CactusMunitionChildArray.Num(); i++)
	{
		if (AC_CommonCactus* CactusMunition = Cast<AC_CommonCactus>(CactusMunitionChildArray[i]->GetChildActor()))
		{
			CactusMunition->MyCharacter = this;
		}
	}
}

int AC_Cactoro::CactusNumberAvailable()
{
	int Result = 0;
	for (bool bValue : bIsAvailableArray)
		if (bValue) Result++;

	return Result;
}

void AC_Cactoro::MeleeAttack(const FInputActionValue& Value)
{
	//Super::MeleeAttack(Value);
	if (bCanAttack)
	{
		bCanAttack = false;
		for (int i = 0; i < CactusMunitionChildArray.Num(); i++)
		{
			if (AC_CommonCactus* CactusMunition = Cast<AC_CommonCactus>(CactusMunitionChildArray[i]->GetChildActor()))
			{
				if (!CactusMunition->bUsed)
				{
					CactusMunition->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					CactusMunition->OnActivation(*this);
					break;
				}
			}
		}
		FTimerHandle TimerHandleCanAttack;
		GetWorld()->GetTimerManager().SetTimer(TimerHandleCanAttack, [&](){ bCanAttack = true;}, DelayBetweenAttack, false);
	}
}
	
	

void AC_Cactoro::DeathEvent()
{
	Super::DeathEvent();

	CactusNumberWidget->SetVisibility(false);
}

void AC_Cactoro::SpecialAttack(const FInputActionValue& Value)
{
	if (!(GetCharacterMovement()->IsFalling()))
	for (int i = 0; i < bIsAvailableArray.Num(); i++)
	{
		if (bIsAvailableArray[i])
		{
			CactusChildArray[i]->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			bIsAvailableArray[i] = false;
			if (AC_SpecialCactus* Cactus = Cast<AC_SpecialCactus>(CactusChildArray[i]->GetChildActor()))
			{
				Cactus->Appear();
			}
			break;
		}
	}
}
