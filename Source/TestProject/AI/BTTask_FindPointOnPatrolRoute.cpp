// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPointOnPatrolRoute.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine.h"

#include "AI/Controller/SoldierAIController.h"
#include "AI/Soldier/AISoldierCharacter.h"

EBTNodeResult::Type UBTTask_FindPointOnPatrolRoute::ExecuteTask(UBehaviorTreeComponent& BehaviorComponent, uint8* NodeMemory)
{
	ASoldierAIController* AIController = Cast<ASoldierAIController>(BehaviorComponent.GetAIOwner());
	if (AIController)
	{
		AAISoldierCharacter* Character = Cast<AAISoldierCharacter>(BehaviorComponent.GetAIOwner()->GetPawn());
		if (Character)
		{
			USplineComponent* PatrolPath = Character->GetPatrolPath();
			if (PatrolPath)
			{
				FVector NextPatrolPoint = PatrolPath->GetLocationAtSplinePoint(CurrentPatrolPointIndex, ESplineCoordinateSpace::Local);
				CurrentPatrolPointIndex = (++CurrentPatrolPointIndex) % PatrolPath->GetNumberOfSplinePoints();

				if (BehaviorComponent.GetBlackboardComponent())
				{
					BehaviorComponent.GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, NextPatrolPoint);
					AIController->SetFocalPoint(NextPatrolPoint);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}