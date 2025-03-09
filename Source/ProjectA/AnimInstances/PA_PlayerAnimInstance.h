// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/PA_BaseAnimInstance.h"
#include "PA_PlayerAnimInstance.generated.h"

UCLASS()
class PROJECTA_API UPA_PlayerAnimInstance : public UPA_BaseAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Reference")
	TObjectPtr<class APA_CharacterPlayer> OwningPlayerCharacter;
};
