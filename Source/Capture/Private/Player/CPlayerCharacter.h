// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CCharacter.h"
#include "AbilitySystem/CAbilityInputID.h"
#include "CPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ACPlayerCharacter : public ACCharacter
{
	GENERATED_BODY()
public:
	ACPlayerCharacter();
	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
	class UCameraComponent* ViewCam;
	
	//-----------------------------------------------------//
	//                     Input                          //
	//----------------------------------------------------//
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Input")
	TMap<ECAbilityInputID, class UInputAction*> GameplayAbilityInputActions;
	
	void HandleLookInput(const struct  FInputActionValue& InputActionValue);
	void HandleMoveInput(const struct  FInputActionValue& InputActionValue);
	
	void HandleAbilityInputAction(const FInputActionValue& InputActionValue, ECAbilityInputID InputID);
	
	FVector GetRightDirection() const;
	FVector GetLookFwdDirection() const;
	FVector GetMoveFwdDirection() const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* GameplayMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* JumpInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* LookInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveInputAction;
};
