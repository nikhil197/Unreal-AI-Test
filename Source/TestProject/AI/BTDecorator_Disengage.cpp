// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_Disengage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine.h"

#include "AI/Controller/SoldierAIController.h"
#include "AI/Soldier/AISoldierCharacter.h"
#include "Player/EnemyPlayer.h"
#include "Weapon/TestWeapon.h"

bool UBTDecorator_Disengage::CalculateRawConditionValue(UBehaviorTreeComponent& BehaviorComp, uint8* NodeMemory) const
{
	// Disengage conditions
	bool bNeedAmmo = true;
	bool bNeedHealth = true;

	UBlackboardComponent* BBComp = BehaviorComp.GetBlackboardComponent();
	ASoldierAIController* AIController = Cast<ASoldierAIController>(BehaviorComp.GetAIOwner());
	if (AIController && BBComp)
	{
		AAISoldierCharacter* AICharacter = Cast<AAISoldierCharacter>(AIController->GetPawn());
		AEnemyPlayer* Enemy = Cast<AEnemyPlayer>(BBComp->GetValueAsObject(EnemyKey.SelectedKeyName));
		if (AICharacter && Enemy)
		{
			float AIHealthPercent = AICharacter->GetCurrentHealth() / AICharacter->GetMaxHealth();
			float EnemyHealthPercent = Enemy->GetCurrentHealth() / Enemy->GetMaxHealth();

			bool bIsEnemyAlmostDead = EnemyHealthPercent <= 0.2f && EnemyHealthPercent >= 0.0f;

			// Check ammo condition
			const ATestWeapon* Weapon = AICharacter->GetCurrentWeapon();
			if (Weapon)
			{
				int32 CurrentAmmo = Weapon->GetCurrentAmmo() + Weapon->GetCurrentMagAmmo();
				if (CurrentAmmo == 0)
				{
					bNeedAmmo = true;
				}
				else if (CurrentAmmo > 0 && CurrentAmmo < Weapon->GetAmmoPerMag() * 2)
				{
					if (bIsEnemyAlmostDead)
					{
						bNeedAmmo = false;
					}
					else
					{
						bNeedAmmo = true;
					}
				}
				else if (CurrentAmmo >= Weapon->GetAmmoPerMag() * 2)
				{
					bNeedAmmo = false;
				}
			}

			// Check Health Condition
			// Do not disengage unless the AI's health is low or if both AI and player's health is low
			if (AIHealthPercent > 0 && AIHealthPercent <= 0.4f)
			{
				if (bIsEnemyAlmostDead)
				{
					bNeedHealth = false;
				}
				else
				{
					bNeedHealth = true;
				}
			}
			if (AIHealthPercent >= 0.4f)
			{
				bNeedHealth = false;
			}

			if (bNeedAmmo)
			{
				static FName KeyName = FName("NeedAmmo");
				BBComp->SetValueAsBool(KeyName, bNeedAmmo);
			}

			if (bNeedHealth)
			{
				static FName KeyName = FName("NeedHealth");
				BBComp->SetValueAsBool(KeyName, bNeedHealth);
			}
		}
	}

	if (bNeedAmmo)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Low On Ammo", true, 2.5f * FVector2D::UnitVector);
	}

	return bNeedAmmo || bNeedHealth;
}
