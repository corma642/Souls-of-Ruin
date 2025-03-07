// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstances/PA_GameInstance.h"
#include "AbilitySystemGlobals.h"

void UPA_GameInstance::Init()
{
	Super::Init();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
