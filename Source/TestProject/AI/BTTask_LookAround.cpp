// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LookAround.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

#include "AI/Controller/SoldierAIController.h"
#include "AI/Soldier/AISoldierCharacter.h"

UBTTask_LookAround::UBTTask_LookAround()
{
	BlackboardKey.AddVectorFilter(this, FName("UpdateKey"));
}

EBTNodeResult::Type UBTTask_LookAround::ExecuteTask(UBehaviorTreeComponent& BehaviorComponent, uint8* NodeMemory)
{
	ASoldierAIController* AIController = Cast<ASoldierAIController>(BehaviorComponent.GetAIOwner());
	if (AIController)
	{
		AAISoldierCharacter* AICharacter = Cast<AAISoldierCharacter>(AIController->GetPawn());
		if (AICharacter && BehaviorComponent.GetBlackboardComponent())
		{
			FVector Origin = BehaviorComponent.GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName);
			FVector EnemyDirection(0);
			const AActor* Enemy = Cast<AActor>(AIController->GetTarget());
			if (Enemy)
			{
				Origin = Enemy->GetActorLocation();
				EnemyDirection = Origin - AICharacter->GetActorLocation();
				EnemyDirection.Normalize();
			}

			FVector Destination(0);

			int32 TryCount;
			if (Enemy)
				TryCount = 1;
			else
				TryCount = 9;

			while (TryCount < 10)
			{
				UNavigationSystemV1::K2_GetRandomPointInNavigableRadius(GetWorld(), Origin, Destination, SearchRadius);
				if (Enemy)
				{
					FVector Direction = Destination - AICharacter->GetActorLocation();
					Direction.Normalize();
					float Dot = FMath::Abs(FVector::DotProduct(Direction, EnemyDirection));
					if (Dot <= 0.25881)
					{
						BehaviorComponent.GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Destination);
						AIController->ClearFocus(EAIFocusPriority::Move);
						AIController->SetFocalPoint(Destination);
						return EBTNodeResult::Succeeded;
					}

					Origin = Enemy->GetActorLocation();
				}
				else
				{
					break;
				}
				TryCount++;
			}

			if (Destination != FVector::ZeroVector)
			{
				BehaviorComponent.GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Destination);
				AIController->SetFocalPoint(Destination);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}