// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_EntrySequenceBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

// UserWidget 관련
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// LevelSequence 관련
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"

TMap<TWeakObjectPtr<APlayerController>, TArray<FWidgetVisibilityRecord>> APA_EntrySequenceBase::CachedWidgetStates;

APA_EntrySequenceBase::APA_EntrySequenceBase()
	: SequenceActor(nullptr), SequencePlayer(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

	// 시퀀스 재생 콜리전 박스
	PlaySequenceCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlaySequenceCollisionBox"));
	RootComponent = PlaySequenceCollisionBox;
	PlaySequenceCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PlaySequenceCollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	PlaySequenceCollisionBox->SetGenerateOverlapEvents(true);
	PlaySequenceCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &APA_EntrySequenceBase::OnPlaySequenceCollisionBoxBeginOverlap);
}

void APA_EntrySequenceBase::OnPlaySequenceCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어 확인
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character && Character->IsPlayerControlled())
	{
		// 연출 재생
		PlayEntroSequence(Character);

		// 재오버랩 방지. 콜리전을 PhysicsOnly로 바꾸고 ObjectType 변경 (벽처럼)
		PlaySequenceCollisionBox->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		PlaySequenceCollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
		PlaySequenceCollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
}

void APA_EntrySequenceBase::PlayEntroSequence(ACharacter* PlayerCharacter)
{
	if (!PlayerCharacter || !PlaySequence)
	{
		return;
	}

	// #1: 시퀀스 설정 세팅 활성화
	APlayerController* Controller = Cast<APlayerController>(PlayerCharacter->GetController());
	SetUpSequenceSettings(Controller);

	// #2: 시퀀스 플레이어 생성 후 저장
	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bAutoPlay = false;

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), PlaySequence, Settings, SequenceActor);
	if (!SequencePlayer || !SequenceActor)
	{
		// 시퀀스 설정 세팅 비활성화
		UnSetUpSequenceSettings(Controller);
		return;
	}

	// #3: 시퀀스 재생 및 종료 콜백 바인딩
	SequencePlayer->OnFinished.AddDynamic(this, &APA_EntrySequenceBase::OnSequenceFinished);
	SequencePlayer->Play();
}

void APA_EntrySequenceBase::OnSequenceFinished()
{
	// 시퀀스 설정 세팅 비활성화
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	UnSetUpSequenceSettings(Controller);

	// 시퀀스 정리
	if (SequencePlayer)
	{
		SequencePlayer->Stop();
		SequencePlayer = nullptr;
	}
	if (SequenceActor)
	{
		SequenceActor->Destroy();
		SequenceActor = nullptr;
	}

	// 트리거 액터 정리
	//Destroy();
}

void APA_EntrySequenceBase::SetUpSequenceSettings(APlayerController* Controller)
{
	if (Controller)
	{
		// 입력 비활성화
		Controller->DisableInput(Controller);

		TArray<FWidgetVisibilityRecord> Records;
		TArray<UUserWidget*> AllWidgets;

		// 월드 전체에서 위젯 클래스를 찾아서 저장
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(Controller->GetWorld(), AllWidgets, UUserWidget::StaticClass(), false);

		for (UUserWidget* Widget : AllWidgets)
		{
			if (!Widget) continue;

			// 소유 플레이어가 동일한지 확인
			if (Widget->GetOwningPlayer() && Widget->GetOwningPlayer()->IsA(Controller->GetClass()))
			{
				// 위젯 가시화 기록 구조체에 가시화 정보 저장
				FWidgetVisibilityRecord Rec;
				Rec.Widget = Widget;
				Rec.PrevVisibility = Widget->GetVisibility();
				Widget->SetVisibility(ESlateVisibility::Collapsed);
				Records.Add(MoveTemp(Rec));
			}
		}

		// 가시화가 변경된 위젯이 있는 경우
		if (Records.Num() > 0)
		{
			// 변경 위젯 갱신 (이동으로 최적화)
			CachedWidgetStates.Add(Controller, MoveTemp(Records));
		}
	}
}

void APA_EntrySequenceBase::UnSetUpSequenceSettings(APlayerController* Controller)
{
	if (Controller)
	{
		// 입력 활성화
		Controller->DisableInput(Controller);

		TArray<FWidgetVisibilityRecord>* Found = CachedWidgetStates.Find(Controller);
		if (!Found) return;

		for (const FWidgetVisibilityRecord& Rec : *Found)
		{
			if (Rec.Widget.IsValid())
			{
				UUserWidget* W = Rec.Widget.Get();
				// 위젯이 여전히 존재하면 이전 상태로 복원
				W->SetVisibility(Rec.PrevVisibility);
			}
			// 만약 위젯이 파괴되었으면 무시
		}

		CachedWidgetStates.Remove(Controller);
	}
}
