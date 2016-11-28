// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryCollector.h"
#include "BatteryPickup.h"

//Set default constructor
ABatteryPickup::ABatteryPickup(){
	getMesh()->SetSimulatePhysics(true);

	//set base battery power
	batteryPower = 150.0f;
}


void ABatteryPickup::WasCollected_Implementation(){
	//Use the base pickup behavior
	Super::WasCollected_Implementation();

	//Destroy the battery
	Destroy();
}