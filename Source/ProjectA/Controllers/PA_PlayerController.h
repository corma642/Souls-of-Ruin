// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "PA_PlayerController.generated.h"

UCLASS()
class PROJECTA_API APA_PlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APA_PlayerController();

	// 팀 ID 가져오기 함수 재정의
	virtual FGenericTeamId GetGenericTeamId() const override { return PlayerTeamID; }

private:
	// 플레이어 팀 ID
	FGenericTeamId PlayerTeamID;
};
