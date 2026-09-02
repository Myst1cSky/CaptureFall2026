// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class UCAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const;
	
	ATTRIBUTE_ACCESSORS_BASIC(UCAttributeSet, Health);
	ATTRIBUTE_ACCESSORS_BASIC(UCAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS_BASIC(UCAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS_BASIC(UCAttributeSet, MaxMana);
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	
	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	
	UPROPERTY(ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	
	// This will be called on the client when MaxHealth is replicated from the server to the client.
	UFUNCTION()
	void OnRep_MaxHealth(FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_Health(FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_MaxMana(FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_Mana(FGameplayAttributeData& OldValue);
};
