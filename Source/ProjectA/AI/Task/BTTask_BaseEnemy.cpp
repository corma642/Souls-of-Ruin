// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_BaseEnemy.h"
#include "Characters/PA_CharacterEnemy.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_BaseEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* OwningPawn = AIController->GetPawn();
	if (!AIController || !OwningPawn) return EBTNodeResult::Failed;

	// 적 캐릭터 캐시 데이터가 유효하지 않은 경우
	if (!CachedEnemyCharacter.IsValid())
	{
		// 캐스팅을 통한 첫 캐시 데이터 저장
		CachedEnemyCharacter = Cast<APA_CharacterEnemy>(OwningPawn);
		
		// 캐스팅을 했음에도 불구하고 유효하지 않은 경우
		if (!CachedEnemyCharacter.IsValid())
		{
			// 태스크를 실패로 완료
			FinishExecute(false);
			return EBTNodeResult::Failed;
		}
	}

	// 태스크 실행 함수를 호출 (태스크 메인 로직)
	OnExecuteTask();

	// 태스크를 성공으로 완료
	FinishExecute(true);
	return EBTNodeResult::Succeeded;
}

void UBTTask_BaseEnemy::OnExecuteTask()
{
}
