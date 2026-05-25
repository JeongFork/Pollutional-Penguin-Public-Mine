// Fill out your copyright notice in the Description page of Project Settings.


#include "GlacierPlatformArrangement.h"
#include<utility>
#include"GlacierPlatform.h"
#include<GameFramework/PlayerState.h>
#include"Player/Controllers/InGamePlayerController.h"
#include"Player/State/MyPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Characters/ShooterCharacter.h"
#include "Enemy/Characters/Squid.h"
#include "GameFramework/Pawn.h"
#include "Engine/DamageEvents.h"
#include "Enemy/Characters/Crab.h"

// Sets default values
AGlacierPlatformArrangement::AGlacierPlatformArrangement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGlacierPlatformArrangement::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AInGamePlayerController* pcIngame;
	if (pc != nullptr) {
		pcIngame = Cast<AInGamePlayerController>(pc);
		if (pcIngame != nullptr) {
			myPS = pcIngame->GetPlayerState<AMyPlayerState>();
		}
	}
}
void AGlacierPlatformArrangement::CreateGlacierPlatformsHexagonShpae_Implementation(int countOfHexagonLength, const FVector& impactCoordinate, const TArray<double>& glacierHealth)
{
	//UE_LOG(LogTemp, Warning, TEXT("CreateGlacierPlatformsHexagonShpae_Implementation Func Called"));
	if (!HasAuthority()) return;


	const TArray<FVector2D> pointsOfTriangle = GetPointsOfTriangleWhichBelong(FVector2D(impactCoordinate));
	const FVector2D& center = GetClosestPoint(pointsOfTriangle, FVector2D(impactCoordinate));


	double nowY = center.Y + triangleLength;
	for (int yCount = 0; yCount < countOfHexagonLength; yCount++) {
		double deltaX = 0;
		for (double xCount = 0; xCount <= yCount; xCount++) {
			FTimerHandle UniqueHandle;
			FTimerDelegate timeDel;
			timeDel.BindUFunction(this, FName("CreateGlacierPlatformHexagonSymettric"), FVector2D(center.X + deltaX, nowY), center, (float)glacierHealth[yCount]);
			if (GEngine)
			{
				UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
				if (World)
				{
					World->GetTimerManager().SetTimer(UniqueHandle, timeDel, (yCount + 0.01f) * glacierRadialCreateDelay, false);
				}
			}
			//CreateGlacierPlatformHexagonSymettric(FVector2D(center.X + deltaX, nowY), center, glacierHealth[yCount]);
			if (xCount != 0) {
				FTimerHandle UniqueHandle1;
				FTimerDelegate timeDel1;
				timeDel1.BindUFunction(this, FName("CreateGlacierPlatformHexagonSymettric"), FVector2D(center.X - deltaX, nowY), center, (float)glacierHealth[yCount]);
				if (GEngine)
				{
					UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
					if (World)
					{
						World->GetTimerManager().SetTimer(UniqueHandle1, timeDel1, (yCount + 0.01f) * glacierRadialCreateDelay, false);
					}
				}
				//CreateGlacierPlatformHexagonSymettric(FVector2D(center.X - deltaX, nowY), center, glacierHealth[yCount]);
			}
			deltaX += triangleLength;
		}

		nowY += triangleLength * UNIT_Y;
	}
	//CreateGlacierPlatformsHexagonShpae_Implementation(countOfHexagonLength, impactCoordinate, glacierHealth);

	InitializeHitArrays();
}


TArray<FVector2D> AGlacierPlatformArrangement::GetPointsOfTriangleWhichBelong(const FVector2D& impactCoordinate)const
{
	const FVector2D& normalizedImpactPoint = NormalizeGridVector(FVector2D(impactCoordinate));
	const FVector2D& minimumRangePoint = GaussRangeOfMinumum(normalizedImpactPoint);
	const TArray<FVector2D>& lineCoordinates = GetLineCordinatesFromGaussRange(minimumRangePoint);
	const bool isImpactAboveLine = IsImpactPointAboveLine(lineCoordinates, normalizedImpactPoint);
	const FVector2D& anotherTrianglePoint = GetTriangleAnotherCoordinate(lineCoordinates, isImpactAboveLine);
	TArray<FVector2D> triPoints;
	triPoints.Add(DeNormalizeGridVector(lineCoordinates[0]));
	triPoints.Add(DeNormalizeGridVector(lineCoordinates[1]));
	triPoints.Add(DeNormalizeGridVector(anotherTrianglePoint));
	return triPoints;
}

