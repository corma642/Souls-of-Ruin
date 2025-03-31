// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/GA_Player_LightAttackMaster.h"
#include "Characters/PA_CharacterPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UGA_Player_LightAttackMaster::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 콤보 타이머 초기화
	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);

	// 현재 콤보의 공격 몽타주 탐색
	UAnimMontage* PlayMontage = *AttackMontageMap.Find(CurrenCombo);
	if (!PlayMontage) return;

	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, PlayMontage);

	// 몽타주가 혼합, 완료, 중단, 취소되었을 때, EndAbility 호출
	MontageTask->OnBlendOut.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);
	MontageTask->OnCompleted.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);

	// 몽타주 태스크 활성화
	MontageTask->ReadyForActivation();

	bComboAttack = true;
}

void UGA_Player_LightAttackMaster::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ResetComboAttackCount();
	bComboAttack = false;
}

void UGA_Player_LightAttackMaster::NextComboAttack()
{
	if (bComboAttack)
	{
		// 현재 콤보가 최대 콤보가 아닌 경우
		if (CurrenCombo < AttackMontageMap.Num())
		{
			// 콤보 카운트 증가
			CurrenCombo++;
		}
		else
		{
			// 콤보 초기화
			ResetComboAttackCount();
		}
	}
	else
	{
		// 콤보 초기화
		ResetComboAttackCount();
	}
}

void UGA_Player_LightAttackMaster::ResetComboAttackCount()
{
	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			CurrenCombo = 1;
		}), 0.3f, false);
}
