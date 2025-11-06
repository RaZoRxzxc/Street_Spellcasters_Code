// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWidget.generated.h"

class UEnhancedInputUserSettings;
class UVerticalBox;
class UGfxSettingsWidget;
class USettingsEntryWidget;
class UButton;
class UBaseSaveGame;
class USlider;

UCLASS()
class STREET_SPELLCASTERS_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnDisplaySettingsClicked();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USettingsEntryWidget* SettingsEntryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USettingsEntryWidget> SettingsEntryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Sounds)
	USoundMix* DefaultSoundMix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Sounds)
	USoundClass* MasterSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Sounds)
	USoundClass* MusicSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Sounds)
	USoundClass* SFXSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Sounds)
	USoundClass* VoiceSoundClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBaseSaveGame> SaveGameObject;
	
   	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UVerticalBox* InputSettingsVB;
private:
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VideoSettingsVB;
	
	UPROPERTY(meta = (BindWidget))
    UVerticalBox* AudioSettingsVB;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* SensitivityBox;
	
	void RegisterCVars();
	
	void LoadSettings(const FString& SlotName, bool bIsAudio);
	void SaveSettings(const FString& SlotName, bool bIsAudio);
	void ApplyAudioSettings(UBaseSaveGame* SaveGame);
	void ApplyInputSettings();
	
	UFUNCTION()
	void OnSliderValueChanged(float Value, USoundClass* InSoundClass, const FString& CVarName);
	
	void CreateSettingsWidgets();
	void AddDisplayModeSetting();
	void AddResolutionSetting();
	void AddQualitySetting(const FText& DisplayName, const FString& CVarName);
	void AddSliderSetting(UVerticalBox* TargetVerticalBox, const FText& DisplayName, const FString& CVarName = "");
};
