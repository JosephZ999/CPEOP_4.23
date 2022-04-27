// Fill out your copyright notice in the Description page of Project Settings.

#include "MyFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
// #include "Engine/EngineTypes.h"

#define CHANNEL_WALL ECC_GameTraceChannel3

int UMyFunctionLibrary::RotationDivide(FVector2D pointA, FVector2D pointB)
{
	float Rotation = UKismetMathLibrary::FindLookAtRotation(FVector(pointA.X, pointA.Y, 0.f), FVector(pointB.X, pointB.Y, 0.f)).Yaw + 225.f;

	float remainder;
	int	  outValue = UKismetMathLibrary::FMod(Rotation, 90.f, remainder);
	return (outValue == 0) ? 4 : outValue;
}

int UMyFunctionLibrary::RotationDivide2(FVector2D pointA, FVector2D pointB, uint8 divisionNum, float& angle)
{
	if (divisionNum != 0)
	{
		angle		   = 360.f / divisionNum;
		float Rotation = UKismetMathLibrary::FindLookAtRotation(FVector(pointA.X, pointA.Y, 0.f), FVector(pointB.X, pointB.Y, 0.f)).Yaw +
						 180.f + angle / 2.f;
		float remainder;
		int	  outValue = UKismetMathLibrary::FMod(Rotation, angle, remainder);

		if (outValue == 0)
		{
			outValue = divisionNum;
		}
		angle *= (float)outValue;
		return outValue;
	}
	else
	{
		angle = UKismetMathLibrary::FindLookAtRotation(FVector(pointB.X, pointB.Y, 0.f), FVector(pointA.X, pointA.Y, 0.f)).Yaw;
		return 0;
	}
}

float UMyFunctionLibrary::PercentDivision(float percent, uint8 divisionNum)
{
	if (divisionNum != 0)
	{
		float div = 1.f / divisionNum;
		float remainder;
		float outValue = UKismetMathLibrary::FMod(percent + div / 2.f, div, remainder);
		outValue	   = outValue / divisionNum;
		return outValue;
	}
	return percent;
}

FString UMyFunctionLibrary::FindObjectName(FString objectPath)
{
	FString ObjectName;
	int32	charIndex = objectPath.Len();
	while (true)
	{
		charIndex--;
		if (objectPath[charIndex] == '/')
		{
			for (int i = ++charIndex; i < objectPath.Len(); i++)
			{
				ObjectName.AppendChar(objectPath[i]);
			}
			break;
		}
		else if (charIndex == 0)
		{
			for (int i = ++charIndex; i < objectPath.Len(); i++)
			{
				ObjectName.AppendChar(objectPath[i]);
			}
			break;
		}
	}
	return ObjectName;
}

FVector UMyFunctionLibrary::FindRandomLocation(AActor* ActorInCenter, float DistMin, float DistMax)
{
	if (! ActorInCenter)
		return FVector::ZeroVector;

	UWorld*		  nWorld	 = ActorInCenter->GetWorld();
	const FVector TraceStart = ActorInCenter->GetActorLocation();
	const FVector RandVector = FVector(FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f).Vector());
	const FVector TraceEnd	 = TraceStart + RandVector * FMath::FRandRange(DistMin, DistMax);

	FCollisionQueryParams QueryParams(TEXT("Wall"), false, ActorInCenter);
	FHitResult			  Hit;
	if (nWorld->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, CHANNEL_WALL, QueryParams, ECollisionResponse::ECR_Block))
	{
		return Hit.ImpactPoint;
	}
	return TraceEnd;
}
