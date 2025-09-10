// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AT_ExecuteTaskOnTick.h"

UAT_ExecuteTaskOnTick::UAT_ExecuteTaskOnTick()
{
	// Tick 사용 활성화
	bTickingTask = true;
}

UAT_ExecuteTaskOnTick* UAT_ExecuteTaskOnTick::ExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	// 태스크 객체 생성 후 반환
	// 생성된 태스크를 관리할 Owner 어빌리티 설정
	UAT_ExecuteTaskOnTick* Node = NewAbilityTask<UAT_ExecuteTaskOnTick>(OwningAbility);

	// 작업 트리거 대기 활성화
	Node->ReadyForActivation();

	return Node;
}

void UAT_ExecuteTaskOnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	// 어빌리티 태스크의 델리게이트를 브로드캐스트 하기 전에 호출
	// 해당 어빌리티가 활성화 중인지 확인
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnAbilityTaskTick.Broadcast(DeltaTime);
	}
	else
	{
		EndTask();
	}
}
