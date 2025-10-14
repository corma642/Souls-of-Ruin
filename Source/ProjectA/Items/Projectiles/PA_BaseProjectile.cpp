// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Projectiles/PA_BaseProjectile.h"
#include "Characters/PA_CharacterBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "PA_FunctionLibrary.h"
#include "PA_GameplayTags.h"

APA_BaseProjectile::APA_BaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// 발사체 콜리전 초기화
	ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	SetRootComponent(ProjectileCollisionBox);
	ProjectileCollisionBox->SetCollisionProfileName(TEXT("Projectile"));

	// 발사체 나이아가라 초기화
	ProjectileNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComp"));
	ProjectileNiagaraComp->SetupAttachment(GetRootComponent());

	// 발사체 캐스케이드 초기화
	ProjectileCascadeComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileCascadeComp"));
	ProjectileCascadeComp->SetupAttachment(GetRootComponent());

	// 발사체 무브먼트 컴포넌트 초기화
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = ProjectileInitialSpeed;	// 초기 속력
	ProjectileMovementComp->MaxSpeed = ProjectileMaxSpeed;			// 최대 속력
	ProjectileMovementComp->Velocity = FVector(1.f, 0.f, 0.f);		// 속도
	ProjectileMovementComp->ProjectileGravityScale = 0.f;			// 중력 영향 X

	// 오버랩 함수 바인딩
	ProjectileCollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit);
	ProjectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);
}

void APA_BaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	// 발사체 생명 주기 초기화
	InitialLifeSpan = LifeSpan;
	SetLifeSpan(LifeSpan);
}

void APA_BaseProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 피격 대상이 Instigator면 넘어감
	if (!OtherActor || OtherActor == GetInstigator()) return;

	// 피격 대상이 적대적이지 않으면 즉시 제거
	if (!UPA_FunctionLibrary::IsTargetPawnHostile(GetInstigator(), Cast<APawn>(OtherActor)))
	{
		// 발사체 피격 FX 스폰
		BP_OnSpawnProjectileHitFX(Hit.ImpactPoint, false);
		Destroy();
		return;
	}

	// 무적 여부
	bool bIsInvincible = UPA_FunctionLibrary::NativeIsInvincible(OtherActor);
	if (bIsInvincible)
	{
		Destroy();
		return;
	}

	// 피해자의 막기 유효 여부 확인
	bool bIsValidBlock = UPA_FunctionLibrary::NativeIsValidBlock(GetInstigator(), OtherActor);

	// 피격 이벤트 전달 객체
	FGameplayEventData Payload;
	Payload.Instigator = this;
	Payload.Target = OtherActor;

	// 피격 HitResult
	UAbilitySystemComponent* ASC = Cast<APA_CharacterBase>(GetInstigator())->GetAbilitySystemComponent();
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddHitResult(Hit, false);
	Payload.ContextHandle = EffectContext;

	if (bIsValidBlock)
	{
		// 막기에 성공했으니, 막기 성공 이벤트를 전달
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			OtherActor,
			PA_GameplayTags::Player_Event_SuccessfulBlock,
			Payload
		);
	}
	else
	{
		// 발사체 피격 FX 스폰
		BP_OnSpawnProjectileHitFX(Hit.ImpactPoint, true);

		// 발사체 대미지 이펙트 스펙 핸들 적용
		HandleApplyProjectileDamage(Cast<APawn>(OtherActor), Payload);
	}

	Destroy();
}

void APA_BaseProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void APA_BaseProjectile::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload)
{
	if (!ProjectileDamegeEffectSpecHandle.IsValid())
	{
		return;
	}

	// 타깃에게 발사체 대미지 이펙트 스펙 핸들을 적용하고 결과를 저장
	const bool bWasDamageEffectApplied = UPA_FunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(
		GetInstigator(),
		InHitPawn,
		ProjectileDamegeEffectSpecHandle
	);

	if (bWasDamageEffectApplied)
	{
		// 피격 이벤트를 전달
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			InHitPawn,
			PA_GameplayTags::Shared_Event_HitReact,
			InPayload
		);
	}
}
