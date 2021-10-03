// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGrid.h"

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
	float xOffset = 14.75;
	float y = 0;
	float yOffset = 17.3;
	for (int i = 0; i < GridWidth; i++)
	{
		
		for (int j = 0; j < GridHeight; j++)
		{
			//SomeSpawningMagic; boundaries of the static mesh with 0.1 scale is (20, 17.3);
			/*FVector Boundaries = FVector(20, 17.3, 0);
			x = Boundaries.X * i + (0.45/2) * (j % 2);
			y = Boundaries.Y * j;*/
			y = yOffset * j + yOffset / 2 * (i % 2);
			x = xOffset * i;
			FVector SpawnLocation = FVector(x, y, z) + GetActorLocation();
			
			FColor RandomColor = TileInfos[FMath::RandRange(0, TileInfos.Num() - 1)];
			
			GenerateTile(SpawnLocation, RandomColor);
		}
	}
}

AHexTile* AHexGrid::GenerateTile(FVector loc, FColor TileColor)
{
	//ignoring for now randomising the Color
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UWorld* const World = GetWorld();
	FRotator rot = FRotator::ZeroRotator;
	AHexTile* const NewTile = World->SpawnActor<AHexTile>(TileBaseClass, loc, rot, SpawnParams);
	NewTile->OwnerGrid = this;
	NewTile->SetColor(TileColor);
	Tiles.Add(NewTile);
	return NewTile;
}