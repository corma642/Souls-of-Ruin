// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/GA_Player_TargetLockOn.h"
#include "Characters/PA_CharacterPlayer.h"
#include "Controllers/PA_PlayerController.h"
#include "Widgets/PA_BaseWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PA_FunctionLibrary.h"
#include "EnhancedInputSubsystems.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"

#include "Components/SizeBox.h"

#include "PA_GameplayTags.h"

void UGA_Player_TargetLockOn::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 락온 시도 함수 호출
	TryLockOnTarget();

	// 락온 입력 맵핑 콘텍스트 초기화
	InitTargetLockOnMappingContext();
}

void UGA_Player_TargetLockOn::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 데이터 정리 함수 호출
	ClaenUp();

	// 락온 입력 맵핑 콘텍스트 제거
	ResetTargetLockOnMappingContext();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Player_TargetLockOn::TryLockOnTarget()
{
	// 고정 대상 확보 함수 호출
	GetAvailableActorsToLock();

	// 고정 가능 대상이 비어있으면, 락온 취소
	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockOnAbility();
		return;
	}

	// 고정 대상 중, 플레이어와 가장 근접한 액터를 구하기
	CurrentLockedActor = GetNearestTargetFromAvailableActors(GetPlayerCharacterFromActorInfo(), AvailableActorsToLock);

	if (CurrentLockedActor)
	{
		// 타깃 락온 위젯 그리기
		DrawTargetLockOnWidget();

		// 타깃 락온 위젯 위치 설정하기
		SetTargetLockOnWidgetPosition();
	}
	else
	{
		// 내부 데이터 정리
		CancelTargetLockOnAbility();
		return;
	}
}

void UGA_Player_TargetLockOn::GetAvailableActorsToLock()
{
	// 이전 타깃 고정 가능 액터 배열 초기화
	AvailableActorsToLock.Empty();

	// 충돌 감지 배열
	TArray<FHitResult> HitResults;

	APA_CharacterPlayer* Player = GetPlayerCharacterFromActorInfo();
	APA_PlayerController* Controller = GetPlayerControllerFromActorInfo();

	// 카메라(View) 위치 및 회전 가져오기
	FVector CameraLocation;
	FRotator CameraRotation;
	Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// 충돌 무시할 액터
	TArray<AActor*> ActorsToIgnore;
	if (AActor* OwnerActor = GetPlayerCharacterFromActorInfo())
	{
		ActorsToIgnore.Add(OwnerActor);
	}

	// 플레이어 전방으로 박스 트레이싱
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		Controller,
		CameraLocation,
		CameraLocation + (CameraRotation.Vector() * BoxTraceDistance),
		TraceBoxSize / 2.0f,
		CameraRotation,
		BoxTraceChannel,
		false,
		ActorsToIgnore,
		bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		HitResults,
		true
	);

	// 충돌한 액터들 순회
	for (const FHitResult& HitResult : HitResults)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			// 충돌한 액터가 적대적인 대상인지 확인 후, 배열에 저장
			if (UPA_FunctionLibrary::IsTargetPawnHostile(GetPlayerCharacterFromActorInfo(), Cast<APawn>(HitActor)))
			{
				AvailableActorsToLock.AddUnique(HitActor);
			}
		}
	}
}

