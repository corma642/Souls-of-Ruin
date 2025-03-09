// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/PA_PlayerAnimInstance.h"
#include "Characters/PA_CharacterPlayer.h"

void UPA_PlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter)
	{
		OwningPlayerCharacter = Cast<APA_CharacterPlayer>(OwningCharacter);
	}
}
