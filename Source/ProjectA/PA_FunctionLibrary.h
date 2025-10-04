#pragma once

#include "CoreMinimal.h"
#include "ProjectATypes/PA_EnumTypes.h"
#include "PA_FunctionLibrary.generated.h"

UCLASS()
class PROJECTA_API UPA_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 폰 전투 컴포넌트 가져오기
	static class UPA_PawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);
	
	// 액터의 ASC 가져오기
	static class UPA_AbilitySystemComponent* NativeGetPAAbilitySystemComponentFromActor(AActor* InActor);

	// 액터의 특정 태그 보유 여부
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	// 막기 유효 여부 반환 함수
	static bool NativeIsValidBlock(AActor* InAttacker, AActor* InVictim);

	// 무적 여부 반환 함수
	static bool NativeIsInvincible(AActor* InActor);

public:
	// 액터에 태그 추가하는 함수 (중복인 경우 패스)
	UFUNCTION(BlueprintCallable, Category = "Custom | FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag InTag);

	// 액터에 태그 제거하는 함수 (없는 경우 패스)
	UFUNCTION(BlueprintCallable, Category = "Custom | FunctionLibrary")
	static void RemoveGameplayTagToActorIfFound(AActor* InActor, FGameplayTag InTag);

	// 액터의 특정 태그 보유 여부
	UFUNCTION(BlueprintCallable, Category = "Custom | FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag InTag, EPA_ConfirmType& OutConfirmType);

	// 폰 전투 컴포넌트 가져오기
	UFUNCTION(BlueprintCallable, Category = "Custom | FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static class UPA_PawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EPA_ValidType& OutValidType);

	// 일반 팀 ID를 통해, 대상이 적대적인지 확인하는 함수
	UFUNCTION(BlueprintCallable, Category = "Custom | FunctionLibrary")
	static bool IsTargetPawnHostile(const APawn* MyPawn, const APawn* TargetPawn);

	// 피격 액션 방향 구하는 함수 [0: 앞쪽 피격, 1: 왼쪽 피격, 2: 오른쪽 피격, 3: 뒤쪽 피격]
	UFUNCTION(BlueprintPure, Category = "Custom | FunctionLibrary")
	static int ComputeHitReactDirection(AActor* InAttacker, AActor* InVictim);
	
	// 타깃에게 게임플레이 이펙트 스펙 핸들 적용하는 함수
	UFUNCTION(BlueprintPure, Category = "Custom | FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	// 공격 충돌 HitResult 반환 함수
	UFUNCTION(BlueprintPure, Category = "Custom | FunctionLibrary")
	static FHitResult GetAttackHitResult(AActor* InAttacker, AActor* InVictim);

};
