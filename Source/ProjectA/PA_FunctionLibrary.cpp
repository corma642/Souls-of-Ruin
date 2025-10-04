
#include "PA_FunctionLibrary.h"
#include "Interface/PA_PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"

#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "PA_GameplayTags.h"

UPA_PawnCombatComponent* UPA_FunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);

	if (IPA_PawnCombatInterface* PawnCombatInterface = Cast<IPA_PawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}
	return nullptr;
}

UPA_AbilitySystemComponent* UPA_FunctionLibrary::NativeGetPAAbilitySystemComponentFromActor(AActor* InActor)
{
	check(InActor);
	return Cast<UPA_AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

bool UPA_FunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UPA_AbilitySystemComponent* ASC = NativeGetPAAbilitySystemComponentFromActor(InActor);
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

bool UPA_FunctionLibrary::NativeIsValidBlock(AActor* InAttacker, AActor* InVictim)
{
	bool bIsValidBlock = false;

	// 피해자에게 "막는 중" 태그 보유중인지 검사
	const bool bIsVictimBlocking = NativeDoesActorHaveTag(InVictim, PA_GameplayTags::Player_Status_Blocking);

	// 공격자의 공격이 막기를 뚫는 공격인지 검사
	const bool bIsAttackUnblockable = NativeDoesActorHaveTag(InAttacker, PA_GameplayTags::Enemy_Status_UnBlockable);

	// 피해자가 막는 중이고, 공격자의 공격은 막기를 뚫지 못할 경우
	if (bIsVictimBlocking && !bIsAttackUnblockable)
	{
		// 공격자와 피해자의 두 전방 벡터의 내적을 구함
		const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InVictim->GetActorForwardVector());

		// -1이 완전히 마주 보는 상태, 0이 수직인 상태이니, -0.4정도면 막기 성공
		bIsValidBlock = DotResult < -0.4f;
	}

	return bIsValidBlock;
}

bool UPA_FunctionLibrary::NativeIsInvincible(AActor* InActor)
{
	if (!InActor) return false;

	// 액터의 무적 상태 태그 보유 여부를 반환
	return NativeDoesActorHaveTag(InActor, PA_GameplayTags::Shared_Status_Invincible);
}

void UPA_FunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag InTag)
{
	UPA_AbilitySystemComponent* ASC = NativeGetPAAbilitySystemComponentFromActor(InActor);

	// ASC에 해당 태그가 없는 경우
	if (ASC && !ASC->HasMatchingGameplayTag(InTag))
	{
		// ASC에 수동으로 태그를 추가
		ASC->AddLooseGameplayTag(InTag);
	}
}

void UPA_FunctionLibrary::RemoveGameplayTagToActorIfFound(AActor* InActor, FGameplayTag InTag)
{
	UPA_AbilitySystemComponent* ASC = NativeGetPAAbilitySystemComponentFromActor(InActor);

	// ASC에 해당 태그가 있는 경우
	if (ASC && ASC->HasMatchingGameplayTag(InTag))
	{
		// ASC에 수동으로 태그를 제거
		ASC->RemoveLooseGameplayTag(InTag);
	}
}

void UPA_FunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag InTag, EPA_ConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, InTag) ? EPA_ConfirmType::Yes : EPA_ConfirmType::No;
}

UPA_PawnCombatComponent* UPA_FunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EPA_ValidType& OutValidType)
{
	UPA_PawnCombatComponent* PawnCombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

	OutValidType = PawnCombatComponent ? EPA_ValidType::Valid : EPA_ValidType::InValid;
	if (PawnCombatComponent)
	{
		return PawnCombatComponent;
	}
	return nullptr;
}

