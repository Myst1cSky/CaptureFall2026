// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindNextPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class UBTTask_FindNextPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
private:
	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category= Blackboard)
	struct FBlackboardKeySelector NextPatrolPointKey;
	
	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category= Blackboard)
	struct FBlackboardKeySelector PatrolCenterKey;
	
	UPROPERTY(EditAnywhere, Category= "Patrol")
	float FindRadius = 1000.f;
};
