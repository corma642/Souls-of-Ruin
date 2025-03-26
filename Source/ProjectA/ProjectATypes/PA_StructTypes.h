#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PA_StructTypes.generated.h"

// ������ ������
UCLASS(BlueprintType, Blueprintable)
class UItemStaticData : public UObject
{
	GENERATED_BODY()

public:
	//// ������ �̸�
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//FName Name;

	//// ������ ���� Ŭ����
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TSubclassOf<class AItemActor> ItemActorClass;

	//// ������ ���� ���� �̸�
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//FName AttachmentSocket = NAME_None;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//bool bCanBeEquipped = true;

	//// �������� ������ �ִ� ������
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//FCharacterAnimationData CharacterAnimationData;

	//// ������ ���� ��, �ο��Ǵ� �����Ƽ
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	//// ������ ���� ��, ����Ǵ� �����÷��� ����Ʈ
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TArray<TSubclassOf<UGameplayEffect>> OngoingEffects;

	//// �κ��丮 �±� ���
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TArray<FGameplayTag> InventoryTags;

	//// �ִ� ���� ����
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//int32 MaxStackCount = 1;
};
