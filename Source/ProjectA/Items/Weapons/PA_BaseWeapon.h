// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PA_BaseWeapon.generated.h"

UCLASS()
class PROJECTA_API APA_BaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	APA_BaseWeapon();

	FORCEINLINE class UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox; }

protected:
	UFUNCTION()
	void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom")
	TObjectPtr<class UStaticMeshComponent> WeaponMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom")
	TObjectPtr<class UBoxComponent> WeaponCollisionBox;


};
