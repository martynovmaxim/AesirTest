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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Screen Message"));
	//UE_LOG(Text(LogTemp, Warning, "Hi there!"));
	float z = this->GetActorLocation().Z;
	float x = 0;
	float y = 0;
	
	for (int i = 0; i < GridWidth; i++)
	{
		for (int j = 0; j < GridHeight; j++)
		{
			y = yOffset * j + yOffset / 2 * (i % 2);
			x = xOffset * i;
			FVector SpawnLocation = FVector(x, y, z) + GetActorLocation();
			
			FColor RandomColor = TileInfos[FMath::RandRange(0, TileInfos.Num() - 1)];
			
			GenerateTile(SpawnLocation, RandomColor, i, j);
		}
	}
}

AHexTile* AHexGrid::GenerateTile(FVector loc, FColor TileColor, int x, int y)
{
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
	TilesAdresses.Add(TTuple<int, int>(x, y), NewTile);

	//spawningBackground
	AStaticMeshActor* back = GetWorld()->SpawnActor<AStaticMeshActor>(loc - FVector(0, 0, 1), rot, SpawnParams);
	back->GetStaticMeshComponent()->SetStaticMesh(Background);
	back->SetMobility(EComponentMobility::Movable);
	return NewTile;
}

void AHexGrid::StartFalling(TMap <int, int> Highest, TMap <int, int> Lowest)
{
	//find Highest and lowest in each Row
	for (auto& elem : Lowest)
	{
		//clamping from 1 to inf
		int32 max = Highest[elem.Key];
		int count = max - elem.Value + 1;
		FallLine(elem.Key, elem.Value, count);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fall is %i"), Coords.Num()));
}

void AHexGrid::FallLine(int Row, int From, int HowMuch)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fall Line %i from %i howmuch %i"), Row, From, HowMuch));
	TArray<AHexTile*> tilesToMove;
	for (auto& coords : TilesAdresses)
	{
		int TileX = coords.Key.Key;
		int TileY = coords.Key.Value;
		if (TileX == Row)
		{
			if (TileY >= From + HowMuch)
			{
				tilesToMove.Add(coords.Value);
			}
		}
	}
	for (auto& tile : tilesToMove)
	{
		//change coordinates
		TPair<int, int> oldAdress = TPair<int, int>(tile->xCoord, tile->yCoord);
		tile->yCoord -= HowMuch;
		//update adress list
		TilesAdresses.FindAndRemoveChecked(oldAdress);
		const TPair<int, int> newCoordinates = TPair<int, int>(Row, tile->yCoord);
		TilesAdresses.Add(newCoordinates, tile);

		//Updating Location
		FVector oldLoc = tile->GetActorLocation();
		float y = yOffset * HowMuch;
		FVector newLoc = FVector(0, y, 0);
		tile->AddActorWorldOffset(newLoc);
	}
}