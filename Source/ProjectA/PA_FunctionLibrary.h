#pragma once

#include "CoreMinimal.h"
#include "PA_FunctionLibrary.generated.h"

UCLASS()
class PROJECTA_API UPA_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 폰 전투 컴포넌트 가져오기
	static class UPA_PawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

};
