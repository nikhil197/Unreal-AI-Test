// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_StartFiring.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UBTTask_StartFiring : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BehaviorComponent, uint8* NodeMemory);
};
