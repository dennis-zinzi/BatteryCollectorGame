// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryCollector.h"
#include "Pickup.h"


// Sets default values
APickup::APickup(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Every pickup starts active
	isActive = true;

	//Create the static mesh component
	pickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = pickupMesh;
}

// Called when the game starts or when spawned
void APickup::BeginPlay(){
	Super::BeginPlay();
	
}

// Called every frame (NOT IN THIS CASE)
void APickup::Tick( float DeltaTime ){
	Super::Tick( DeltaTime );

}

//Returns active state
bool APickup::IsActive(){
	return isActive;
}


//Changes pickup state
void APickup::setActive(bool pickupState){
	isActive = pickupState;
}


void APickup::WasCollected_Implementation(){
	//Log a debug message
	FString pickupDebugString = GetName();
	UE_LOG(LogClass, Log, TEXT("You have collected: %s"), *pickupDebugString);
}
