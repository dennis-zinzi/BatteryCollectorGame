// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
	public:	
		// Sets default values for this actor's properties
		ASpawnVolume();

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	
		// Called every frame
		virtual void Tick( float DeltaSeconds ) override;

		//Returns the whereToSpawn subobject
		FORCEINLINE class UBoxComponent* getWhereToSpawn() const{
			return whereToSpawn;
		}

		//Finds random point within BoxComponent
		UFUNCTION(BlueprintPure, Category = "Spawning")
		FVector getRandomPointInVolume();


	protected:
		//The pickup to spawn
		UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class APickup> whatToSpawn;

		FTimerHandle spawnTimer;

		//Minimum spawn delay
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float spawnDelayMin;

		//Maximum spawn delay
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float spawnDelayMax;

	private:
		//Box component to specify where pickups will be spawned
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent *whereToSpawn;

		//Handles spawning of new pickups
		void spawnPickup();

		//The current spawn delay
		float spawnDelay;
	
};
