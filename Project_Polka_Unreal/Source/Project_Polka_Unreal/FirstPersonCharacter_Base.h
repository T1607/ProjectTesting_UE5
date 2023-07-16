#pragma once
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FirstPersonCharacter_Base.generated.h"

UCLASS()
class PROJECT_POLKA_UNREAL_API AFirstPersonCharacter_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFirstPersonCharacter_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "ClimbAndVaultingCheck")
	virtual void CheckForClimbingOrVaulting
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
	);

	UFUNCTION(BlueprintCallable, Category = "ClimbAndVaultingPositionValidity")
	virtual void CheckPlayerFitAndWalkability
	(
		bool drawDebug,
		FHitResult positionHit,
		float debugDuration,
		UCapsuleComponent* capsuleComp,
		UCharacterMovementComponent* characterMovementComp,
		FHitResult& positionSweepHitReturn,
		bool& isWalkableReturnValue,
		bool& canFitReturnvalue
	);
};
