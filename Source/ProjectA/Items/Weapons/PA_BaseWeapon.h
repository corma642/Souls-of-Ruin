// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PA_BaseWeapon.generated.h"

// 대상 상호작용 델리게이트
DECLARE_DELEGATE_TwoParams(FOnTargetInteractedDelegate, AActor*, const FHitResult&)

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

	// 상호작용 시작 이벤트
	FOnTargetInteractedDelegate OnWeaponTargetHitStart;

	// 상호작용 종료 이벤트
	FOnTargetInteractedDelegate OnWeaponTargetHitEnd;

protected:
	// 왼쪽 무기 콜리전 오버랩 함수
	UFUNCTION()
	void OnLeftCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 왼쪽 무기 콜리전 오버랩 종료 함수
	UFUNCTION()
	void OnLeftCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 오른쪽 무기 콜리전 오버랩 함수
	UFUNCTION()
	void OnRightCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 오른쪽 무기 콜리전 오버랩 종료 함수
	UFUNCTION()
	void OnRightCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// 왼쪽 무기 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Mesh")
	TObjectPtr<class UStaticMeshComponent> LeftWeaponMesh = nullptr;

	// 왼쪽 무기 콜리전 박스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Mesh")
	TObjectPtr<class UBoxComponent> LeftWeaponCollisionBox;

	// 오른쪽 무기 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Mesh")
	TObjectPtr<class UStaticMeshComponent> RightWeaponMesh = nullptr;

	// 오른쪽 무기 콜리전 박스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Mesh")
	TObjectPtr<class UBoxComponent> RightWeaponCollisionBox;
};
