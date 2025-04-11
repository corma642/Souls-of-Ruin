// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PA_PawnUIInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPA_PawnUIInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTA_API IPA_PawnUIInterface
{
	GENERATED_BODY()

public:
	virtual class UPA_PawnUIComponent* GetUIComponent() const = 0;
	virtual class UPA_PlayerUIComponent* GetPlayerUIComponent() const;
	virtual class UPA_EnemyUIComponent* GetEnemyUIComponent() const;
};
