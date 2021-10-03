// Fill out your copyright notice in the Description page of Project Settings.


#include "Background.h"

// Sets default values
ABackground::ABackground()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hexagon"));

	//Setting the Material to Tile


	RootComponent = StaticMesh;
	RootComponent->SetMobility(EComponentMobility::Movable);
}

// Called when the game starts or when spawned
void ABackground::BeginPlay()
{
	Super::BeginPlay();
	MI_Background = UMaterialInstanceDynamic::Create(StaticMesh->GetMaterial(0), this);
	StaticMesh->SetMaterial(0, MI_Background);
}

// Called every frame
void ABackground::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABackground::SetColor(FColor NewColor)
{
	MI_Background->SetVectorParameterValue(FName(TEXT("Color")), NewColor);
}

void ABackground::Activate(bool activate)
{
	FColor NewColor = activate ? FColor::Yellow : FColor::Silver;
	SetColor(NewColor);
}
