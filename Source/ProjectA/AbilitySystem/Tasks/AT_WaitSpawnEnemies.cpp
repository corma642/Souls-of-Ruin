// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AT_WaitSpawnEnemies.h"
#include "AbilitySystemComponent.h"
#include "Characters/PA_CharacterEnemy.h"
#include "Engine/AssetManager.h"
#include "NavigationSystem.h"

UAT_WaitSpawnEnemies::UAT_WaitSpawnEnemies()
{
	bTickingTask = false;
}

UAT_WaitSpawnEnemies* UAT_WaitSpawnEnemies::WaitSpawnEnemies(
	UGameplayAbility* OwningAbility,
	FGameplayTag EventTag,
	TArray<TSoftClassPtr<class APA_CharacterEnemy>> SoftEnemyClassToSpawn,
	const int32 NumToSpawn,
	const float MinSpawnExtent,
	const float MaxSpawnExtent
)
{
	// 태스크 객체 생성, 생성된 태스크를 관리할 Owner 어빌리티 설정
	UAT_WaitSpawnEnemies* Node = NewAbilityTask<UAT_WaitSpawnEnemies>(OwningAbility);

	// 변수 캐시 저장
	Node->CachedEventTag = EventTag;
	Node->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedMinSpawnExtent = MinSpawnExtent;
	Node->CachedMaxSpawnExtent = MaxSpawnExtent;

	return Node;
}

void UAT_WaitSpawnEnemies::Activate()
{
	if (AbilitySystemComponent.IsValid())
	{
		// ASC에 이벤트 태그 트리거 델리게이트 추가
		FGameplayEventMulticastDelegate* Delegate = &AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

		if (Delegate)
		{
			// 이벤트 트리거 콜백 함수 바인딩 후, 델리게이트 저장
			DelegateHandle = Delegate->AddUObject(this, &ThisClass::OnGameplayEventReceived);
		}
	}
	else
	{
		SpawnFailed();
	}
}

void UAT_WaitSpawnEnemies::OnDestroy(bool bInOwnerFinished)
{
	if (AbilitySystemComponent.IsValid())
	{
		// ASC에 이벤트 태그 트리거 델리게이트 검색
		FGameplayEventMulticastDelegate* Delegate = &AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

		if (Delegate)
		{
			// Activate에서 바인딩한 델리게이트 제거
			Delegate->Remove(DelegateHandle);
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UAT_WaitSpawnEnemies::OnGameplayEventReceived(const FGameplayEventData* InPayload)
{
	if (CachedSoftEnemyClassToSpawn.IsEmpty())
	{
		// 적 소환 실패 함수 호출
		SpawnFailed();
		return;
	}

	// 적 비동기 로딩 요청
	for (int32 i = 0; i < CachedSoftEnemyClassToSpawn.Num(); ++i)
	{
		if (!CachedSoftEnemyClassToSpawn[i].IsNull())
		{
			// 적 비동기 로딩 요청
			if (i + 1 == CachedSoftEnemyClassToSpawn.Num())
			{
				// 마지막 적 로딩 요청인 경우, 로딩 종료 콜백 함수 바인딩
				UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
					CachedSoftEnemyClassToSpawn[i].ToSoftObjectPath(),
					FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoaded)
				);
			}
			else
			{
				UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
					CachedSoftEnemyClassToSpawn[i].ToSoftObjectPath());
			}
		}
		else
		{
			// 적 소환 실패 함수 호출
			SpawnFailed();
		}
	}
}

// 원형 기반
//FVector UAT_WaitSpawnEnemies::GetSpawnLocationInRandRange()
//{
//	FVector RetLocation = FVector::ZeroVector;
//
//	// 소환자 액터 가져오기
//	AActor* Avatar = AbilitySystemComponent.IsValid() ? AbilitySystemComponent->GetAvatarActor() : nullptr;
//	if (!Avatar)
//	{
//		return RetLocation;
//	}
//
//	// #1: 기준 위치 구하기
//	const FVector OwnerLocation = Avatar->GetActorLocation();
//
//	// #2: 소환 각도 구하기
//	const float RandomAngle = FMath::FRandRange(0.f, 2.f * PI);
//
//	// #3: 소환 가능 범위 면적 구하기
//	const float MinRange = FMath::Max(0.f, CachedMinSpawnRadius);
//	const float MaxRange = FMath::Max(MinRange, CachedMaxSpawnRadius);
//	const float Range = FMath::FRand();
//	const float RandomRadius = FMath::Sqrt(Range * (MaxRange * MaxRange - MinRange * MinRange) + MinRange * MinRange);
//
//	// #4: 로컬 오프셋 구하기 (XY 평면)
//	const float CosA = FMath::Cos(RandomAngle);
//	const float SinA = FMath::Sin(RandomAngle);
//	FVector LocalOffset(RandomRadius * CosA, RandomRadius * SinA, 0.f);
//
//	// #5: 회전값 구하기 (회전은 Yaw만 반영)
//	const float Yaw = Avatar->GetActorRotation().Yaw;
//	const FRotator YawRot(0.f, Yaw, 0.f);
//	const FVector WorldOffset = YawRot.RotateVector(LocalOffset);
//
//	// #6: 결과 위치 + 바닥에 끼임 방지를 위한 임시 Z값 보정
//	RetLocation = OwnerLocation + WorldOffset;
//	RetLocation.Z += 150.f;
//
//	return RetLocation;
//}

