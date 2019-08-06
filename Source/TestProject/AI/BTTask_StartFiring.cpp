// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StartFiring.h"

#include "AI/Controller/SoldierAIController.h"
#include "AI/Soldier/AISoldierCharacter.h"

EBTNodeResult::Type UBTTask_StartFiring::ExecuteTask(UBehaviorTreeComponent& BehaviorComponent, uint8* NodeMemory)
{
	ASoldierAIController* AIController = Cast<ASoldierAIController>(BehaviorComponent.GetAIOwner());
	if (AIController)
	{
		AAISoldierCharacter* AISoldier = Cast<AAISoldierCharacter>(AIController->GetPawn());
		if (AISoldier)
		{
			AISoldier->OnStartFire();

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}