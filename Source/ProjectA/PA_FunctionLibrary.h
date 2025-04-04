#pragma once

#include "CoreMinimal.h"
#include "ProjectATypes/PA_EnumTypes.h"
#include "PA_FunctionLibrary.generated.h"

UCLASS()
class PROJECTA_API UPA_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// �� ���� ������Ʈ ��������
	static class UPA_PawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);
	
	// ������ ASC ��������
	static class UPA_AbilitySystemComponent* NativeGetPAAbilitySystemComponentFromActor(AActor* InActor);

public:
	// ���Ϳ� �±� �߰��ϴ� �Լ� (�ߺ��� ��� �н�)
	UFUNCTION(BlueprintCallable, Category = "Custom | FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag InTag);

	// ���Ϳ� �±� �����ϴ� �Լ� (���� ��� �н�)
	UFUNCTION(BlueprintCallable, Category = "Custom | FunctionLibrary")
	static void RemoveGameplayTagToActorIfFound(AActor* InActor, FGameplayTag InTag);

	// �� ���� ������Ʈ ��������
	UFUNCTION(BlueprintCallable, Category = "Custom | FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static class UPA_PawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EPA_ValidType& OutValidType);


};
