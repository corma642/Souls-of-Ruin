// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PA_BaseWeapon.generated.h"

// ��� ��ȣ�ۿ� ��������Ʈ
DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*)

UCLASS()
class PROJECTA_API APA_BaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	APA_BaseWeapon();

	FORCEINLINE class UBoxComponent* GetLeftWeaponCollisionBox() const { return LeftWeaponCollisionBox; }
	FORCEINLINE class UBoxComponent* GetRightWeaponCollisionBox() const { return RightWeaponCollisionBox; }

	FORCEINLINE class UStaticMeshComponent* GetLeftWeaponMesh() const { return LeftWeaponMesh; }
	FORCEINLINE class UStaticMeshComponent* GetRightWeaponMesh() const { return RightWeaponMesh; }

	// ��ȣ�ۿ� ���� �̺�Ʈ
	FOnTargetInteractedDelegate OnWeaponTargetHitStart;
	
	// ��ȣ�ۿ� ���� �̺�Ʈ
	FOnTargetInteractedDelegate OnWeaponTargetHitEnd;


	///////////////////////////////////////////////////////////////////////////
	/* ���� ���� */
protected:
	UFUNCTION()
	void OnLeftCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLeftCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Mesh")
	TObjectPtr<class UStaticMeshComponent> LeftWeaponMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Mesh")
	TObjectPtr<class UBoxComponent> LeftWeaponCollisionBox;


	///////////////////////////////////////////////////////////////////////////
	/* ������ ���� */
protected:
	UFUNCTION()
	void OnRightCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRightCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Mesh")
	TObjectPtr<class UStaticMeshComponent> RightWeaponMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Mesh")
	TObjectPtr<class UBoxComponent> RightWeaponCollisionBox;
};
