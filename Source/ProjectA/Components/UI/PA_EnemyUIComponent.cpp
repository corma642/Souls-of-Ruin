// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/PA_EnemyUIComponent.h"
#include "Widgets/PA_BaseWidget.h"

UPA_EnemyUIComponent::UPA_EnemyUIComponent()
{
	EnemyDrawnWidgets.Empty();
}

void UPA_EnemyUIComponent::RegisterEnemyDrawnWidgets(UPA_BaseWidget* InWidgetToRegister)
{
	// 그리는 위젯을 배열에 저장
	EnemyDrawnWidgets.AddUnique(InWidgetToRegister);
}

void UPA_EnemyUIComponent::RemoveEnemyDrawnWidgetsIfAny()
{
	// 적이 그린 위젯이 없으면 리턴
	if (EnemyDrawnWidgets.IsEmpty()) return;

	for (UPA_BaseWidget* DrawnWidget : EnemyDrawnWidgets)
	{
		if (DrawnWidget)
		{
			// 상위 위젯에서 위젯 제거
			DrawnWidget->RemoveFromParent();
		}
	}

	EnemyDrawnWidgets.Empty();
}