FVector2D AGlacierPlatformArrangement::GetClosestPoint(const TArray<FVector2D>& points, const FVector2D& impactCoordinate)const
{
	if (points.Num() == 0) {
		//UE_LOG(LogTemp, Warning, TEXT("In GetClosestPoint Function, points's Num is 0."));
		//UE_LOG(LogTemp, Warning, TEXT("It has to be more than 0."));
		return FVector2D();
	}
	int closestPointIndex = 0;
	double closestDistance = DBL_MAX;
	for (int index = 0; index < points.Num(); index++) {
		double nowDistance = FVector2D::Distance(points[index], impactCoordinate);

		if (closestDistance > nowDistance) {
			closestPointIndex = index;
			closestDistance = nowDistance;
		}
	}
	return points[closestPointIndex];
}

void AGlacierPlatformArrangement::CreateGlacierPlatformHexagonSymettric(const FVector2D& coordinate, const FVector2D& center, float health)
{
	FVector2D coordinateInLocal = coordinate - center;
	double r = FVector2D::Distance(coordinate, center);
	double theta = atan2(coordinateInLocal.Y, coordinateInLocal.X);
	for (int i = 0; i < 6; i++) {
		double nowTheta = theta + (PI / 3) * i;
		double x = r * cos(nowTheta) + center.X;
		double y = r * sin(nowTheta) + center.Y;

		FTimerHandle UniqueHandle;
		FTimerDelegate timeDel;
		timeDel.BindUFunction(this, FName("CreateGlacierPlatformInGrid"), FVector(x, y, glacierPlatformZ), health);
		if (GEngine)
		{
			UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
			if (World)
			{
				World->GetTimerManager().SetTimer(UniqueHandle, timeDel, ((i % 2) + 0.01f) * glacierRotatingCreateDelay, false);
			}
		}
	}


}


void AGlacierPlatformArrangement::CreateGlacierPlatformInGrid(const FVector& ImpactCoordinate, float health)
{
	const FVector2D& normalizedImpactPoint = NormalizeGridVector(FVector2D(ImpactCoordinate));
	const FVector2D& minimumRangePoint = GaussRangeOfMinumum(normalizedImpactPoint);
	const TArray<FVector2D>& lineCoordinates = GetLineCordinatesFromGaussRange(minimumRangePoint);
	bool isImpactAboveLine = IsImpactPointAboveLine(lineCoordinates, normalizedImpactPoint);
	const FVector2D& anotherTrianglePoint = GetTriangleAnotherCoordinate(lineCoordinates, isImpactAboveLine);
	const FVector2D& triCenter = GetTriangleCenter(lineCoordinates[0], lineCoordinates[1], anotherTrianglePoint);
	const FVector2D& finalTriCenter = DeNormalizeGridVector(triCenter);
	const FVector& finalTriCenter3D = FVector(finalTriCenter, glacierPlatformZ);

	float lenFromZero = FVector::Dist(finalTriCenter3D, FVector::Zero());
	if (lenFromZero > createMaxRadius) {
		return;
	}


	//빙하 위에 있는 몬스터들에게 데미지 입히기 

	TArray<FHitResult> outHit;
	FCollisionQueryParams collisinParams;
	bool isHit = GetWorld()->SweepMultiByChannel(outHit, finalTriCenter3D, finalTriCenter3D, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel5, FCollisionShape::MakeSphere(NoMonsterRadius), collisinParams);
	//DrawDebugSphere(GetWorld(), finalTriCenter3D, NoMonsterRadius, 10, FColor(181, 0, 0), false, 3, 0, 2);

	if (isHit) {
		for (const FHitResult& hit : outHit) {
			AActor* actor = hit.GetActor();
			if (actor && !hitActors.Contains(actor)) {
				hitActors.Add(actor);
				if (ACrab* enemy = Cast<ACrab>(actor)) {
					ApplyDamage(actor, attackDamage, hit);
				}
				else if (ASquid* squid = Cast<ASquid>(actor)) {
					ApplyDamage(actor, attackDamage, hit);
				}
			}
			if (actor->ActorHasTag(FName("Enemy"))) {
				return;
			}

		}
	}




	AActor* existingGlacierPlatform = FindExsitingGlacierAt(triCenter);
	if (existingGlacierPlatform) {
		AGlacierPlatform* glacierPlatform = Cast<AGlacierPlatform>(existingGlacierPlatform);

		if (glacierPlatform == nullptr) {
			//UE_LOG(LogTemp, Warning, TEXT("GlacierPlatformArrangement.cpp_ Casting to GlacierPlatform is Null!"));
			return;
		}

		glacierPlatform->AdjustHealth(health);

		return;
	}

	const FRotator& triRotation = GetTriangleRotation(isImpactAboveLine);
	AActor* createdGlacierPlatform = CreateGlacierPlatform(FVector(finalTriCenter, glacierPlatformZ), triRotation, health);

	AddElementToGlacierPlatformMap(triCenter, createdGlacierPlatform);
}

