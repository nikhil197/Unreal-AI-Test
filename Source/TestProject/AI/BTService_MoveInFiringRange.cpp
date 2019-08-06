// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_MoveInFiringRange.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "AI/Controller/SoldierAIController.h"
#include "AI/Soldier/AISoldierCharacter.h"
#include "Player/EnemyPlayer.h"
#include "Weapon/TestWeapon.h"

static const int32 MaxTries = 10;

/////////////////////////////////
// Service
///////////

UBTService_MoveInFiringRange::UBTService_MoveInFiringRange(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Find a Vantage Point";

	bNotifyBecomeRelevant = false;
	bNotifyBecomeRelevant = false;
}

void UBTService_MoveInFiringRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ASoldierAIController* AIController = Cast<ASoldierAIController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (AIController && BBComp)
	{
		AEnemyPlayer* Target = Cast<AEnemyPlayer>(BBComp->GetValueAsObject(BlackboardKey.SelectedKeyName));
		AAISoldierCharacter* AICharacter = Cast<AAISoldierCharacter>(AIController->GetPawn());
		if (AICharacter && Target)
		{
			const ATestWeapon* Weapon = AICharacter->GetCurrentWeapon();
			if (Weapon)
			{
				float WeaponRange = Weapon->GetWeaponRange();
				FVector VantagePoint = AICharacter->GetActorLocation();
				bool bHasLineOfSight = RunLineTraceToActor(AICharacter, Target, VantagePoint, Target->GetActorLocation());
				bool bIsInFiringRange = LiesInFiringRange(VantagePoint, Target->GetActorLocation(), WeaponRange);
				
				FVector Origin = Target->GetActorLocation();

				int32 TryCount = 0;

				while ((TryCount < MaxTries) && (!bHasLineOfSight || !bIsInFiringRange))
				{
					TryCount++;
					bHasLineOfSight = false;
					bIsInFiringRange = false;
					VantagePoint = FVector::ZeroVector;

					UNavigationSystemV1::K2_GetRandomReachablePointInRadius(GetWorld(), Origin, VantagePoint, WeaponRange);

					bHasLineOfSight = RunLineTraceToActor(AICharacter, Target, VantagePoint, Target->GetActorLocation());
					bIsInFiringRange = LiesInFiringRange(VantagePoint, Target->GetActorLocation(), WeaponRange);
				}

				if (bHasLineOfSight && bIsInFiringRange)
				{
					BBComp->SetValueAsVector(KeyToUpdate.SelectedKeyName, VantagePoint);
				}
			}
		}
	}
}

bool UBTService_MoveInFiringRange::RunLineTraceToActor(const AActor* Querier, const AActor* Target, const FVector& TraceStartLocation, const FVector& TraceEndLocation)
{
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(AILoSTrace), true, Querier);

	FHitResult HitResult(ForceInit);

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStartLocation, TraceEndLocation, ECC_GameTraceChannel1, TraceParams);
	GetWorld()->DebugDrawTraceTag = TraceParams.TraceTag;

	if (HitResult.bBlockingHit)
	{
		if (HitResult.Actor == Target)
		{
			return true;
		}
	}

	return false;
}

bool UBTService_MoveInFiringRange::LiesInFiringRange(const FVector& VantagePoint, const FVector& TargetLocation, float WeaponRange)
{
	float Distance = FMath::Abs(FVector::Distance(VantagePoint, TargetLocation));
	if (Distance >= 10.0f && Distance <= WeaponRange)
	{
		return true;
	}

	return false;
}