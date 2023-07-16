// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonCharacter_Base.h"
#include "string"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/EngineTypes.h"
#include "Camera/CameraActor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFirstPersonCharacter_Base::AFirstPersonCharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFirstPersonCharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFirstPersonCharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFirstPersonCharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFirstPersonCharacter_Base::CheckForClimbingOrVaulting
(
	bool drawDebug,
	float climbHeightMax,
	float climbHeightMin,
	float climbReach, 
	float climbingDebugDuration, 
	int lineTraceClimbResolution,
	UCapsuleComponent* capsuleComp,
	UCameraComponent* cameraComp,
	FHitResult& hitResult,
	bool& hitReturnValue
)
{
	//paragrams for initial linetrace forward and a temp float used in the linetrace comb
	float capsuleHalfHeight = capsuleComp->GetScaledCapsuleHalfHeight();
	FVector startForwardLine = (capsuleComp->GetComponentLocation() - FVector(0, 0, capsuleHalfHeight)) + FVector(0, 0, climbHeightMax);
	FVector endForwardLine = startForwardLine + (capsuleComp->GetForwardVector() * climbReach);
	FCollisionQueryParams collisionParams;
	float tempClimbReach;
	
	//debug
	if (drawDebug) { DrawDebugLine(GetWorld(), startForwardLine, endForwardLine, FColor::Red, false, climbingDebugDuration, 0, 1); }

	FHitResult forwardHit;
	if (GetWorld()->LineTraceSingleByChannel(forwardHit, startForwardLine, endForwardLine, ECC_Visibility, collisionParams))
	{
		//if hit, reduce length of linetrace comb
		//also add -1 buffer from walls
		tempClimbReach = ((forwardHit.Distance - climbReach) + climbReach) - 1;
	}
	else
	{
		tempClimbReach = climbReach;
	}

	//line trace comb loop
	FHitResult loopHit;
	for(int i = 0; i < lineTraceClimbResolution; i++)
	{
		//paragrams for linetrace comb
		//next tooth start position in comb
		FVector loopIterationAddition = (((tempClimbReach / lineTraceClimbResolution) * i) * capsuleComp->GetForwardVector());
		FVector startLoopLine = (loopIterationAddition + ((capsuleComp->GetComponentLocation() - FVector(0, 0, capsuleHalfHeight)) + FVector(0, 0, climbHeightMax)));
		FVector endLoopLine = ((capsuleComp->GetUpVector() * -1) * climbHeightMin) + startLoopLine;

		//debug
		if (drawDebug) { DrawDebugLine(GetWorld(), startLoopLine, endLoopLine, FColor::Green, false, climbingDebugDuration, 0, 1); }

		if (GetWorld()->LineTraceSingleByChannel(loopHit, startLoopLine, endLoopLine, ECC_Visibility, collisionParams))
		{
			//return value
			hitReturnValue = true;
			break;
		}
		else
		{
			//return value
			hitReturnValue = false;
		}
	}

	//return value
	hitResult = loopHit;
}

void AFirstPersonCharacter_Base::CheckPlayerFitAndWalkability
(
	bool drawDebug,
	FHitResult positionHit,
	float debugDuration,
	UCapsuleComponent* capsuleComp,
	UCharacterMovementComponent* characterMovementComp,
	FHitResult& positionSweepHitReturn,
	bool& isWalkableReturnValue,
	bool& canFitReturnvalue
) 
{
	//paragrams for sweep
	FQuat quatRotation = capsuleComp->GetComponentRotation().Quaternion();
	ECollisionChannel collisionChannel = ECC_Visibility;
	FCollisionShape collisionShape = capsuleComp->GetCollisionShape();
	FCollisionQueryParams collisionQueryParams;
	FCollisionResponseParams collisionResponseParams;
	//buffer from ground +2
	FVector capsuleCenter = positionHit.Location + FVector(0,0,capsuleComp->GetScaledCapsuleHalfHeight() + 2);

	//debug
	if (drawDebug) { DrawDebugCapsule(GetWorld(), capsuleCenter, capsuleComp->GetScaledCapsuleHalfHeight(), capsuleComp->GetScaledCapsuleRadius(), quatRotation, FColor::Cyan, false, debugDuration, 0, 1); }
	
	//check if position is walkable and if the player can fit in the position
	if (characterMovementComp->IsWalkable(positionHit))
	{
		FHitResult capsuleSweepHit;
		if (GetWorld()->SweepSingleByChannel
		(
			capsuleSweepHit,
			capsuleCenter,
			capsuleCenter,
			quatRotation,
			collisionChannel,
			collisionShape,
			collisionQueryParams,
			collisionResponseParams
		))
		{
			//return value
			canFitReturnvalue = false;
			positionSweepHitReturn = capsuleSweepHit;
		}
		else
		{
			//return value
			canFitReturnvalue = true;
		}
	
		//return value
		isWalkableReturnValue = true;
	}
	else
	{
		//return value
		isWalkableReturnValue = false;
	}
}