FVector UAT_WaitSpawnEnemies::GetSpawnLocationInRandRange()
{
	FVector RetLocation = FVector::ZeroVector;

	// 소환자 액터 가져오기
	AActor* Avatar = AbilitySystemComponent.IsValid() ? AbilitySystemComponent->GetAvatarActor() : nullptr;
	if (!Avatar)
	{
		return RetLocation;
	}

	// #1: 기준 위치(소환자 위치) 및 회전 구하기
	const FVector OriginLocation = Avatar->GetActorLocation();
	const FRotator YawRot(0.f, Avatar->GetActorRotation().Yaw, 0.f);

	// #2: 소환 가능 범위 계산
	const float MinExtent = FMath::Max(0.f, CachedMinSpawnExtent);
	const float MaxExtent = FMath::Max(MinExtent, CachedMaxSpawnExtent);

	// #3: 로컬 좌표계에서 박스 내 무작위 오프셋 계산
	// X, Y 각각 독립적으로 MinExtent와 MaxExtent 사이에서 무작위 값 선택
	const float RandomX = FMath::FRandRange(MinExtent, MaxExtent) * (FMath::RandBool() ? 1.f : -1.f);
	const float RandomY = FMath::FRandRange(MinExtent, MaxExtent) * (FMath::RandBool() ? 1.f : -1.f);
	FVector LocalOffset(RandomX, RandomY, 0.f);

	// #4: 월드 좌표계로 변환 (Yaw 회전 적용)
	const FVector WorldOffset = YawRot.RotateVector(LocalOffset);

	// #5: 결과 위치 계산 + 바닥 끼임 방지를 위한 Z 보정
	RetLocation = OriginLocation + WorldOffset;
	RetLocation.Z += 150.f;

	return RetLocation;
}

void UAT_WaitSpawnEnemies::SpawnFailed()
{
	// 적 소환 실패 델리게이트 브로드캐스트
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// 빈 배열 전달
		OnDidNotSpawn.Broadcast(TArray<APA_CharacterEnemy*>());
	}

	EndTask();
	return;
}

void UAT_WaitSpawnEnemies::OnEnemyClassLoaded()
{
	// 소환된 적 배열
	TArray<APA_CharacterEnemy*> SpanwedEnemies;

	// 스폰 파라미터 구조체
	// 항상 스폰, 충돌하지 않도록 위치 조정
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < CachedNumToSpawn; ++i)
	{
		// 소환할 적 클래스 배열에서 랜덤으로 소환할 인덱스를 얻음
		int32 RandIdx = FMath::RandRange(0, CachedSoftEnemyClassToSpawn.Num() - 1);

		// 로드된 클래스 저장 변수
		UClass* LoadedClass = CachedSoftEnemyClassToSpawn[RandIdx].Get();

		// 로드된 클래스가 없거나 월드가 존재하지 않는 경우
		if (!LoadedClass || !GetWorld())
		{
			// 적 소환 실패 함수 호출
			SpawnFailed();
		}

		// 스폰 위치값(랜덤 범위)
		FVector RandomLocation = GetSpawnLocationInRandRange();

		// 시전자의 전방 벡터 기준 오리진 회전값
		const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();

		// 적 스폰
		APA_CharacterEnemy* SpawnedEnemy = GetWorld()->SpawnActor<APA_CharacterEnemy>(
			LoadedClass,
			RandomLocation,
			SpawnFacingRotation,
			SpawnParams
		);

		// 소환된 적 배열에 추가
		if (SpawnedEnemy)
		{
			SpanwedEnemies.Add(SpawnedEnemy);
		}
	}

	// 소환 완료 / 소환 실패 델리게이트 브로드캐스트
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!SpanwedEnemies.IsEmpty())
		{
			OnSpawnFinished.Broadcast(SpanwedEnemies);
		}
		else
		{
			OnDidNotSpawn.Broadcast(TArray<APA_CharacterEnemy*>());
		}
	}
	EndTask();
}
