// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/PA_PlayerLinkedAnimLayer.h"
#include "AnimInstances/PA_PlayerAnimInstance.h"

UPA_PlayerAnimInstance* UPA_PlayerLinkedAnimLayer::GetPlayerAnimInstance() const
{
	return Cast<UPA_PlayerAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
