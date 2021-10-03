// Fill out your copyright notice in the Description page of Project Settings.


#include "HexPlayerController.h"

AHexPlayerController::AHexPlayerController()
{
	bEnableClickEvents = true;
	bShowMouseCursor = true;
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

}

void AHexPlayerController::Tick(float DeltaTime)
{
	if (HasClicked)
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit)
		{
			//Check if this is the same color as we already hit and the same
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
					//went back to previous
					else
					{
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
		for (auto& elem : Line)
		{
			int x = elem->xCoord;
			int y = elem->yCoord;
			if (!RowsToDestroy.Contains(x)) RowsToDestroy.Add(x);
			RowsToDestroy[x].Add(y);
			//Clean
			Grid->Tiles.Remove(elem);
			elem->SelectTile(false);
			elem->Destroy();
		}
		Grid->StartFalling(RowsToDestroy);
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