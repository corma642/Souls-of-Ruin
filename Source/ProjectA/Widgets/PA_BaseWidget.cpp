// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PA_BaseWidget.h"
#include "Interface/PA_PawnUIInterface.h"
#include "Components/UI/PA_PlayerUIComponent.h"
#include "Components/UI/PA_EnemyUIComponent.h"

void UPA_BaseWidget::InitEnemyCreatedWidget(AActor* OwningEnemyActor)
{
	// 적 캐릭터로부터 UI 인터페이스를 가져옴
	if (IPA_PawnUIInterface* PawnUIInterface = Cast<IPA_PawnUIInterface>(OwningEnemyActor))
	{
		// UI 인터페이스를 통해 적 UI 컴포넌트를 가져옴
		UPA_EnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();
		check(EnemyUIComponent);

		// 가져온 적 UI 컴포넌트로 초기화
		BP_OnOwingEnemyUIComponentInitialized(EnemyUIComponent);
	}
}

void UPA_BaseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 소유자로부터 UI 인터페이스를 가져옴
	if (IPA_PawnUIInterface* PawnUIInterface = Cast<IPA_PawnUIInterface>(GetOwningPlayerPawn()))
	{
		// UI 인터페이스를 통해 플레이어 UI 컴포넌트를 가져옴
		UPA_PlayerUIComponent* PlayerUIComponent = PawnUIInterface->GetPlayerUIComponent();
		check(PlayerUIComponent);

		// 가져온 플레이어 UI 컴포넌트로 초기화
		BP_OnOwingPlayerUIComponentInitialized(PlayerUIComponent);
	}
}
