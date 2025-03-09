// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PA_BaseAnimInstance.generated.h"

UCLASS()
class PROJECTA_API UPA_BaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Reference")
	TObjectPtr<class APA_CharacterBase> OwningCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Reference")
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MovementData")
	FVector Velocity;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MovementData")
	float GroundSpeed;

	// 이동 방향
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MovementData")
	float Direction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MovementData")
	bool bShoudMove;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MovementData")
	bool bIsFalling;
};
