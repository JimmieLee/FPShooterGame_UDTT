// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BulletImpactComponent.generated.h"


UCLASS( ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent) )
class FPSHOOTERGAME_API UBulletImpactComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBulletImpactComponent();

private:
	// ÄÜÅ©¸®Æ® ÀçÁúÀÇ ÂøÅº È¿°ú.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact FX", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* ImpactContrete;

	// ±Ý¼Ó ÀçÁúÀÇ ÂøÅº È¿°ú.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact FX", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* ImpactMetal;

	// ¿¬Ã¼ ÀçÁúÀÇ ÂøÅº È¿°ú.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact FX", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* ImpactFlesh;

	// ÄÜÅ©¸®Æ® ÀçÁúÀÇ ÂøÅº È¿°úÀ½.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact SFX", meta = (AllowPrivateAccess = "true"))
	class USoundBase* ImpactContreteSound;

	// ±Ý¼Ó ÀçÁúÀÇ ÂøÅº È¿°úÀ½.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact SFX", meta = (AllowPrivateAccess = "true"))
	class USoundBase* ImpactMetalSound;

	// ¿¬Ã¼ ÀçÁúÀÇ ÂøÅº È¿°úÀ½.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact SFX", meta = (AllowPrivateAccess = "true"))
	class USoundBase* ImpactFleshSound;

	// ÄÜÅ©¸®Æ® ÀçÁúÀÇ ÂøÅº µ¥Ä® ÀçÁú.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact Decal", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* ImapactConcreteDecal;

	// ±Ý¼Ó ÀçÁúÀÇ ÂøÅº µ¥Ä® ÀçÁú.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact Decal", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* ImapactMetalDecal;

	// ¿¬Ã¼ ÀçÁúÀÇ ÂøÅº µ¥Ä® ÀçÁú.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact Decal", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* ImapactFlashDecal;

	// µ¥Ä® Å©±â
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact Decal", meta = (AllowPrivateAccess = "true"))
	FVector DecalSize;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PlayImpactEffects(FHitResult ImpactHit);
};
