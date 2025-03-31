// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Input/DA_InputConfig.h"

UInputAction* UDA_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	if (!InInputTag.IsValid()) return nullptr;

	// �⺻ �Է� �׼� �迭�� ��ȸ
	for (const FInputActionConfig& InputActionConfig : NativeInputActions)
	{
		// �ش� �Է� �׼� ������ ��ȿ�ϰ�, �±װ� ã������ �±׿� ��ġ�ϸ�, �ش� �Է� �׼��� ��ȯ
		if (InputActionConfig.IsValid() && InputActionConfig.InputTag == InInputTag)
		{
			return InputActionConfig.InputAction;
		}
	}
	return nullptr;
}
