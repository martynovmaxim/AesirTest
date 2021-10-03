// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Math/UnrealMathUtility.h>
#include "HexTile.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "HexGrid.generated.h"

class AHexPlayerController;

UCLASS()
class HEXMATCH3_API AHexGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <AHexTile*> Tiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <FColor> TileInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AHexTile> TileBaseClass;

	//BackGround
	UPROPERTY(EditAnywhere, BlueprintReadWRite)
	UStaticMesh* BackgroundMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWRite)
	UMaterial* BackgroundMeshMaterial;
	TArray <AStaticMeshActor*> Backgrounds;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GridHeight;
	float yOffset = 18;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GridWidth;
	float xOffset = 15.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GapWidth;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateGrid();

	AHexTile* GenerateTile(FVector loc, FColor TileColor, int _xCoord, int _yCoord);

	void StartGravity();

	void StartFalling(TMap <int, TArray<int>> RowsToDestroy);

	void FallTile(AHexTile* tile, int movement);

	void SpawnTile(int x, int y);

};
