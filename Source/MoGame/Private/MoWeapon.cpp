// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOWeapon.h"

AMOWeapon::AMOWeapon(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	WeaponMeshComponent = ObjectInitializer.CreateOptionalDefaultSubobject<UStaticMeshComponent>(this, TEXT("WeaponRMeshComp"));
	if (WeaponMeshComponent != nullptr)
	{
		WeaponMeshComponent->SetupAttachment(RootComponent);
		WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMeshComponent->SetCastShadow(false);
	}

	HandsAttachSocketName = TEXT("Weapon_R");

	Damage = 10.f;
}

void AMOWeapon::GivenTo(AMOCharacter* NewOwner, bool bAutoActivate)
{
	Super::GivenTo(NewOwner, bAutoActivate);
	AttachToOwner();
}

void AMOWeapon::AttachToOwner()
{
	if (MOOwner == NULL)
	{
		return;
	}

	if (WeaponMeshComponent != nullptr)
	{
		WeaponMeshComponent->AttachToComponent(MOOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, HandsAttachSocketName);
	}
	Super::RegisterAllComponents();
	RegisterAllActorTickFunctions(true, true);

	FWeaponInfo WeaponInfo;
	WeaponInfo.MOWeapon = this;
	WeaponInfo.SocketName = HandsAttachSocketName;

	MOOwner->MOWeaponList.Add(WeaponInfo);
}

void AMOWeapon::DetachFromOwner()
{
	bIsDrawWeaponCollision = false;
	// unregister components so they go away
	UnregisterAllComponents();
}

void AMOWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITOR
	if (MeleeCollision != nullptr && bIsDrawWeaponCollision)
	{
		DrawDebugCapsule(GetWorld(), MeleeCollision->GetComponentLocation(), MeleeCollision->GetScaledCapsuleHalfHeight(), MeleeCollision->GetScaledCapsuleRadius(), MeleeCollision->GetComponentQuat(), FColor::Green);
	}
#endif	// WITH_EDITOR
}

void AMOWeapon::FireShot()
{
	MOOwner->bIsAttacking = true;
	if (ProjectileClass != nullptr)
	{
		FireProjectile(ProjectileClass)->Damage = Damage;
	}
}

AMOProjectile* AMOWeapon::FireProjectile(const TSubclassOf<AMOProjectile>& ProjectileClass)
{
	checkSlow(ProjectileClass != nullptr);

	FVector SpawnLocation = MOOwner->GetActorLocation() + MOOwner->GetActorRotation().RotateVector(FVector(100.f, 30.f, 10.f));

	if (GetWorld() != NULL)
	{
		return GetWorld()->SpawnActor<AMOProjectile>(ProjectileClass, MOOwner->GetActorLocation(), MOOwner->GetActorRotation());
	}
	return NULL;
}

void AMOWeapon::OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AMOCharacter* Charater = Cast<AMOCharacter>(OtherActor);
		if (Charater != nullptr && Charater->CurrentWeapon != this && this->MOOwner->bIsAttacking && this->MOOwner->bIsBot() != Charater->bIsBot())
		{
			UGameplayStatics::ApplyPointDamage(SweepResult.Actor.Get(), Damage, -SweepResult.ImpactNormal, SweepResult, NULL, this, UDamageType::StaticClass());
			this->MOOwner->bIsAttacking = false;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Projectile Collision"));
		}
	}
}