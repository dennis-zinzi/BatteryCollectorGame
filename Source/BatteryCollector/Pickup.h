// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API APickup : public AActor
{
	GENERATED_BODY()
	
	public:	
		// Sets default values for this actor's properties
		APickup();

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	
		// Called every frame
		virtual void Tick( float DeltaSeconds ) override;

		//Return pickup's mesh
		FORCEINLINE class UStaticMeshComponent* getMesh() const{
			return pickupMesh;
		}

		//Return pickup state
		UFUNCTION(BlueprintPure, Category = "Pickup")
		bool IsActive();

		//Allows other classes to change pickup state
		UFUNCTION(BlueprintCallable, Category = "Pickup")
		void setActive(bool pickupState);

		//Called when the pickup is collected
		UFUNCTION(BlueprintNativeEvent)
		void WasCollected();
		virtual void WasCollected_Implementation();

	protected:
		//True when pickup usable, false when it can't be
		bool isActive;

	private:
		//Static mesh to represent the pickup in the level
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent *pickupMesh;
	
	
};
