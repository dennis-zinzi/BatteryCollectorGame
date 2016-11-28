// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollector.h"
#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"

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

	//find all spawn volume Actors
	TArray<AActor*> foundActors;
	//Gets all objects of type AActor
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), foundActors);

	//Iterate through all AActor Objects
	for(auto Actor : foundActors){
		//Attempt to cast AActor to a SpawnVolume object
		ASpawnVolume *spawnVolumeActor = Cast<ASpawnVolume>(Actor);

		//Add any unique SpawnVolume (will be false if obj not castable) objects within array
		if(spawnVolumeActor){
			SpawnVolumeActors.AddUnique(spawnVolumeActor);
		}
	}

	SetCurrentState(EBatteryPlayState::EPlaying);

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
			SetCurrentState(EBatteryPlayState::EWin);
		}
		//Check if character power is positive
		else if(myCharacter->getCurrentPower() > 0){
			//Decrease the character's power using the decay rate
			myCharacter->UpdatePower(-DeltaTime * decayRate * (myCharacter->getInitialPower()));
		}
		else{
			SetCurrentState(EBatteryPlayState::EGameOver);
		}
	}

	//Log a debug message
	FVector pos = myCharacter->GetActorLocation();

	FString pickupDebugString = "X: " + FString::SanitizeFloat(pos.X)
		+ "\nY: " + FString::SanitizeFloat(pos.Y) + "\nZ: " + FString::SanitizeFloat(pos.Z);

	UE_LOG(LogClass, Log, TEXT("character at pos:\n %s"), *pickupDebugString);
}


void ABatteryCollectorGameMode::HandleNewState(EBatteryPlayState newState){
	switch(newState){
		//If game is playing
		case EBatteryPlayState::EPlaying:
			//spawn volumes active
			for(ASpawnVolume *volume : SpawnVolumeActors){
				volume->SetSpawningActive(true);
			}
			break;
		//If game is won
		case EBatteryPlayState::EWin:
			//spawn volumes inactive
			for(ASpawnVolume *volume : SpawnVolumeActors){
				volume->SetSpawningActive(false);
			}
			break;
		//If game is lost
		case EBatteryPlayState::EGameOver:
			//spawn volumes inactive
			for(ASpawnVolume *volume : SpawnVolumeActors){
				volume->SetSpawningActive(false);
			}

			//block player input
			APlayerController *playerController = UGameplayStatics::GetPlayerController(this, 0);
			if(playerController){
				playerController->SetCinematicMode(true, false, false, true, true);
			}

			//ragdoll the character
			ACharacter *myCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
			if(myCharacter){
				myCharacter->GetMesh()->SetSimulatePhysics(true);
				myCharacter->GetMovementComponent()->MovementState.bCanJump = false;
			}

			break;
		//Unknown/default state
		/*default:
			case EBatteryPlayState::EUnknown:
			break;*/
	}
}