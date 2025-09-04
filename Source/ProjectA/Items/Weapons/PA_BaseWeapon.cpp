// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/PA_BaseWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "PA_FunctionLibrary.h"

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
		// 오버랩된 대상이 적대적인 경우
		if (UPA_FunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			TPair<bool, FHitResult> AttackHit = GetAttackHitResult(OtherActor);

			if (AttackHit.Key)
			{
				// 상호작용 시작 이벤트 호출
				OnWeaponTargetHitStart.ExecuteIfBound(OtherActor, AttackHit.Value);
			}
			else
			{
				// 상호작용 시작 이벤트 호출
				OnWeaponTargetHitStart.ExecuteIfBound(OtherActor, AttackHit.Value);
			}
		}
	}
}

void APA_BaseWeapon::OnLeftCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	check(WeaponOwningPawn);

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		// 오버랩된 대상이 적대적인 경우
		if (UPA_FunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			// 상호작용 종료 이벤트 호출
			// 여기서는 FHitResult가 필요 없으므로, 기본 생성자를 넘김
			OnWeaponTargetHitStart.ExecuteIfBound(OtherActor, FHitResult());
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
		// 오버랩된 대상이 적대적인 경우
		if (UPA_FunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			TPair<bool, FHitResult> AttackHit = GetAttackHitResult(OtherActor);

			if (AttackHit.Key)
			{
				// 상호작용 시작 이벤트 호출
				OnWeaponTargetHitStart.ExecuteIfBound(OtherActor, AttackHit.Value);
			}
			else
			{
				// 상호작용 시작 이벤트 호출
				OnWeaponTargetHitStart.ExecuteIfBound(OtherActor, AttackHit.Value);
			}
		}
	}
}

void APA_BaseWeapon::OnRightCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	check(WeaponOwningPawn);

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		// 오버랩된 대상이 적대적인 경우
		if (UPA_FunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			// 상호작용 종료 이벤트 호출
			// 여기서는 FHitResult가 필요 없으므로, 기본 생성자를 넘김
			OnWeaponTargetHitStart.ExecuteIfBound(OtherActor, FHitResult());
		}
	}
}

TPair<bool, FHitResult> APA_BaseWeapon::GetAttackHitResult(AActor* HitActor)
{
	TPair<bool, FHitResult> Ret;

	if (APawn* HitPawn = Cast<APawn>(HitActor))
	{
		// 트레이스로 공격 충돌 지점 HitResult 얻기
		FHitResult AttackHit;
		FVector Start = GetActorLocation(); // 또는 무기 끝점
		FVector End = HitPawn->GetActorLocation(); // 또는 충돌 지점 추정
		FCollisionQueryParams Params(NAME_None, true, GetOwner());
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		bool bHit = GetWorld()->LineTraceSingleByChannel(AttackHit, Start, End, ECC_Pawn, Params);
		if (bHit)
		{
			Ret.Key = false;
			Ret.Value = AttackHit;
			return Ret;
		}
	}

	Ret.Key = false;
	Ret.Value = FHitResult();
	return Ret;
}
