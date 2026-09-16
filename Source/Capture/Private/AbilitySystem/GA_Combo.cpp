// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA_Combo.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/CAbilitySystemNativeTags.h"
#include "GameplayTagsManager.h"

UGA_Combo::UGA_Combo()
{
	AbilityTags.AddTag(TAG_ABILITY_BASICATTACK);
	BlockAbilitiesWithTag.AddTag(TAG_ABILITY_BASICATTACK);
}

void UGA_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle,ActorInfo,ActivationInfo, true, false);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Casting Combo Ability"))
	
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask = 
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ComboMontage);
		
		PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		
		PlayMontageAndWaitTask->ReadyForActivation();
		
		UAbilityTask_WaitGameplayEvent* WaitComboChangeEvent = 
			UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_ABILITY_COMBO_CHANGE, nullptr, false, false);
		
		WaitComboChangeEvent->EventReceived.AddDynamic(this, &UGA_Combo::HandleComboChange);
		
		WaitComboChangeEvent->ReadyForActivation();
	}
}

void UGA_Combo::HandleComboChange(FGameplayEventData EventData)
{
	FGameplayTag EventTag = EventData.EventTag;
	if (EventTag == TAG_ABILITY_COMBO_CHANGE_End)
	{
		NextComboName = NAME_None;
	
		UE_LOG(LogTemp, Warning, TEXT("Next Combo Name changed to None"));
		return;
	}
	
	TArray<FName> TagNames;
	UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, TagNames);
	NextComboName = TagNames.Last();
	
	UE_LOG(LogTemp, Warning, TEXT("Next Combo Name changed to: %s"), *(NextComboName.ToString()));
}
