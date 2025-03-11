// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/PA_BaseWeapon.h"

// Sets default values
APA_BaseWeapon::APA_BaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APA_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APA_BaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

