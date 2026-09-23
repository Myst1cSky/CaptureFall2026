// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GC_HitCameraShake.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CPlayerController.h"

bool UGC_HitCameraShake::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	UE_LOG(LogTemp, Warning, TEXT("Triggering C++ Gameplay Cue"))
	
	APlayerController* TargetPlayerController = Cast<ACPlayerController>(MyTarget->GetOwner());
	if (TargetPlayerController)
	{
		TargetPlayerController->ClientStartCameraShake(CameraShake);
	}
	
	return true;
}
