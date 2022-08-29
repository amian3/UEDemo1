// Fill out your copyright notice in the Description page of Project Settings.


#include "AIProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SamuraiCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
AAIProjectile::AAIProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);

	CollisionComp->OnComponentHit.AddDynamic(this, &AAIProjectile::OnHit);
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

	Damage = 10.0f;

}


void AAIProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		ASamuraiCharacter* Man = Cast<ASamuraiCharacter>(OtherActor);
		UGameplayStatics::ApplyPointDamage(Man, Damage, GetActorLocation(), Hit, NULL, this, DamageTypeClass);
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		Destroy();
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Emitter_Projectile, GetActorLocation());//�����Ч

	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Emitter_Projectile, this->GetTransform());
}