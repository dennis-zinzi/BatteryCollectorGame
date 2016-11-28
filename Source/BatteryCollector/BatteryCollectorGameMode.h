// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "BatteryCollectorGameMode.generated.h"


//enum to store current gameplay state
UENUM(BlueprintType)
enum class EBatteryPlayState{
	EPlaying,
	EGameOver,
	EWin,
	EUnknown
};

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameMode
{
	GENERATED_BODY()

	public:
		ABatteryCollectorGameMode();

		virtual void Tick(float DeltaTime) override;

		UFUNCTION(BlueprintPure, Category = "Power")
		inline float GetPowerToWin() const{
			return PowerToWin;
		}


		//Current playing state getter
		UFUNCTION(BlueprintPure, Category = "Power")
		inline EBatteryPlayState GetCurrentState() const{
			return CurrentState;
		}

		//Current playing state setter
		//UFUNCTION(BlueprintPure, Category = "Power")
		inline void SetCurrentState(EBatteryPlayState state){
			CurrentState = state;
		}
		

		virtual void BeginPlay() override;

	protected:
		//The rate at which the character loses power
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float decayRate;

		//The power needed to win the game
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float PowerToWin;

		//The Widget class to use for the HUD screen
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> HUDWidgetClass;

		//The instance of the HUD
		UPROPERTY()
		class UUserWidget *CurrentWidget;

	private:
		//Keeps track of current play state
		EBatteryPlayState CurrentState;
};



