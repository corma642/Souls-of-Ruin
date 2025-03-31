// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/PA_BaseAnimInstance.h"
#include "PA_PlayerLinkedAnimLayer.generated.h"

UCLASS()
class PROJECTA_API UPA_PlayerLinkedAnimLayer : public UPA_BaseAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	class UPA_PlayerAnimInstance* GetPlayerAnimInstance() const;
};
