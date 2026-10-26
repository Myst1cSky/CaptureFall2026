// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TA_GroundPick.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "Capture/Capture.h"
#include "Components/DecalComponent.h"
#include "Engine/OverlapResult.h"

ATA_GroundPick::ATA_GroundPick()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComp"));
	DecalComponent = CreateDefaultSubobject<UDecalComponent>("Decal Component");
	DecalComponent->SetupAttachment(RootComponent);
}

void ATA_GroundPick::ConfirmTargetingAndContinue()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(TargetAreaRadius)
		
		);
	
	TSet<AActor*> TargetActors;
	IGenericTeamAgentInterface* OwnerAgentInterface = nullptr;
	if (OwningAbility)
	{
		OwnerAgentInterface = Cast<IGenericTeamAgentInterface>(OwningAbility->GetAvatarActorFromActorInfo());
	}
	
	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		if (OwnerAgentInterface)
		{
			ETeamAttitude::Type OtherTeamAttitude = OwnerAgentInterface->GetTeamAttitudeTowards(*OverlapResult.GetActor());
			if (!TargetTeamAttitudes.Contains(OtherTeamAttitude))
			{
				continue;
			}
		}
		
		TargetActors.Add(OverlapResult.GetActor());
	}
	
	FGameplayAbilityTargetDataHandle TargetData =
		UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(TargetActors.Array(), false);
	
	FGameplayAbilityTargetData_SingleTargetHit* HitLocation = new FGameplayAbilityTargetData_SingleTargetHit;
	HitLocation->HitResult.ImpactPoint = GetActorLocation();
	HitLocation->HitResult.ImpactNormal = FVector::UpVector;
	
	TargetData.Add(HitLocation);
	
	TargetDataReadyDelegate.Broadcast(TargetData);
}

void ATA_GroundPick::SetTargetAreaRadius(float NewTargetAreaRadius)
{
	TargetAreaRadius = NewTargetAreaRadius;
	DecalComponent->DecalSize = FVector{TargetAreaRadius};
}

void ATA_GroundPick::SetTargetTraceRange(float NewTargetTraceRange)
{
	TargetTraceRange = NewTargetTraceRange;
}

void ATA_GroundPick::SetShouldDrawDebug(bool NewShouldDrawDebug)
{
	bShouldDrawDebug = NewShouldDrawDebug;
}

void ATA_GroundPick::AddTargetTeamAttitude(ETeamAttitude::Type NewTeamAttitude)
{
	TargetTeamAttitudes.Add(NewTeamAttitude);
}

void ATA_GroundPick::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (PrimaryPC && PrimaryPC->IsLocalPlayerController())
	{
		SetActorLocation(GetTargetPoint());
	}
}

FVector ATA_GroundPick::GetTargetPoint() const
{
	if (!PrimaryPC || !PrimaryPC->IsLocalPlayerController())
		return GetActorLocation();
	
	FVector ViewLocation;
	FRotator ViewRotation;
	
	PrimaryPC->GetPlayerViewPoint(ViewLocation, ViewRotation);
	FVector TraceEnd = ViewLocation + ViewRotation.Vector() * TargetTraceRange;
	
	FHitResult TraceResult;
	if (!GetWorld()->LineTraceSingleByChannel(TraceResult, ViewLocation, TraceEnd, ECC_Target))
	{
		GetWorld()->LineTraceSingleByChannel(TraceResult, TraceEnd, TraceEnd + FVector::DownVector * TNumericLimits<float>::Max(), ECC_Target);
	}
	
	if (!TraceResult.bBlockingHit)
	{
		return GetActorLocation();
	}
	
	if (bShouldDrawDebug)
	{
		DrawDebugSphere(GetWorld(), TraceResult.ImpactPoint, TargetAreaRadius, 32, FColor::Red);
	}
	
	return TraceResult.ImpactPoint;
}

