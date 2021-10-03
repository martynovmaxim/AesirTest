// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Math/UnrealMathUtility.h>
#include "HexTile.h"
#include "HexGrid.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GridHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GridWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GapWidth;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateGrid();

	AHexTile* GenerateTile(FVector loc, FColor TileColor);

};
