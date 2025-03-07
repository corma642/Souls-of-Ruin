// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectATypes/PA_EnumTypes.h"
#include "PA_GameModeBase.generated.h"


UCLASS()
class PROJECTA_API APA_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APA_GameModeBase();

	EPA_GameDifficulty const GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Game Settings")
	EPA_GameDifficulty CurrentGameDifficulty;
};
