// Fill out your copyright notice in the Description page of Project Settings.


#include "C_GameInstance.h"

#include "Kismet/GameplayStatics.h"

UC_GameInstance::UC_GameInstance()
{
	PlayerCharacterArray = { -1 , -1, -1, -1 };
}

void UC_GameInstance::PlayMyMusicSound()
{
	MySound = UGameplayStatics::SpawnSound2D(GetWorld(), MyStartMusic, 1.0f, 1.0f, 0.0f, nullptr, true, true);
}

int UC_GameInstance::GetPlayerNumber()
{
	int cpt = 0;
	for (int i : PlayerCharacterArray)
	{
		if (i != -1) cpt++;
	}
	return cpt;
}