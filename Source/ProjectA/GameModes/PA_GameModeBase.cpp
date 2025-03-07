// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/PA_GameModeBase.h"

APA_GameModeBase::APA_GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}