void AGlacierPlatformArrangement::CreateGlacierPlatformsSquareShape(int xLength, int yLength)
{
	using std::pair;
	TArray<TArray<pair<int, int>>> candidates;

	int xNum = (xLength / triangleLength);
	int yNum = yLength / (triangleLength * UNIT_Y);

	for (double x = -xNum * triangleLength; x <= xNum * triangleLength; x += triangleLength) {
		for (double y = -yNum * (triangleLength * UNIT_Y); y < yNum * triangleLength * UNIT_Y; y += triangleLength * UNIT_Y) {
			FVector relativePostion = FVector(x, y + triangleLength, glacierPlatformZ);

			CreateGlacierPlatformInGrid(FVector(x, y + triangleLength, glacierPlatformZ), 100);
		}
	}
}



FVector2D AGlacierPlatformArrangement::GaussRangeOfMinumum(const FVector2D& impactCoordinate) const
{
	FVector2D ret;
	ret.X = FMath::Floor(impactCoordinate.X);
	ret.Y = FMath::Floor(impactCoordinate.Y);
	return ret;
}

bool isLinePoint(const FVector2D& vec2) {
	int x = (int)vec2.X;
	int y = (int)vec2.Y;
	return ((x + y) % 2) == 0;
}

TArray<FVector2D> AGlacierPlatformArrangement::GetLineCordinatesFromGaussRange(const FVector2D& minimumOfRange) const
{
	int minX = (int)minimumOfRange.X;
	int minY = (int)minimumOfRange.Y;
	TArray<FVector2D> linePointCandidates;
	linePointCandidates.Add(FVector2D(minX, minY));
	linePointCandidates.Add(FVector2D(minX, minY + 1));
	linePointCandidates.Add(FVector2D(minX + 1, minY));
	linePointCandidates.Add(FVector2D(minX + 1, minY + 1));

	linePointCandidates = linePointCandidates.FilterByPredicate(isLinePoint);

	if (linePointCandidates.Num() != 2) {
		//UE_LOG(LogTemp, Warning, TEXT("line point candidate's number is not 2"));
	}

	if (linePointCandidates[0].X > linePointCandidates[1].X) {
		//UE_LOG(LogTemp, Warning, TEXT("line point candidate is not right order, so swapped."));
		linePointCandidates.Swap(0, 1);
	}

	return linePointCandidates;
}

FVector2D AGlacierPlatformArrangement::NormalizeGridVector(const FVector2D& vec) const
{
	FVector2D normalized = (vec - centerPoint) / triangleLength;
	normalized.Y /= UNIT_Y;
	return normalized;
}

FVector2D AGlacierPlatformArrangement::DeNormalizeGridVector(const FVector2D& normalizedVec) const
{
	FVector2D deNormalized = normalizedVec * triangleLength + centerPoint;
	deNormalized.Y *= UNIT_Y;
	return deNormalized;
}


FVector2D AGlacierPlatformArrangement::GetTriangleCenter(const FVector2D& triangleCoordinate1, const FVector2D& triangleCoordinate2, const FVector2D& triangleCoordinate3) const
{
	return (triangleCoordinate1 + triangleCoordinate2 + triangleCoordinate3) / 3;
}

bool AGlacierPlatformArrangement::IsImpactPointAboveLine(const TArray<FVector2D>& lineCoordinates, const FVector2D& impactCoordinate) const
{
	if (lineCoordinates.Num() != 2) {
		//(LogTemp, Warning, TEXT("lineCordinate's Num is not 2!"));
	}
	const FVector2D& left = lineCoordinates[0];
	const FVector2D& right = lineCoordinates[1];

	if (left.Y > right.Y) {
		return impactCoordinate.Y > -impactCoordinate.X + left.X + left.Y;
	}
	else {
		return impactCoordinate.Y > impactCoordinate.X + left.Y - left.X;
	}
}

