// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AbilityGauge.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UAbilityGauge::NativeConstruct()
{
	Super::NativeConstruct();
	CooldownCounterText->SetVisibility(ESlateVisibility::Hidden);
	
	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());
	if (OwnerASC)
	{
		OwnerASC->AbilityCommittedCallbacks.AddUObject(this, &UAbilityGauge::AbilityCommitted);
		OwnerASC->AbilitySpecDirtiedCallbacks.AddUObject(this, &UAbilityGauge::AbilitySpecUpdated);
	}
}

void UAbilityGauge::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	UE_LOG(LogTemp, Warning, TEXT("Ability Gauge Created for ability: %s"), *(ListItemObject->GetName()));
	CachedAbilityCDO = Cast<UGameplayAbility>(ListItemObject);
	
	float CooldownDuration = GetStaticCooldownDuration();
	float Cost = GetStaticCost();
	
	CooldownDurationText->SetText(FText::AsNumber(CooldownDuration));
	CostText->SetText(FText::AsNumber(FMath::Abs(Cost)));
}

void UAbilityGauge::ConfigureWithWidgetData(const FAbilityWidgetData* AbilityWidgetData)
{
	if (Icon && AbilityWidgetData)
	{
		Icon->GetDynamicMaterial()->SetTextureParameterValue(IconTextureParamName, AbilityWidgetData->Icon.LoadSynchronous());
	}
}

float UAbilityGauge::GetStaticCooldownDuration() const
{
	if (!CachedAbilityCDO)
		return 0.f;
	
	const UGameplayEffect* CooldownEffect = CachedAbilityCDO->GetCooldownGameplayEffect();
	if (!CooldownEffect)
		return 0.f;
	
	float CooldownDuration = 0.f;
	if (CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1, CooldownDuration))
	{
		return CooldownDuration;
	}
	
	return 0.f;
}

float UAbilityGauge::GetStaticCost() const
{
	if (!CachedAbilityCDO)
		return 0.f;
	
	const UGameplayEffect* CostEffect = CachedAbilityCDO->GetCostGameplayEffect();
	if (!CostEffect || CostEffect->Modifiers.Num() == 0)
		return 0.f;
	
	float Cost = 0.f;
	if (CostEffect->Modifiers[0].ModifierMagnitude.GetStaticMagnitudeIfPossible(1, Cost))
	{
		return Cost;
	}
	
	return 0.f;
}

void UAbilityGauge::AbilityCommitted(UGameplayAbility* Ability)
{
	if (Ability->GetClass()->GetDefaultObject() == CachedAbilityCDO)
	{
		float CooldownTimeRemaining = 0.f;
		float CooldownDuration = 0.f;
		
		Ability->GetCooldownTimeRemainingAndDuration(Ability->GetCurrentAbilitySpecHandle(), Ability->GetCurrentActorInfo(),
			CooldownTimeRemaining, CooldownDuration);
		StartCooldown(CooldownTimeRemaining, CooldownDuration);
	}
}

void UAbilityGauge::StartCooldown(float CooldownTimeRemaining, float CooldownDuration)
{
	CachedCooldownDuration = CooldownDuration;
	CooldownDurationText->SetText(FText::AsNumber(CooldownDuration));
	CachedCooldownTimeRemaining = CooldownTimeRemaining;
	
	CooldownCounterText->SetVisibility(ESlateVisibility::Visible);
	
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UAbilityGauge::CooldownFinished, CooldownTimeRemaining);
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerUpdateHandle, this, &UAbilityGauge::UpdateCooldown, CooldownUpdateInterval, true, 0.f);
}

void UAbilityGauge::UpdateCooldown()
{
	CachedCooldownTimeRemaining -= CooldownUpdateInterval;
	
	int NumberDigits = CachedCooldownTimeRemaining >= 1.0 ? 0 : 2;
	FNumberFormattingOptions FormattingOptions = FNumberFormattingOptions().SetMaximumFractionalDigits(NumberDigits);
	CooldownCounterText->SetText(FText::AsNumber(CachedCooldownTimeRemaining));
	
	Icon->GetDynamicMaterial()->SetScalarParameterValue(CooldownPerceptParamName, 1.f-CachedCooldownTimeRemaining/CachedCooldownTimeRemaining);
}

void UAbilityGauge::CooldownFinished()
{
	CachedCooldownDuration = CachedCooldownTimeRemaining = 0.f;
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerUpdateHandle);
	CooldownCounterText->SetVisibility(ESlateVisibility::Hidden);
	Icon->GetDynamicMaterial()->SetScalarParameterValue(CooldownPerceptParamName, 1.f);
}

void UAbilityGauge::AbilitySpecUpdated(const struct FGameplayAbilitySpec& AbilitySpec)
{
	// React to spec changes like level.
}
