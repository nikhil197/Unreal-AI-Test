// Fill out your copyright notice in the Description page of Project Settings.


#include "TestProjectGameModeBase.h"
#include "Player/EnemyPlayer.h"

ATestProjectGameModeBase::ATestProjectGameModeBase()
{
	DefaultPawnClass = AEnemyPlayer::StaticClass();
}