// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PA_PawnCombatInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPA_PawnCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTA_API IPA_PawnCombatInterface
{
	GENERATED_BODY()

public:
	virtual class UPA_PawnCombatComponent* GetPawnCombatComponent() const = 0;
};
