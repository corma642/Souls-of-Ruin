
#include "PA_FunctionLibrary.h"
#include "Interface/PA_PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"

#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

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
	return CastChecked<UPA_AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

bool UPA_FunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UPA_AbilitySystemComponent* ASC = NativeGetPAAbilitySystemComponentFromActor(InActor);
	return ASC->HasMatchingGameplayTag(TagToCheck);
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
	check(MyPawn || TargetPawn);

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
