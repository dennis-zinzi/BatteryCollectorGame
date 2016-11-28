// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryCollector.h"
#include "SpawnVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pickup.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create Box component to represent spawn volume
	whereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = whereToSpawn;

	//Set spawn delay range
	spawnDelayMin = 1.0f;
	spawnDelayMax = 4.5f;

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASpawnVolume::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

FVector ASpawnVolume::getRandomPointInVolume(){
	FVector spawnOrigin = whereToSpawn->Bounds.Origin,
		spawnExtent = whereToSpawn->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(spawnOrigin, spawnExtent);
}

void ASpawnVolume::spawnPickup(){
	//If we have set something to spawn
	if(whatToSpawn != NULL){
		//Check for a valid World
		UWorld* const WORLD = GetWorld();

		if(WORLD){
			//Set the spawn params
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = Instigator;

			//Get random location to spawn at
			FVector spawnLocation = getRandomPointInVolume();

			//Get a random rotation for the spawned item
			FRotator spawnRotation;
			spawnRotation.Yaw = FMath::FRand() * 360.0f;
			spawnRotation.Pitch = FMath::FRand() * 360.0f;
			spawnRotation.Roll = FMath::FRand() * 360.0f;

			//Spawn the pickup
			APickup* const SPAWNED_PICKUP = WORLD->SpawnActor<APickup>(whatToSpawn, spawnLocation, spawnRotation, spawnParams);

			spawnDelay = FMath::FRandRange(spawnDelayMin, spawnDelayMax);
			GetWorldTimerManager().SetTimer(spawnTimer, this, &ASpawnVolume::spawnPickup, spawnDelay, false);
		}
	}
}

void ASpawnVolume::SetSpawningActive(bool bShouldSpawn){
	if(bShouldSpawn){
		//Set timer on spawn pickup
		spawnDelay = FMath::FRandRange(spawnDelayMin, spawnDelayMax);
		GetWorldTimerManager().SetTimer(spawnTimer, this, &ASpawnVolume::spawnPickup, spawnDelay, false);
	}
	else{
		//clear timer on spawn pickup
		GetWorldTimerManager().ClearTimer(spawnTimer);
	}
}