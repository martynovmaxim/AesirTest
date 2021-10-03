// Fill out your copyright notice in the Description page of Project Settings.


#include "HexPlayerController.h"

AHexPlayerController::AHexPlayerController()
{
	bEnableClickEvents = true;
	bShowMouseCursor = true;
	//PrimaryActorTick.bCanEverTick = false;
}

void AHexPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("Click", IE_Pressed, this, &AHexPlayerController::Clicked);
	InputComponent->BindAction("Click", IE_Released, this, &AHexPlayerController::Released);
}

void AHexPlayerController::BeginPlay()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("play"));
	
}

void AHexPlayerController::Tick(float DeltaTime)
{
	if (HasClicked)
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Hit %s"), *Hit.Actor->GetName()));
			//check if this is the same color as we already hit and the same
			AHexTile* LastHitTile = Cast<AHexTile>(Hit.Actor);
			if (LastHitTile && LastHitTile != CurrentTile)
			{
				if (!CurrentTile || CurrentTile->TileColor == LastHitTile->TileColor)
				{
					int id = Line.Find(LastHitTile);
					if (id == -1)
					{
						LastHitTile->SelectTile(true);
						//need to be added to a row
						Line.Add(LastHitTile);
					}
					else
					{
						//went back to previous
						CurrentTile->SelectTile(false);
						id = Line.Find(CurrentTile);
						Line.RemoveAt(id);
					}
					CurrentTile = LastHitTile;
				}
			}
		}
	}
	
}

void AHexPlayerController::Clicked()
{
	HasClicked = true;
}

void AHexPlayerController::Released()
{
	if (Line.Num() >= 3)
	{
		TMap <int, TArray<int>> RowsToDestroy;
		for (int i = 0; i < Line.Num(); i++)
		{
			auto elem = Line[i];

			if (!RowsToDestroy.Contains(elem->xCoord)) RowsToDestroy.Add(elem->xCoord);
			RowsToDestroy[elem->xCoord].Add(elem->yCoord);
			//Clean
			Grid->Tiles.Remove(elem);
			elem->SelectTile(false);
			elem->Destroy();
		}
		Grid->StartFalling(RowsToDestroy);
		// Tell to Grid to start Gravity and spawning
	}
	else
	{
		for (auto& tile : Line)
		{
			tile->SelectTile(false);
		}
	}
	Line.Empty();
	CurrentTile = nullptr;
	HasClicked = false;
}