#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PA_StructTypes.generated.h"

// 아이템 데이터
UCLASS(BlueprintType, Blueprintable)
class UItemStaticData : public UObject
{
	GENERATED_BODY()

public:
	//// 아이템 이름
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//FName Name;

	//// 아이템 액터 클래스
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TSubclassOf<class AItemActor> ItemActorClass;

	//// 아이템 부착 소켓 이름
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//FName AttachmentSocket = NAME_None;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//bool bCanBeEquipped = true;

	//// 아이템이 가지는 애님 데이터
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//FCharacterAnimationData CharacterAnimationData;

	//// 아이템 장착 시, 부여되는 어빌리티
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	//// 아이템 장착 시, 적용되는 게임플레이 이펙트
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TArray<TSubclassOf<UGameplayEffect>> OngoingEffects;

	//// 인벤토리 태그 목록
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TArray<FGameplayTag> InventoryTags;

	//// 최대 보관 스택
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//int32 MaxStackCount = 1;
};