void UGA_Player_TargetLockOn::CancelTargetLockOnAbility()
{
	// 어빌리티 취소
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UGA_Player_TargetLockOn::ClaenUp()
{
	// 타깃 고정 가능 액터 배열 초기화
	AvailableActorsToLock.Empty();

	// 현재 락온 중인 액터 비우기
	CurrentLockedActor = nullptr;

	// 타깃 락온 위젯 제거
	if (DrawnTargetLockOnWidget)
	{
		DrawnTargetLockOnWidget->RemoveFromParent();
		DrawnTargetLockOnWidget = nullptr;
	}

	// 타깃 락온 위젯 크기를 0으로 초기화
	TargetLockOnWidgetSize = FVector2D::ZeroVector;
}

AActor* UGA_Player_TargetLockOn::GetNearestTargetFromAvailableActors(const AActor* TargetActor, const TArray<AActor*>& InVailableActors)
{
	// 원점에서 반환된 액터까지의 거리
	float ClosestDistance = 0.0f;

	// 타겟의 위치로부터 가장 가까운 대상을 찾아 반환
	return UGameplayStatics::FindNearestActor(
		TargetActor->GetActorLocation(),
		InVailableActors,
		ClosestDistance
	);
}

void UGA_Player_TargetLockOn::DrawTargetLockOnWidget()
{
	// 중복 그리기 방지
	if (DrawnTargetLockOnWidget) return;

	// CreateWidget 함수를 통해 플레이어 컨트롤러에 락온 위젯을 만들고 이를 저장
	check(TargetLockOnWidgetClass);
	DrawnTargetLockOnWidget = CreateWidget<UPA_BaseWidget>(GetPlayerControllerFromActorInfo(), TargetLockOnWidgetClass);

	// 저장된 락온 위젯을 화면에 그림
	check(DrawnTargetLockOnWidget);
	DrawnTargetLockOnWidget->AddToViewport();
}

void UGA_Player_TargetLockOn::SetTargetLockOnWidgetPosition()
{
	// 타깃 락온 위젯이 없거나, 현재 락온 중인 액터가 없으면 어빌리티 취소
	if (!DrawnTargetLockOnWidget || !CurrentLockedActor)
	{
		CancelTargetLockOnAbility();
		return;
	}

	// 위젯의 재정의된 크기까지 고려하여 타깃 락온 위젯 크기 재정의
	if (TargetLockOnWidgetSize == FVector2D::ZeroVector)
	{
		// 위젯 내부 패널들에 접근하여 이를 순회
		DrawnTargetLockOnWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget)
			{
				// SizeBox에 접근하여, 재정의된 너비/높이를 저장
				if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				{
					TargetLockOnWidgetSize.X = FoundSizeBox->GetWidthOverride();
					TargetLockOnWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}
			}
		);
	}

	// 위젯의 화면 좌표
	FVector2D ScreenPosition;

	// 월드 객체의 위치를 기반으로 화면 좌표를 계산
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetPlayerControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ScreenPosition,
		true
	);

	// 화면 좌표에서 위젯 크기의 절반을 빼서 위젯의 중앙이 해당 좌표에 오도록 함
	// UMG는 기본적으로 위젯의 좌측 상단을 기준으로 해석하기 때문
	ScreenPosition -= (TargetLockOnWidgetSize / 2.0f);

	// 위젯의 뷰포트 위치를 설정
	// 이미 플레이어의 화면을 기준으로 계산했기 떄문에, DPI 스케일링 비활성화
	DrawnTargetLockOnWidget->SetPositionInViewport(ScreenPosition, false);
}

void UGA_Player_TargetLockOn::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight)
{
	// 락온 대상이 없으면 어빌리티 취소
	if (!CurrentLockedActor || AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockOnAbility();
		return;
	}

	const FVector PlayerLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();

	// 현재 락온 대상 -> 플레이어를 향하는 방향 벡터
	const FVector CurrentTargetLookAtPlayer = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	for (AActor*& AvailableActor : AvailableActorsToLock)
	{
		// 락온 가능 액터가 유호하지 않거나, 현재 락온 중인 액터라면 패스
		if (!AvailableActor || AvailableActor == CurrentLockedActor)
		{
			continue;
		}

		// 락온 가능한 락온 대상 -> 플레이어를 향하는 방향 벡터
		const FVector AvailableTargetLookAtPlayer = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

		// 현재 락온 대상에서 플레이어를 향하는 벡터
		// 새로 락온 가능한 대상에서 플레이어를 향하는 벡터
		// 두 벡터를 외적하여 좌/우 방향을 구함
		const FVector CrossResult = FVector::CrossProduct(CurrentTargetLookAtPlayer, AvailableTargetLookAtPlayer);

		// 외적 값에 따라 각 좌/우 배열에 저장
		if (CrossResult.Z < 0.0)
		{
			OutActorsOnLeft.AddUnique(AvailableActor);
		}
		else
		{
			OutActorsOnRight.AddUnique(AvailableActor);
		}
	}
}

void UGA_Player_TargetLockOn::InitTargetLockOnMappingContext()
{
	// 플레이어 컨트롤러로부터 LocalPlayer 가져옴
	const ULocalPlayer* LocalPlayer = GetPlayerControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	// 락온 입력 맵핑 콘텍스트 추가
	Subsystem->AddMappingContext(TargetLockOnMappingContext, 3);
}