FVector2D AGlacierPlatformArrangement::GetTriangleAnotherCoordinate(const TArray<FVector2D>& lineCoordinates, bool isImpactPointAboveLine) const
{
	if (lineCoordinates.Num() != 2) {
		//UE_LOG(LogTemp, Warning, TEXT("lineCordinate's Num is not 2!"));
	}

	const FVector2D& left = lineCoordinates[0];
	const FVector2D& right = lineCoordinates[1];
	double slope = (right.Y - left.Y) / (right.X - left.X);
	bool isSlopePositive = slope > 0;

	FVector2D anotherCoordinate;
	if (isImpactPointAboveLine && isSlopePositive) {
		anotherCoordinate = FVector2D(right.X - 2, right.Y);
	}
	else if (!isImpactPointAboveLine && isSlopePositive) {
		anotherCoordinate = FVector2D(left.X + 2, left.Y);
	}
	else if (isImpactPointAboveLine && !isSlopePositive) {
		anotherCoordinate = FVector2D(left.X + 2, left.Y);
	}
	else {
		anotherCoordinate = FVector2D(right.X - 2, right.Y);
	}
	return anotherCoordinate;
}

FRotator AGlacierPlatformArrangement::GetTriangleRotation(bool isImpacePointAboveLine) const
{
	if (isImpacePointAboveLine) {
		return FRotator(0.0f, 150.0f, 0.0f);
	}
	else {
		return FRotator(0.0f, 90.0f, 0.0f);
	}
}

AActor* AGlacierPlatformArrangement::CreateGlacierPlatform(const FVector& coordinate, const FRotator& rotation, float deltaHealth)
{
	AActor* actor = GetWorld()->SpawnActor(BP_GlacierPlatform);
	//actor->SetFolderPath("GlacierPlatforms");


	if (!actor) {
		//UE_LOG(LogTemp, Warning, TEXT("GlacierPlatformArrangement.cpp_ actor which is spawned by SpawnActor function is null!"));
		return nullptr;
	}
	AGlacierPlatform* glacierPlatform = Cast<AGlacierPlatform>(actor);
	if (glacierPlatform == nullptr) {
		//UE_LOG(LogTemp, Warning, TEXT("GlacierPlatformArrangement.cpp_ Casting to GlacierPlatform is Null!"));
		return nullptr;
	}
	glacierPlatform->AdjustHealth(deltaHealth);
	actor->SetActorLocation(coordinate - FVector(0.0, 0.0, glacierPlatformDownStartZ));
	actor->SetActorRotation(rotation);
	return actor;
}

AActor* AGlacierPlatformArrangement::FindExsitingGlacierAt(const FVector2D& triangleCenter) const
{
	int x = (int)(triangleCenter.X);
	int y = (int)roundl(triangleCenter.Y * 3);
	const FVector2D& transformedCenter = FVector2D(x, y);
	AActor* const* existingGlacierPlatform = nowGlacierPlatforms.Find(transformedCenter);
	if (existingGlacierPlatform) {
		return *existingGlacierPlatform;
	}
	else {
		return nullptr;
	}
}

bool AGlacierPlatformArrangement::AddElementToGlacierPlatformMap(FVector2D center, AActor* createdGlacier)
{
	if (myPS != nullptr) {
		myPS->FreezeCount++;
	}

	center.Y = (int)roundl(center.Y * 3);
	AActor* ret = nowGlacierPlatforms.Add(center, createdGlacier);
	return ret != nullptr;
}

//빙하위에 있는 몬스터한테 데미지입히기 
void AGlacierPlatformArrangement::ApplyDamage(AActor* hitActor, float DamageAmount, const FHitResult& hitResult)
{
	FVector ShotDirection = hitActor->GetActorRotation().Vector();
	AController* eventInstigator = nullptr;

	if (ACharacter* character = Cast<ACharacter>(hitActor)) {
		eventInstigator = character->GetController();
	}

	FPointDamageEvent DamageEvent(DamageAmount, hitResult, ShotDirection, nullptr);

	hitActor->TakeDamage(DamageAmount, DamageEvent, eventInstigator, this);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		DamageFlash,
		GetActorLocation(),
		GetActorRotation(),
		FVector(5.0f),
		true
	);
}

void AGlacierPlatformArrangement::InitializeHitArrays()
{
	hitActors.Empty();
}


