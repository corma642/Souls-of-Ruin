// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "PA_BaseProjectile.generated.h"

// 발사체 대미지 타입
UENUM(BlueprintType)
enum class EProjectileDamagePolicy : uint8
{
	OnHit,			// 단일 대미지 타입
	OnBeginOverlap,	// 지속 대미지 타입
};

UCLASS()
class PROJECTA_API APA_BaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	APA_BaseProjectile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 발사체 피격 FX
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Spawn Projectile Hit FX"))
	void BP_OnSpawnProjectileHitFX(const FVector& HitLocation);

	// 발사체 피해 적용 함수
	void HandleApplyProjectileDamage(APawn* InHitPawn, const struct FGameplayEventData& InPayload);

protected:
	// 발사체 대미지 타입
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	EProjectileDamagePolicy ProjectileDamagePolicy = EProjectileDamagePolicy::OnHit;

	// 발사체 콜리전 컴포넌트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<class UBoxComponent> ProjectileCollisionBox;

	// 발사체 무브먼트 컴포넌트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComp;

	// 나이아가라
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<class UNiagaraComponent> ProjectileNiagaraComp;

	// 캐스케이드
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<class UParticleSystemComponent> ProjectileCascadeComp;

	// 발사체 초기 속력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	float ProjectileInitialSpeed = 700.f;

	// 발사체 최대 속력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	float ProjectileMaxSpeed = 1000.f;

	// 발사체 생명 주기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	float LifeSpan = 3.f;

	// 발사체 대미지 이펙트 스펙 핸들
	UPROPERTY(BlueprintReadOnly, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle ProjectileDamegeEffectSpecHandle;
};