void UGA_Player_TargetLockOn::ResetTargetLockOnMappingContext()
{
	// 락온 상태에서 급 종료 시, nullptr 값이 된 컨트롤러에 접근하게 되는 현상을 방지
	if (!GetPlayerControllerFromActorInfo()) return;

	// 플레이어 컨트롤러로부터 LocalPlayer 가져옴
	const ULocalPlayer* LocalPlayer = GetPlayerControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	// 락온 입력 맵핑 콘텍스트 제거
	Subsystem->RemoveMappingContext(TargetLockOnMappingContext);
}

void UGA_Player_TargetLockOn::OnTargetLockOnTick(float DeltaTime)
{
	// 타깃 락온이 가능한지 여부 판별
	if (!CanOnTargetLockOnTick())
	{
		CancelTargetLockOnAbility();
		return;
	}

	// 틱마다 타깃 고정 위젯 위치를 업데이트
	SetTargetLockOnWidgetPosition();

	// 틱마다 락온 대상을 향해 회전
	TargetLockOnRotate(DeltaTime);
}

bool UGA_Player_TargetLockOn::CanOnTargetLockOnTick()
{
	// 타깃 락온 대상이 없는 경우 실패 반환
	if (!CurrentLockedActor) return false;

	// 플레이어가 죽은 경우 실패 반환
	if (UPA_FunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), PA_GameplayTags::Shared_Status_Dead)) return false;

	// 락온 대상이 죽은 경우 실패 반환
	if (UPA_FunctionLibrary::NativeDoesActorHaveTag(CurrentLockedActor, PA_GameplayTags::Shared_Status_Dead)) return false;

	// 락온 대상이 너무 멀리 떨어져 있는 경우 실패 반환
	float Distance = FVector::Dist(GetPlayerCharacterFromActorInfo()->GetActorLocation(), CurrentLockedActor->GetActorLocation());
	if (Distance >= TargetLockOnKeepingDistance) return false;

	return true;
}

void UGA_Player_TargetLockOn::TargetLockOnRotate(float DeltaTime)
{
	APA_CharacterPlayer* Player = GetPlayerCharacterFromActorInfo();
	APA_PlayerController* Controller = GetPlayerControllerFromActorInfo();

	// 플레이어가 구르는 중에는 회전을 업데이트 해서는 안됨
	if (!UPA_FunctionLibrary::NativeDoesActorHaveTag(Player, PA_GameplayTags::Player_Status_Rolling))
	{
		// 락온 대상으로의 회전값
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
			Player->GetActorLocation(),
			CurrentLockedActor->GetActorLocation()
		);

		// 플레이어 컨트롤러의 회전값을 기준으로 회전 보간 값을 계산
		const FRotator TargetRot = FMath::RInterpTo(
			Controller->GetControlRotation(),
			LookAtRot,
			DeltaTime,
			TargetLockOnRotationInterpSpeed
		);

		// 회전값 업데이트
		Player->SetActorRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0));
		Controller->SetControlRotation(FRotator(0, TargetRot.Yaw, 0));
	}
}

void UGA_Player_TargetLockOn::ChangeLockOnTarget(const FGameplayTag& InChangeDirectionTag)
{
	// 플레이어 위치가 변경되었을 수 있으므로 새 고정 대상 확보
	GetAvailableActorsToLock();

	// 새로 변경할 락온 대상
	AActor* NewTargetToLock = nullptr;

	TArray<AActor*> ActorsOnLeft;	// 현재 락온 타깃 기준 왼쪽의 락온 가능 타깃 배열
	TArray<AActor*> ActorsOnRight;	// 현재 락온 타깃 기준 오른쪽의 락온 가능 타깃 배열

	// 락온 가능 대상 주변 좌/우 타깃 구하는 함수를 호출
	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	// 변경하려는 방향에 맞는 배열에서 가장 근접한 락온 대상을 찾아, 변경할 락온 대상 업데이트
	if (InChangeDirectionTag == PA_GameplayTags::Player_Event_ChangeLockOnTarget_Left)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(CurrentLockedActor, ActorsOnLeft);
	}
	else if (InChangeDirectionTag == PA_GameplayTags::Player_Event_ChangeLockOnTarget_Right)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(CurrentLockedActor, ActorsOnRight);
	}

	if (NewTargetToLock)
	{
		// 현재 락온 대상 업데이트
		CurrentLockedActor = NewTargetToLock;
	}
}
