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
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.2f;

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
	ProjectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);

	// Projectile 태그 추가
	Tags.AddUnique(TEXT("Projectile"));
}

void APA_BaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 발사체 생명 주기 초기화
	InitialLifeSpan = LifeSpan;
	SetLifeSpan(LifeSpan);
}

void APA_BaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 장애물 및 배경 오브젝트에 부딪힌 경우의 처리
	if (ProjectileCollisionBox &&
		ProjectileCollisionBox->GetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic) == ECollisionResponse::ECR_Block)
	{
		// 현재 발사체의 속도 가져오기
		const FVector CurrentVelocity = ProjectileMovementComp->Velocity;

		// 속도 벡터의 길이 제곱이 임계값 제곱보다 작은 경우
		// Block인 장애물 및 배경 오브젝트에 부딪혔다고 판별
		if (CurrentVelocity.SizeSquared() <= 0.1f * 0.1f)
		{
			// 발사체 피격 FX 스폰
			BP_OnSpawnProjectileHitFX(GetActorLocation(), false);
			Destroy();
			return;
		}
	}
}

void APA_BaseProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 피격 대상이 유효하지 않거나, 시전자거나, 같은 발사체거나, 무기라면 리턴
	if (!OtherActor ||
		OtherActor == GetInstigator() ||
		OtherActor->ActorHasTag(TEXT("Projectile")) ||
		OtherActor->ActorHasTag(TEXT("Weapon")))
	{
		return;
	}

	// 같은 액터 태그를 가지고 있으면 리턴
	for (const FName& Tag : GetInstigator()->Tags)
	{
		if (OtherActor->ActorHasTag(Tag)) return;
	}

	// 오버랩 배열에 추가
	OverlapActors.AddUnique(OtherActor);

	// 무적 여부
	bool bIsInvincible = UPA_FunctionLibrary::NativeIsInvincible(OtherActor);
	if (bIsInvincible)
	{
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
	EffectContext.AddHitResult(SweepResult, false);
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
		BP_OnSpawnProjectileHitFX(SweepResult.ImpactPoint, true);

		// 발사체 대미지 이펙트 스펙 핸들 적용
		HandleApplyProjectileDamage(Cast<APawn>(OtherActor), Payload);
	}

	// 단일 대미지 타입이라면, 한 번 충돌 후 제거
	if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnHit)
	{
		Destroy();
		return;
	}
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
