// Ilya Ilyin 2021


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	OpeningActor = GetWorld()->GetFirstPlayerController()->GetPawn();

	
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	OpenAngle += InitialYaw;
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("The %s has the OpenDoor component on it, but no PressurePlate set."), *GetOwner()->GetName());
	}
	
	// FRotator CurrentRotation = GetOwner()->GetActorRotation();
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("The yaw is %f"), CurrentRotation.Yaw);
	// UE_LOG(LogTemp, Warning, TEXT("The roll is %f"), CurrentRotation.Roll);
	// CurrentRotation.Yaw = 90.f;
	// GetOwner()->SetActorRotation(CurrentRotation);

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (PressurePlate && PressurePlate->IsOverlappingActor(OpeningActor))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() > DoorLastOpened + DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}


	// CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	// FRotator OpenDoor(0.f, OpenAngle, 0.f);
	// OpenDoor.Yaw = FMath::Lerp(CurrentYaw, OpenAngle, 0.05f);
	// // OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, 2);
	// // OpenDoor.Yaw = FMath::FInterpConstantTo(CurrentYaw, OpenAngle, DeltaTime, 45);
	// GetOwner()->SetActorRotation(OpenDoor);
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

