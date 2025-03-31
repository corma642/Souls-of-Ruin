
#include "PA_FunctionLibrary.h"
#include "Interface/PawnCombatInterface.h"

#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UPA_PawnCombatComponent* UPA_FunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);

	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
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

	// ASC�� �ش� �±װ� ���� ���
	if (ASC && !ASC->HasMatchingGameplayTag(InTag))
	{
		// ASC�� �������� �±׸� �߰�
		ASC->AddLooseGameplayTag(InTag);
	}
}

void UPA_FunctionLibrary::RemoveGameplayTagToActorIfFound(AActor* InActor, FGameplayTag InTag)
{
	UPA_AbilitySystemComponent* ASC = NativeGetPAAbilitySystemComponentFromActor(InActor);

	// ASC�� �ش� �±װ� �ִ� ���
	if (ASC && ASC->HasMatchingGameplayTag(InTag))
	{
		// ASC�� �������� �±׸� ����
		ASC->RemoveLooseGameplayTag(InTag);
	}
}
