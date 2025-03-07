// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PA_GameInstance.generated.h"

UCLASS()
class PROJECTA_API UPA_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
};
