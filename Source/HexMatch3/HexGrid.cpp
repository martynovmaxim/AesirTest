// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGrid.h"
#include "HexPlayerController.h"

// Sets default values
AHexGrid::AHexGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	GenerateGrid();

}

// Called when the game starts or when spawned
void AHexGrid::BeginPlay()
{
	Super::BeginPlay();
	Cast<AHexPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Grid = this;

	GenerateGrid();
	
}

// Called every frame
void AHexGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHexGrid::GenerateGrid()
{	
	for (int i = 0; i < GridWidth; i++)
	{
		for (int j = GridHeight - 1; j >= 0 ; j--)
		{
			SpawnTile(i, j);
		}
	}
}

void AHexGrid::StartFalling(TMap <int, TArray<int>> RowsToDestroy)
{
	//find Highest and lowest in each Row
	for (auto& elem :RowsToDestroy)
	{
		int row = elem.Key;
		TArray<int> line = elem.Value;
		TArray <AHexTile*> CopyTiles = Tiles;
		for (auto& tile : CopyTiles)
		{
			int x = tile->xCoord;
			int y = tile->yCoord;
			if (x == row)
			{
				//calculate movement
				int movement = 0;
				for (int id : line)
				{
					if (y > id) movement++;
				}
				FallTile(tile, movement);
			}
		}
		//SpawnTiles
		for (int i = GridHeight - line.Num(); i < GridHeight; i++)
		{
			SpawnTile(row, i);
		}
	}
}

void AHexGrid::FallTile(AHexTile* tile, int movement)
{
	//Change coordinates
	tile->yCoord -= movement;
		
	//Updating Background
	tile->Background = Backgrounds[tile->xCoord * 6 + GridHeight - tile->yCoord - 1];

	//Updating Location
	float y = yOffset * movement;
	FVector newLoc = FVector(0, y, 0);
	tile->AddActorWorldOffset(newLoc);
}

AHexTile* AHexGrid::SpawnTile(int x, int y)
{
	//preparing for spawn
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UWorld* const World = GetWorld();
	FRotator rot = FRotator::ZeroRotator;
	FVector loc = GetActorLocation() + FVector(xOffset * x, yOffset * (GridHeight - y - 1) + yOffset / 2 * (x % 2), 0);

	//new tile init
	AHexTile* const NewTile = World->SpawnActor<AHexTile>(TileBaseClass, loc, rot, SpawnParams);
	NewTile->OwnerGrid = this;
	FColor RandomColor = TileInfos[FMath::RandRange(0, TileInfos.Num() - 1)];
	NewTile->SetColor(RandomColor);
	NewTile->xCoord = x;
	NewTile->yCoord = y;
	Tiles.Add(NewTile);

	//Setting Background for the new tile
	if (Backgrounds.Num() < GridHeight * GridWidth)
	{
		ABackground* back = GetWorld()->SpawnActor<ABackground>(BackgroundClass, loc - FVector(0, 0, 1), rot, SpawnParams);
		NewTile->Background = back;
		Backgrounds.Add(back);
	}
	else NewTile->Background = Backgrounds[x * 6 + GridHeight - y - 1];

	return NewTile;
}