// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// the below functions are the native overrides for each phase
	// Native initialization override point
	virtual void NativeInitializeAnimation() override;
	// Native update override point. It is usually a good idea to simply gather data in this step and
	// for the bulk of the work to be done in NativeThreadSafeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, meta =(BlueprintThreadSafe))
	FORCEINLINE bool IsMoving() const { return Speed > 0;}
	
	UFUNCTION(BlueprintCallable, meta =(BlueprintThreadSafe))
	FORCEINLINE bool IsNotMoving() const { return Speed == 0;}
	
	UFUNCTION(BlueprintCallable, meta =(BlueprintThreadSafe))
	FORCEINLINE bool IsOnGround() const { return !bIsFalling; }
private:
	UPROPERTY()
	class ACharacter* OwningCharacter;
	
	UPROPERTY()
	class UCharacterMovementComponent* OwningCharacterMovementComponent;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float Speed;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bIsFalling;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float YawSpeed;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float SmoothedYawSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float YawSpeedSmoothLerpRate = 2.f;
	
	FRotator BodyPrevRotation;
};
