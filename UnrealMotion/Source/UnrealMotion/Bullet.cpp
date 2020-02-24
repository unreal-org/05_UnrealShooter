// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/Class.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);   
	CollisionMesh->SetNotifyRigidBodyCollision(true);  
	CollisionMesh->SetVisibility(false);

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::LaunchProjectile(float Speed)
{
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector* Speed);
	ProjectileMovement->Activate();
	
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	ExplosionForce->FireImpulse();

	SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();

	// apply damage
	UGameplayStatics::ApplyRadialDamage(
		this,
		10,
		GetActorLocation(),
		ExplosionForce->Radius,   // for consistency
		UDamageType::StaticClass(),
		TArray<AActor*>()  // damage all actors
	);

	// Bullet clean-up
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ABullet::OnTimerExpire, 5, false);
}

void ABullet::OnTimerExpire()
{
	Destroy();
}
