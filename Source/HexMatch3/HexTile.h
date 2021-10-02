// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "HexGrid.h"
#include "HexTile.generated.h"



UCLASS()
class HEXMATCH3_API AHexTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* TileMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor TileColor;

	class AHexGrid* OwnerGrid;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
