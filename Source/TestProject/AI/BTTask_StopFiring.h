// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StopFiring.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UBTTask_StopFiring : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BehaviorComponent, uint8* NodeMemory) override;
};
