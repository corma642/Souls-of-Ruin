// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Input/DA_InputConfig.h"

UInputAction* UDA_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	if (!InInputTag.IsValid()) return nullptr;

	// 기본 입력 액션 배열을 순회
	for (const FInputActionConfig& InputActionConfig : NativeInputActions)
	{
		// 해당 입력 액션 구성이 유효하고, 태그가 찾으려는 태그와 일치하면, 해당 입력 액션을 반환
		if (InputActionConfig.IsValid() && InputActionConfig.InputTag == InInputTag)
		{
			return InputActionConfig.InputAction;
		}
	}
	return nullptr;
}
