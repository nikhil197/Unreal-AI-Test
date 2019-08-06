// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindCover.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UBTTask_FindCover : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BehaviorComponent, uint8* NodeMemory) override;

protected:
	/* Blackboard key that should be updated by the service */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector KeyToUpdate;
};
