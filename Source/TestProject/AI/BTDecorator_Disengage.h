// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_Disengage.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UBTDecorator_Disengage : public UBTDecorator
{
	GENERATED_BODY()

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, Category = Condition)
	struct FBlackboardKeySelector EnemyKey;
};
