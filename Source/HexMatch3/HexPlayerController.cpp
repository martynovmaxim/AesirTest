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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("play"));
	
}

void AHexPlayerController::Tick(float DeltaTime)
{
	if (HasClicked)
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit)
		{
			//check if this is the same color as we already hit and the same
			AHexTile* LastHitTile = Cast<AHexTile>(Hit.Actor);
			if (LastHitTile && LastHitTile != CurrentTile)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Tile"));
				if (!CurrentTile || CurrentTile->TileColor == LastHitTile->TileColor)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NEW TILE"));
					int id = Line.Find(LastHitTile);
					if (id == -1)
					{
						GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, FString::Printf(TEXT("id is %i"), id));
						//need to be added to a row
						Line.Add(LastHitTile);
						
					}
					else
					{
						//went back to previous
						id = Line.Find(CurrentTile);
						Line.RemoveAt(id);
					}
					CurrentTile = LastHitTile;
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Len is %i"), Line.Num()));
				}

			}
		}
	}
	
}

void AHexPlayerController::Clicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("click"));
	//PrimaryActorTick.bCanEverTick = true;
	HasClicked = true;
	/*FHitResult Hit;
	GetHitResultUnderCursor*/
}

void AHexPlayerController::Released()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Release"));
	//PrimaryActorTick.bCanEverTick = false;
	if (Line.Num() >= 3)
	{
		for (int i = 0; i < Line.Num(); i++)
		{
			Line[i]->Destroy();
		}
	}
	Line.Empty();
	CurrentTile = nullptr;
	HasClicked = false;
}