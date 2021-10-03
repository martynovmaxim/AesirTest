// Fill out your copyright notice in the Description page of Project Settings.


#include "HexTile.h"

// Sets default values
AHexTile::AHexTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hexagon"));

	//Setting the Material to Tile
	

	RootComponent = StaticMesh;
	RootComponent->SetMobility(EComponentMobility::Movable);
}


// Called when the game starts or when spawned
void AHexTile::BeginPlay()
{
	Super::BeginPlay();
	MI_Color = UMaterialInstanceDynamic::Create(TileMaterial, this);
	StaticMesh->SetMaterial(0, MI_Color);
}

// Called every frame
void AHexTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHexTile::SetColor(FColor NewColor)
{
	TileColor = NewColor;
	MI_Color->SetVectorParameterValue(FName(TEXT("Color")), TileColor);
}

