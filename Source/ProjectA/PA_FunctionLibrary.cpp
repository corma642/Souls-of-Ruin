
#include "PA_FunctionLibrary.h"
#include "Interface/PA_PawnCombatInterface.h"

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
