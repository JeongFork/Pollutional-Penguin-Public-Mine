// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Player/Weapons/Gun.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"

//#include "Components/CapsuleComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*WalkParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WalkParticleComponent"));
	WalkParticleComponent->SetupAttachment(RootComponent);
	WalkParticleComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -250.0f));
	WalkParticleComponent->SetRelativeScale3D(FVector(0.001f));
	WalkParticleComponent->SetAutoActivate(false);*/

	WaterParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WaterParticleComponent"));
	WaterParticleComponent->SetupAttachment(RootComponent);
	WaterParticleComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	WaterParticleComponent->SetAutoActivate(false);
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	/*if (WalkParticle && WalkParticleComponent)
	{
		WalkParticleComponent->SetAsset(WalkParticle);
	}*/
	if (WaterParticle && WaterParticleComponent)
	{
		WaterParticleComponent->SetAsset(WaterParticle);
	}

	// 서버에서만 무기 장착 
	if (HasAuthority())
	{
		Gun = GetWorld()->SpawnActor<AGun>(GunClass);
		if (Gun)
		{
			Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
			Gun->SetOwner(this);
		}
	}

	PlayerController = Cast<APlayerController>(GetController());
	lastShootTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

	GameState = GetWorld()->GetGameState<APollutionGameState>();
	if(GameState){
		GameState->AlivePlayers.Add(this);
	}
}


void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->MoveUpdatedComponent(FVector(1, 1, 1), GetActorRotation(), true);
	GetCharacterMovement()->MoveUpdatedComponent(FVector(-1, -1, -1), GetActorRotation(), true);
	//UpdateLookAtCursor();

	/*if (!isDead && GetVelocity().SizeSquared() > 0.0f)
	{
		if (WalkParticleComponent && !WalkParticleComponent->IsActive())
		{
			WalkParticleComponent->Activate();
		}
	}
	else
	{
		if (WalkParticleComponent && WalkParticleComponent->IsActive())
		{
			WalkParticleComponent->Deactivate();
		}
	}*/
}

void AShooterCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GameState && GameState->AlivePlayers.Contains(this)) {
		GameState->AlivePlayers.Remove(this);
	}
	Super::EndPlay(EndPlayReason);
}

//플레이어 입력 바인딩 관련 함수 
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//플레이어 입력 바인딩(움직이기, 카메락 조작, 점프)
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);



	// //카메라 위 아래로 보는 효과인데 주석처리 할지 말지 상의 필요 
	// PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	//플레이어 방향 회전 
	//PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::Rotate);

	if (isDead) {
		return;
	}
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ShotWithSound);	//총 발사
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	//루트꺼 가져오기 
	DamageToApply = FMath::Min(Health, DamageToApply); //남은 Health값보다 데미지 수치가 높은경우 대비.. 
	Health -= DamageToApply;
	//UE_LOG(LogTemp, Warning, TEXT("남은 체력: %f"), Health);

	return DamageToApply;
}

void AShooterCharacter::ShotWithSound()
{
	if (isDead) {
		return;
	}
	if (ShootDelay > UGameplayStatics::GetRealTimeSeconds(GetWorld()) - lastShootTime) {
		return;
	}
	TRACE_BOOKMARK(TEXT("[Clinet] Input : Mouse Clicked"));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactEfeect, GetActorLocation());
	lastShootTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	Shoot();

}

void AShooterCharacter::HandleDeath()
{
	FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 30.0f);

	if (WaterParticleComponent)
	{
		WaterParticleComponent->Activate(true);
	}

	if (GameState && GameState->AlivePlayers.Contains(this)) {
		GameState->AlivePlayers.Remove(this);
		OnPlayerDied.Broadcast();
	}

	
	
}

void AShooterCharacter::HandleRevive()
{	
	if (WaterParticleComponent)
	{
		WaterParticleComponent->Deactivate();
	}

	if(GameState && !GameState->AlivePlayers.Contains(this)){
		GameState->AlivePlayers.AddUnique(this);
	}
}


void AShooterCharacter::MoveForward(float AxisValue)
{
	if (isDead) {
		return;
	}

	FVector ForwardDirection = FVector(1, 0, 0);
	AddMovementInput(ForwardDirection * AxisValue);	//AxisValue 만큼 전진 
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	if (isDead) {
		return;
	}
	FVector RightDirection = FVector(0, 1, 0);
	AddMovementInput(RightDirection * AxisValue);
}

void AShooterCharacter::Rotate(float AxisValue)
{
	//SetActorRotation(GetActorRotation() + FRotator(0,AxisValue * 1000,0));
}


//플레이어가 마우스 포인터 바라보게 하는 함수 
//void AShooterCharacter::UpdateLookAtCursor()
//{
//	if (!PlayerController) return;
//
//	FVector2D CursorPosition;
//	if (GEngine->GameViewport->GetMousePosition(CursorPosition))
//	{
//		FVector WorldPosition, WorldDirection;
//		UGameplayStatics::DeprojectScreenToWorld(PlayerController, CursorPosition, WorldPosition, WorldDirection);
//
//		FHitResult HitResult;
//		FVector End = WorldPosition + (WorldDirection * 10000);
//		if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, End, ECC_Visibility))
//		{
//			FVector LookAtDirection = (HitResult.Location - GetActorLocation()).GetSafeNormal();
//			FRotator NewRotation = FRotationMatrix::MakeFromX(LookAtDirection).Rotator();
//			FRotator CurrentRotation = GetActorRotation();
//
//			NewRotation.Pitch = CurrentRotation.Pitch;
//
//			SetActorRotation(NewRotation);
//			if (!HasAuthority()) {
//				ClientUpdateLookAtCursor(NewRotation);
//			}
//
//			CurrentRotation = NewRotation;
//		}
//	}
//
//}
//
//void AShooterCharacter::ClientUpdateLookAtCursor_Implementation(FRotator NewRotation)
//{
//	SetActorRotation(NewRotation);
//}


void AShooterCharacter::Shoot()
{
	if (isDead) return;
	if (!Gun) return;

	isShooting = true;
	
	
	if (IsLocallyControlled())
	{
		Gun->PlayLocalShootEffect();
		Gun->SpawnDummyBullet();
	}
	Server_Shoot(); // 클라이언트면 서버로 발사 요청 
}

void AShooterCharacter::Server_Shoot_Implementation()
{
	isShooting = true;
	if (Gun) {
		Gun->SpawnBullet();
		Gun->Multicast_PlayShootEffect(this);
	}
}

// void AShooterCharacter::LookUp(float AxisValue)
// {
// 	AddControllerPitchInput(AxisValue);
// }

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShooterCharacter, Gun);
	DOREPLIFETIME(AShooterCharacter, isShooting);
	DOREPLIFETIME(AShooterCharacter, isDead);
	DOREPLIFETIME(AShooterCharacter, deadPosition);
}