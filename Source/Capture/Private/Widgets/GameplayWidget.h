// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta=(Binding))
	class UValueGauge* HealthBar;
	
	UPROPERTY(meta=(Binding))
	class UValueGauge* ManaBar;
};
