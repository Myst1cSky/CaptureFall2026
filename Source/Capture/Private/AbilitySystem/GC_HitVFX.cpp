// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GC_HitVFX.h"

bool UGC_HitVFX::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	UE_LOG(LogTemp, Warning, TEXT("Triggering C++ Version"));
	return true;
}
