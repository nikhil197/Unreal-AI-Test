// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindCover.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Engine.h"

#include "AI/Controller/SoldierAIController.h"
#include "AI/Soldier/AISoldierCharacter.h"
#include "Player/EnemyPlayer.h"
#include "Weapon/TestWeapon.h"

static int32 MaxTries = 10;

bool RunLineTraceToActor(UWorld* World, const AActor* Querier, const AActor* Target, const FVector& TraceStartLocation, const FVector& TraceEndLocation)
{
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(AILoSTrace), true, Querier);

	FHitResult HitResult(ForceInit);

	World->LineTraceSingleByChannel(HitResult, TraceStartLocation, TraceEndLocation, ECC_GameTraceChannel1, TraceParams);
	World->DebugDrawTraceTag = TraceParams.TraceTag;

	if (HitResult.bBlockingHit)
	{
		if (HitResult.Actor == Target)
		{
			return true;
		}
	}

	return false;
}

EBTNodeResult::Type UBTTask_FindCover::ExecuteTask(UBehaviorTreeComponent& BehaviorComponent, uint8* NodeMemory)
{
	UBlackboardComponent* BBComp = BehaviorComponent.GetBlackboardComponent();
	ASoldierAIController* AIController = Cast<ASoldierAIController>(BehaviorComponent.GetAIOwner());
	if (BBComp && AIController)
	{
		AEnemyPlayer* Target = Cast<AEnemyPlayer>(BBComp->GetValueAsObject(BlackboardKey.SelectedKeyName));
		AAISoldierCharacter* AICharacter = Cast<AAISoldierCharacter>(AIController->GetPawn());
		if (AICharacter && Target)
		{
			float SearchRadius = 100000.0f;
			FVector CoverLocation = AICharacter->GetActorLocation();
			bool bHasLineOfSight = RunLineTraceToActor(GetWorld(), AICharacter, Target, CoverLocation, Target->GetActorLocation());

			FVector Origin = CoverLocation;

			int32 TryCount = 0;

			while ((TryCount < MaxTries) && bHasLineOfSight)
			{
				TryCount++;
				bHasLineOfSight = false;

				CoverLocation = FVector::ZeroVector;

				UNavigationSystemV1::K2_GetRandomReachablePointInRadius(GetWorld(), Origin, CoverLocation, SearchRadius);

				bHasLineOfSight = RunLineTraceToActor(GetWorld(), AICharacter, Target, CoverLocation, Target->GetActorLocation());
			}

			if (!bHasLineOfSight)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Looking For Cover", true, 2.5 * FVector2D::UnitVector);
				BBComp->SetValueAsVector(KeyToUpdate.SelectedKeyName, CoverLocation);
				AIController->SetFocalPoint(CoverLocation);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
