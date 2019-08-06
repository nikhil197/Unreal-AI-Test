// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestTypes.h"
#include "TestWeapon.generated.h"

UCLASS()
class TESTPROJECT_API ATestWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Destroy the weapon */
	virtual void Destroyed() override;

	//Initialise certain properties after the weapon object has been created
	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the player wants to fire
	void StartFire();

	// Called when player stops firing
	void StopFire();

	// Called when player wants to reload the weapon
	void StartReload();

	// Called when the player force stops the reload
	void StopReload();

	// Sets the current owner of the weapon
	void SetWeaponOwner(class ACharacter* NewOwner);

	// Add ammo to the weapon
	void AddAmmo(const int32 Amount);

	//Get the ammo in the current mag
	FORCEINLINE int32 GetCurrentMagAmmo() const { return CurrentMagAmmo; }

	//Get the ammo per mag
	FORCEINLINE int32 GetAmmoPerMag() const { return AmmoPerMag; }

	// Get the Max ammo that player can have for the weapon
	FORCEINLINE int32 GetMaxAmmo() const { return MaxAmmo; }

	// Get the current ammo the player has of this weapon
	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }

	// Get the current state of the weapon
	FORCEINLINE EWeaponState GetWeaponState() const { return CurrentWeaponState; }

	/* Get the Gun Mesh */
	FORCEINLINE class USkeletalMeshComponent* GetGunMesh() const { return WeaponMesh; }

	/* Returns whether the weapon if firing or not */
	inline bool IsFiring() const { return bWantsToFire && CurrentWeaponState == EWeaponState::FIRING; }

	/* Returns the range of the waepon */
	inline float GetWeaponRange() const { return WeaponRange; }

private:
	// Starts firing the weapon
	virtual void StartWeaponFire();

	// Stops the weapon fire
	virtual void StopWeaponFire();

	// Reloads the weapon
	virtual void ReloadWeapon();

	// Whether the weapon can fire or not
	bool CanFire() const;

	// Whether the weapon can reload or not
	bool CanReload() const;

	/* Checks the current state of the weapon and sets the new state */
	void CheckWeaponState();

	// Sets the new weapon state
	void SetWeaponState(EWeaponState NewState);

	/* Returns the random spread Angles for all the directions (i.e. x, y & z)*/
	const FVector GetRandomSpread(const FRotator& Rotation) const;

public:

protected:

	// Current owner of the weapon
	UPROPERTY(Transient)
	class ACharacter* WeaponOwner;

	// Ammo per each magazine
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Config", META = (AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0"))
	int32 AmmoPerMag;

	// Magazines at the start of the game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Config", META = (AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0"))
	int32 InitialMags;

	// Max ammo that the player can have for the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Config", META = (AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0"))
	int32 MaxAmmo;

	/* Rate of fire of the weapon (rounds per minute) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Config", META = (AllowPrivateAccess = "true", ClampMax = "900", UIMax = "900"))
	float RateOfFire;

	// Range of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Firing")
	float WeaponRange;

	/* Bullet spread when the player is firing while standing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Firing")
	float FiringSpreadRadius;

	/* Bullet Spread when the player is firing while targeting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Firing")
	float TargetingSpreadRadius;

	/* Bullet Spread when the player is firing while targeting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Firing")
	float MovingFireSpreadRadius;

	/* Bullet Spread when the player is firing from the hip (Crouch position) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Firing")
	float CrouchFireSpreadRadius;

	/* Bullet Spread when the player is firing while prone */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Firing")
	float ProneFireSpreadRadius;

	// Attach Point on the Player's Mesh when the weapon is equipped
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FName AttachPointOnEquip;

	// Attach Point on the Player's mesh when the weapon is un equipped (To be set in the subclasses)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FName AttachPointUnEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Firing")
	TSubclassOf<class ATestBullet> BulletClass;

private:
	/* Time Between two Shots (in seconds) */
	float TimeBetweenShots;

	// Current ammo in the magazine
	int32 CurrentMagAmmo;

	// Current ammo of the weapon
	int32 CurrentAmmo;

	// Current state of the weapon
	EWeaponState CurrentWeaponState;

	// Whether this weapon is equipped or not
	uint8 bIsEquipped : 1;

	// If the this weapon is being equipped or not
	uint8 bIsBeingEquipped : 1;

	// Whether the player wants to reload
	uint8 bWantsToReload : 1;

	// Whether the player wants to fire
	uint8 bWantsToFire : 1;

	//To Handle the Equip Finish Timer
	FTimerHandle TimerHandle_EquipFinish;

	//To handle the ReloadWeapon Timer
	FTimerHandle TimerHandle_ReloadWeapon;

	// To handle the automatic fire Timer
	FTimerHandle TimerHandle_AutomaticFire;

	/* Skeletal Mesh Component representing the gun in the game */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", META = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh;

	/* Muzzle Location where the bullets will spawn */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Muzzle", META = (AllowPrivateAccess = "true"))
	class USceneComponent* MuzzleLocation;

	/* Offset of the Muzzle location */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Muzzle", META = (AllowPrivateAccess = "true"))
	FVector MuzzleOffset;
};