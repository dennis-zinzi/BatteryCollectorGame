// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYCOLLECTOR_API ABatteryPickup : public APickup {
	GENERATED_BODY()
	
	public:
		// Sets default values for this actor's properties
		ABatteryPickup();
	
		//Override the WasCollected function - use Implementation because it's a Blueprint Native Event
		void WasCollected_Implementation() override;

		//Battery Power getter
		inline float GetBatteryPower(){
			return batteryPower;
		}

	protected:
		//Sets the amount of power the battery gives to the character
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float batteryPower;
	
};
