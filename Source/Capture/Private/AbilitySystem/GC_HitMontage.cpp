// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GC_HitMontage.h"
#include "Components/StaticMeshComponent.h"

bool UGC_HitMontage::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	USkeletalMeshComponent* TargetSkeletalMesh =  MyTarget->GetComponentByClass<USkeletalMeshComponent>();
	if (TargetSkeletalMesh)
	{
		UAnimInstance* AnimInstance = TargetSkeletalMesh->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(MontageToPlay);
		}
	}
	
	return true;
}
