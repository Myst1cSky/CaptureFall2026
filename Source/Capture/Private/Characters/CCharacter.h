// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "CCharacter.generated.h"

UCLASS()
class ACCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACCharacter();
	
	void ServerSideInit();
	void ClientSideInit();

	bool IsLocallyControlledByPlayer() const;
	void PossessedBy(AController* NewController);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//------------------------------------------------------------//
//                   Gameplay Ability                         //
//------------------------------------------------------------//	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
private:
	void BindGASDelegates();
	
	void DeathTagUpdated(const FGameplayTag Tag, int32 Count);
	
	bool bGASDelegateBound;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Ability System")
	class UCAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY()
	class UCAttributeSet* CAttributeSet;
	
	//------------------------------------------------------------//
	//                  Death  & Respawn Sequence                            //
	//------------------------------------------------------------//
private:
	void StartDeathSequence();
	void Respawn();
	bool IsDead() const;
	
	void SetRagdollEnabled(bool bEnabled);
	
	FTransform SkeletalMeshRelativeTransform;
	
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float DeathAnimationTimeOffset = -1.f;
	
	void PlayDeathMontage();
	void DeathAnimationFinished();

	FTimerHandle DeathAnimationTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	UAnimMontage* RespawnMontage;
	
	
	//------------------------------------------------------------//
	//                   Widget                                   //
	//------------------------------------------------------------//	
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	class UWidgetComponent* OverheadWidgetComponent;
	
	void ConfigureOverheadWidgetComponent();
	
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
