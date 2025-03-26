// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnCombatInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPawnCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTA_API IPawnCombatInterface
{
	GENERATED_BODY()

public:
	virtual class UPA_PawnCombatComponent* GetPawnCombatComponent() const = 0;
};
