// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HexTile.h"
#include "HexGrid.h"
#include "Containers/Map.h"
#include "HexPlayerController.generated.h"

class AHexGrid;

UCLASS()
class HEXMATCH3_API AHexPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHexPlayerController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	//EventForMouseClick
	void Clicked();

	//EventForMouseClick
	void Released();
	
	AHexGrid* Grid;
	
private:
	AHexTile* CurrentTile;
	TArray<AHexTile*> Line;
	bool HasClicked = false;
	
};