bool UPA_FunctionLibrary::IsTargetPawnHostile(const APawn* MyPawn, const APawn* TargetPawn)
{
	if (!MyPawn || !TargetPawn) return false;

	// 두 폰의 일반 팀 ID를 캐스팅을 통해 가져옴
	const IGenericTeamAgentInterface* MyPawnTeamAgent = Cast<IGenericTeamAgentInterface>(MyPawn->GetController());
	const IGenericTeamAgentInterface* TargetPawnTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (MyPawnTeamAgent && TargetPawnTeamAgent)
	{
		// 두 폰의 일반 팀 ID의 동일 여부를 반환 ()
		return MyPawnTeamAgent->GetGenericTeamId() != TargetPawnTeamAgent->GetGenericTeamId();
	}

	return false;
}

int UPA_FunctionLibrary::ComputeHitReactDirection(AActor* InAttacker, AActor* InVictim)
{
	check(InAttacker || InVictim);

	// 피해자(공격 받는 액터)의 전방 벡터
	const FVector VictimForward = InVictim->GetActorForwardVector();

	// 공격자의 위치에서 피해자를 바라본 벡터(정규화)
	const FVector AttackerToVictim = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

	// 두 벡터의 내적 결과를 저장
	const float DotResult = FVector::DotProduct(VictimForward, AttackerToVictim);

	// Acos를 통해 코사인을 제거 (반환값은 라디안으로 나옴)
	const float Radian = FMath::Acos(DotResult);

	// 라디안 값을 각도로 변환 (0 ~ 180의 소수로 나오게 됨)
	float Degree = FMath::RadiansToDegrees(Radian);

	// 두 벡터를 외적한다.
	const FVector CrossResult = FVector::CrossProduct(VictimForward, AttackerToVictim);

	// 외적한 벡터의 Z값이 0보다 작으면, 왼쪽을 의미한다.
	if (CrossResult.Z <= 0.0f)
	{
		Degree *= -1;
	}

	// 구한 각도에 맞춰서 실행할 몽타주 번호 반환
	// 앞쪽에서 맞음
	if (Degree >= -45.f && Degree <= 45.f)	return 0;

	// 왼쪽에서 맞음
	if (Degree < -45.f && Degree >= -135.f)	return 1;

	// 오른쪽에서 맞음
	if (Degree > 45.f && Degree <= 135.f)	return 2;

	// 뒤쪽에서 맞음
	if (Degree < -135.f || Degree > 135.f)	return 3;

	return 0;
}

bool UPA_FunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	// 가해자와 피해자의 ASC를 가져옴
	UPA_AbilitySystemComponent* SourceASC = NativeGetPAAbilitySystemComponentFromActor(InInstigator);
	UPA_AbilitySystemComponent* TargetASC = NativeGetPAAbilitySystemComponentFromActor(InTargetActor);

	// 가해자의 ASC에서 ApplyGameplayEffectSpecToTarget 함수를 통해 입력받은 핸들을 타깃 ASC에 적용
	FActiveGameplayEffectHandle AtiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

	// 적용 결과를 반환
	return AtiveGameplayEffectHandle.WasSuccessfullyApplied();
}

FHitResult UPA_FunctionLibrary::GetAttackHitResult(AActor* InAttacker, AActor* InVictim)
{
	FHitResult Ret = FHitResult();

	if (!InAttacker || !InVictim) return Ret;

	APawn* HitPawn = Cast<APawn>(InVictim);

	// 트레이스로 공격 충돌 지점 HitResult 얻기
	FHitResult AttackHit;
	FVector Start = InAttacker->GetActorLocation(); // 또는 무기 끝점
	FVector End = HitPawn->GetActorLocation(); // 또는 충돌 지점 추정
	FCollisionQueryParams Params(NAME_None, true, InAttacker);
	Params.AddIgnoredActor(InAttacker);
	Params.AddIgnoredActor(InAttacker->Owner);

	bool bHit = InAttacker->GetWorld()->LineTraceSingleByChannel(AttackHit, Start, End, ECC_Pawn, Params);
	if (bHit)
	{
		return AttackHit;
	}

	return Ret;
}
