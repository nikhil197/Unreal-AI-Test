#pragma once
#include "TestTypes.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	INVALID,

	/* AI is patrolling */
	PARTOLLING,

	/* AI is investigating a sound or some other stimulus */
	INVESTIGATING,

	/* AI is searching for the enemy */
	SEARCHING,

	/* AI is fighting the enemy */
	FIGHTING
};

//	For tag to differentiate sounds from different types of sources for AI
namespace ESoundSource
{
	enum Priority
	{
		// For sounds from unknown sources
		UNKNOWN = -1,

		// For sounds originating from any gun fire
		ENVIRONMENT = 0,
		
		// For sounds originating due to movement of player
		MOVEMENT,
		
		// For sounds originating due to firing of guns or other explosive sound sources
		FIRING
	};

	struct Source
	{
		FName Name;

		Priority Priority;
	};

	static const Source Unknown = { FName("Unknown"), Priority::UNKNOWN };

	static const Source GunFire = { FName("GunFire"), Priority::FIRING };

	static const Source Movement = { FName("Movement"), Priority::MOVEMENT };

	static const Source Enviroment = { FName("Enviroment"), Priority::ENVIRONMENT };
}

enum class EWeaponState
{
	IDLE,
	FIRING,
	RELOADING
};