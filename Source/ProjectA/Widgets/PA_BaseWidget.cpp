// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PA_BaseWidget.h"
#include "Interface/PA_PawnUIInterface.h"
#include "Components/UI/PA_PlayerUIComponent.h"
#include "Components/UI/PA_EnemyUIComponent.h"

void UPA_BaseWidget::InitEnemyCreatedWidget(AActor* OwningEnemyActor)
{
	// �� ĳ���ͷκ��� UI �������̽��� ������
	if (IPA_PawnUIInterface* PawnUIInterface = Cast<IPA_PawnUIInterface>(OwningEnemyActor))
	{
		// UI �������̽��� ���� �� UI ������Ʈ�� ������
		UPA_EnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();
		check(EnemyUIComponent);

		// ������ �� UI ������Ʈ�� �ʱ�ȭ
		BP_OnOwingEnemyUIComponentInitialized(EnemyUIComponent);
	}
}

void UPA_BaseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// �����ڷκ��� UI �������̽��� ������
	if (IPA_PawnUIInterface* PawnUIInterface = Cast<IPA_PawnUIInterface>(GetOwningPlayerPawn()))
	{
		// UI �������̽��� ���� �÷��̾� UI ������Ʈ�� ������
		UPA_PlayerUIComponent* PlayerUIComponent = PawnUIInterface->GetPlayerUIComponent();
		check(PlayerUIComponent);

		// ������ �÷��̾� UI ������Ʈ�� �ʱ�ȭ
		BP_OnOwingPlayerUIComponentInitialized(PlayerUIComponent);
	}
}
