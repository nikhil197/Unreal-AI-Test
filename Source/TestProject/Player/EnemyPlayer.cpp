// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPlayer.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

#include "TestTypes.h"

// Sets default values
AEnemyPlayer::AEnemyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyPlayer::BeginPlay()
{
	Super::BeginPlay();

	MaxHitPoints = 100.0f;
	HitPoints = MaxHitPoints;
	
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(GetWorld(), UAISense_Hearing::StaticClass(), GetController());
	//UAIPerceptionSystem::RegisterPerceptionStimuliSource(GetWorld(), UAISense_Sight::StaticClass(), GetController());
}

// Called every frame
void AEnemyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AEnemyPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AEnemyPlayer::MoveRight);

	PlayerInputComponent->BindAction("MakeSound", EInputEvent::IE_Pressed, this, &AEnemyPlayer::MakeSound);
}

void AEnemyPlayer::MoveForward(float Amount)
{
	FRotator rotation = Controller->GetControlRotation();
	FVector direction = FRotationMatrix(rotation).GetUnitAxis(EAxis::X);
	AddMovementInput(direction, Amount);
}

void AEnemyPlayer::MoveRight(float Amount)
{
	FRotator rotation = Controller->GetControlRotation();
	FVector direction = FRotationMatrix(rotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(direction, Amount);
}

void AEnemyPlayer::MakeSound()
{
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, this, 10000.0f, ESoundSource::Movement.Name);
}
