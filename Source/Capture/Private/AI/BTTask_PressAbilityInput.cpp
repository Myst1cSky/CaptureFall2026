// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_PressAbilityInput.h"
#include "AIController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"



EBTNodeResult::Type UBTTask_PressAbilityInput::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIOwner = OwnerComp.GetAIOwner();
	// The AI Character is the pawn of the controller
	if (AIOwner)
	{
		// The AI Character should have the AbilitySystemComponent
		UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AIOwner->GetPawn());
		if (OwnerASC)
		{
			OwnerASC->PressInputID((int32)InputToPress);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
