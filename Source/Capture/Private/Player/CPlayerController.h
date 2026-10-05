// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ACPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	// On Possess is only called on the server.
	virtual void OnPossess(APawn* NewPawn) override;
	
	// Called when clients/or listening server received their pawn on the client machine.
	// Not called on the dedicated server.
	virtual void AcknowledgePossession(class APawn* NewPawn) override;
	
private:
	UPROPERTY()
	class ACPlayerCharacter* CPlayerCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UGameplayWidget> GameplayWidgetClass;
	
	UPROPERTY()
	UGameplayWidget* GameplayWidget;
	
	void SpawnGameplayWidget();
	
	//----------------------------------------------------//
	//                      TEAM                          //
	//----------------------------------------------------//
	
public:
	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID);
	
	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const;

private:
	UPROPERTY(Replicated)
	FGenericTeamId TeamID;
};
