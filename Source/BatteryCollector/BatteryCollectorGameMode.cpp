// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollector.h"
#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "Blueprint/UserWidget.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//set base decay rate
	decayRate = 0.01f;
}


void ABatteryCollectorGameMode::BeginPlay(){
	Super::BeginPlay();

	CurrentState = EBatteryPlayState::EPlaying;

	//set the score to beat
	ABatteryCollectorCharacter *myCharacter =
		Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if(myCharacter){
		PowerToWin = myCharacter->getInitialPower() * 1.25f;

		//Start Character at random position (Wrong place to put it, but constructor overrides these values)
		FVector randPos = myCharacter->GetActorLocation();
		randPos.X = FMath::RandRange(-257.0f, 4257.0f);
		randPos.Y = FMath::RandRange(-1390.0f, 2180.0f);

		myCharacter->SetActorLocation(randPos);
	}

	if(HUDWidgetClass != nullptr){
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if(CurrentWidget != nullptr){
			CurrentWidget->AddToViewport();
		}
	}
}


void ABatteryCollectorGameMode::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	//Check that we are using the battery collector character
	ABatteryCollectorCharacter *myCharacter = 
		Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if(myCharacter){
		//If power is greater than winning threshold, set game state to win
		if(myCharacter->getCurrentPower() > PowerToWin){
			CurrentState = EBatteryPlayState::EWin;
		}
		//Check if character power is positive
		else if(myCharacter->getCurrentPower() > 0){
			//Decrease the character's power using the decay rate
			myCharacter->UpdatePower(-DeltaTime * decayRate * (myCharacter->getInitialPower()));
		}
		else{
			CurrentState = EBatteryPlayState::EGameOver;
		}
	}

	//Log a debug message
	FVector pos = myCharacter->GetActorLocation();

	FString pickupDebugString = "X: " + FString::SanitizeFloat(pos.X)
		+ "\nY: " + FString::SanitizeFloat(pos.Y) + "\nZ: " + FString::SanitizeFloat(pos.Z);

	UE_LOG(LogClass, Log, TEXT("character at pos:\n %s"), *pickupDebugString);
}