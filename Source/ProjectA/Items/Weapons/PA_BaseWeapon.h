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

	FORCEINLINE class UBoxComponent* GetLeftWeaponCollisionBox() const { return LeftWeaponCollisionBox; }
	FORCEINLINE class UBoxComponent* GetRightWeaponCollisionBox() const { return RightWeaponCollisionBox; }

	/* 왼쪽 무기 */
protected:
	UFUNCTION()
	void OnLeftCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLeftCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom")
	TObjectPtr<class UStaticMeshComponent> LeftWeaponMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom")
	TObjectPtr<class UBoxComponent> LeftWeaponCollisionBox;


	/* 오른쪽 무기 */
protected:
	UFUNCTION()
	void OnRightCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRightCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom")
	TObjectPtr<class UStaticMeshComponent> RightWeaponMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom")
	TObjectPtr<class UBoxComponent> RightWeaponCollisionBox;

};
