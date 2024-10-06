// Fill out your copyright notice in the Description page of Project Settings.


#include "C_SpawnPoint.h"

// Sets default values
AC_SpawnPoint::AC_SpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AC_SpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}

