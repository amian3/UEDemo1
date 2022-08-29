// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "StandardAI.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>


AProjectile::AProjectile()
{


	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);

	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");		// set up a notification for when this component hits something blocking


	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	//CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);


	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	Damage = 30.0f;
}



void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		AStandardAI* AI = Cast<AStandardAI>(OtherActor);
		UGameplayStatics::ApplyPointDamage(AI, Damage, GetActorLocation(), Hit, NULL, NULL, DamageTypeClass);
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		Destroy();
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Emitter_Projectile, GetActorLocation());//Éä»÷ÌØÐ§
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Emitter_Projectile, this->GetTransform());
}
/*

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AAICharacter* AI = Cast<AAICharacter>(OtherActor);
		if (AI)
		{
			//UGameplayStatics::ApplyDamage(AI, Damage, )
		//	UGameplayStatics::ApplyDamage(AI, Damage, this->GetController(), this, DamageTypeClass);
			UGameplayStatics::ApplyPointDamage(AI, Damage, GetActorLocation(), SweepResult, NULL, NULL, UDamageType::StaticClass());
		}
		Destroy();
	}
}

void AProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}
*/