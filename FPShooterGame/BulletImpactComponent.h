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
	// ��ũ��Ʈ ������ ��ź ȿ��.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact FX", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* ImpactContrete;

	// �ݼ� ������ ��ź ȿ��.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact FX", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* ImpactMetal;

	// ��ü ������ ��ź ȿ��.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact FX", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* ImpactFlesh;

	// ��ũ��Ʈ ������ ��ź ȿ����.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact SFX", meta = (AllowPrivateAccess = "true"))
	class USoundBase* ImpactContreteSound;

	// �ݼ� ������ ��ź ȿ����.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact SFX", meta = (AllowPrivateAccess = "true"))
	class USoundBase* ImpactMetalSound;

	// ��ü ������ ��ź ȿ����.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact SFX", meta = (AllowPrivateAccess = "true"))
	class USoundBase* ImpactFleshSound;

	// ��ũ��Ʈ ������ ��ź ��Į ����.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact Decal", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* ImapactConcreteDecal;

	// �ݼ� ������ ��ź ��Į ����.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact Decal", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* ImapactMetalDecal;

	// ��ü ������ ��ź ��Į ����.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact Decal", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* ImapactFlashDecal;

	// ��Į ũ��
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
