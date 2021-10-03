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
	float x = 0;
	float y = 0;
	
	for (int i = 0; i < GridWidth; i++)
	{
		for (int j = 0; j < GridHeight; j++)
		{
			y = yOffset * j + yOffset / 2 * (i % 2);
			x = xOffset * i;
			FVector SpawnLocation = FVector(x, y, 0) + GetActorLocation();

			//SettingColor
			FColor RandomColor = TileInfos[FMath::RandRange(0, TileInfos.Num() - 1)];
			GenerateTile(SpawnLocation, RandomColor, i, j);
		}
	}
}

AHexTile* AHexGrid::GenerateTile(FVector loc, FColor TileColor, int x, int y)
{
	//REDO

	y = GridHeight - y - 1;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UWorld* const World = GetWorld();
	FRotator rot = FRotator::ZeroRotator;

	AHexTile* const NewTile = World->SpawnActor<AHexTile>(TileBaseClass, loc, rot, SpawnParams);
	NewTile->OwnerGrid = this;
	NewTile->SetColor(TileColor);
	NewTile->xCoord = x;
	NewTile->yCoord = y;
	Tiles.Add(NewTile);

	//spawningBackground
	ABackground* back = GetWorld()->SpawnActor<ABackground>(BackgroundClass, loc - FVector(0, 0, 1), rot, SpawnParams);
	NewTile->Background = back;
	Backgrounds.Add(back);
	return NewTile;
}

void AHexGrid::StartFalling(TMap <int, TArray<int>> RowsToDestroy)
{
	//find Highest and lowest in each Row
	for (auto& elem :RowsToDestroy)
	{
		int row = elem.Key;
		TArray<int> line = elem.Value;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fall is %d"), RowsToDestroy.Num() * line.Num()));
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
				
				//SpawnNewTile
			}
		}
		for (int i = GridHeight - line.Num(); i < GridHeight; i++)
		{
			SpawnTile(row, i);
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fall is %i"), Coords.Num()));
}

void AHexGrid::FallTile(AHexTile* tile, int movement)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fall Line %i from %i howmuch %i"), ));
	//change coordinates
	tile->yCoord -= movement;
		
	//Updating Background
	tile->Background = Backgrounds[tile->xCoord * 6 + GridHeight - tile->yCoord - 1];

	//Updating Location
	float y = yOffset * movement;
	FVector newLoc = FVector(0, y, 0);
	tile->AddActorWorldOffset(newLoc);
}

void AHexGrid::SpawnTile(int x, int y)
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
	UE_LOG(LogTemp, Warning, TEXT("Spawned %s, row %d, id %d"), *NewTile->GetName(), x, y);
	NewTile->OwnerGrid = this;
	FColor RandomColor = TileInfos[FMath::RandRange(0, TileInfos.Num() - 1)];
	NewTile->SetColor(RandomColor);
	NewTile->xCoord = x;
	NewTile->yCoord = y;
	Tiles.Add(NewTile);

	//Setting Background for the new tile
	NewTile->Background = Backgrounds[x * 6 + GridHeight - y - 1];
}