// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/PA_BaseWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

APA_BaseWeapon::APA_BaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	// 왼쪽 무기 메쉬
	LeftWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWeaponMesh"));
	LeftWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftWeaponMesh->SetupAttachment(GetRootComponent());
	LeftWeaponMesh->bReceivesDecals = false;

	// 왼쪽 콜리전 박스
	LeftWeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftWeaponCollisionBox"));
	LeftWeaponCollisionBox->SetupAttachment(LeftWeaponMesh);
	LeftWeaponCollisionBox->SetBoxExtent(FVector(20.f));
	LeftWeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftWeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnLeftCollisionBoxBeginOverlap);
	LeftWeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnLeftCollisionBoxEndOverlap);


	// 오른쪽 무기 메쉬
	RightWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWeaponMesh"));
	RightWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightWeaponMesh->SetupAttachment(GetRootComponent());
	RightWeaponMesh->bReceivesDecals = false;

	// 오른쪽 콜리전 박스
	RightWeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightWeaponCollisionBox"));
	RightWeaponCollisionBox->SetupAttachment(RightWeaponMesh);
	RightWeaponCollisionBox->SetBoxExtent(FVector(20.f));
	RightWeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightWeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnRightCollisionBoxBeginOverlap);
	RightWeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnRightCollisionBoxEndOverlap);
}

// 왼쪽 콜리전
void APA_BaseWeapon::OnLeftCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	check(WeaponOwningPawn);

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (HitPawn != WeaponOwningPawn)
		{
			OnWeaponTargetHitStart.ExecuteIfBound(OtherActor);
		}
	}
}

void APA_BaseWeapon::OnLeftCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	check(WeaponOwningPawn);

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (HitPawn != WeaponOwningPawn)
		{
			OnWeaponTargetHitEnd.ExecuteIfBound(OtherActor);
		}
	}
}

// 오른쪽 콜리전
void APA_BaseWeapon::OnRightCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	check(WeaponOwningPawn);

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (HitPawn != WeaponOwningPawn)
		{
			OnWeaponTargetHitStart.ExecuteIfBound(OtherActor);
		}
	}
}

void APA_BaseWeapon::OnRightCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	check(WeaponOwningPawn);

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (HitPawn != WeaponOwningPawn)
		{
			OnWeaponTargetHitEnd.ExecuteIfBound(OtherActor);
		}
	}
}
