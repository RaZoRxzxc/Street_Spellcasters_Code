// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingsWidget.h"

#include "Characters/BaseCharacter.h"
#include "Components/VerticalBox.h"
#include "Widgets/Settings/SettingsEntryWidget.h"
#include "Components/Button.h"
#include "SaveGame/BaseSaveGame.h"
#include "Sound/SoundMix.h"
#include "Kismet/GameplayStatics.h"

void USettingsWidget::NativeConstruct()
{
    Super::NativeConstruct();

    RegisterCVars();
    LoadSettings(TEXT("AudioSettings"), true);
    LoadSettings(TEXT("InputSettings"), false);
    CreateSettingsWidgets();
}

void USettingsWidget::OnDisplaySettingsClicked()
{
    // Implement if needed for opening sub-menus
}

void USettingsWidget::RegisterCVars()
{
    // Audio CVars
    IConsoleManager::Get().RegisterConsoleVariable(
        TEXT("au.MasterVolume"),
        1.0f,
        TEXT("Master volume setting"),
        ECVF_Default
    );

    IConsoleManager::Get().RegisterConsoleVariable(
        TEXT("au.MusicVolume"),
        1.0f,
        TEXT("Music volume setting"),
        ECVF_Default
    );

    IConsoleManager::Get().RegisterConsoleVariable(
        TEXT("au.SFXVolume"),
        1.0f,
        TEXT("SFX volume setting"),
        ECVF_Default
    );

    IConsoleManager::Get().RegisterConsoleVariable(
        TEXT("au.VoiceVolume"),
        1.0f,
        TEXT("Voice volume setting"),
        ECVF_Default
    );

    // Input CVars
    IConsoleManager::Get().RegisterConsoleVariable(
        TEXT("input.MouseSensitivity"),
        1.0f,
        TEXT("Mouse sensitivity multiplier"),
        ECVF_Default
    );

    IConsoleManager::Get().RegisterConsoleVariable(
        TEXT("input.ControllerSensitivity"),
        1.0f,
        TEXT("Controller sensitivity multiplier"),
        ECVF_Default
    );
}

void USettingsWidget::LoadSettings(const FString& SlotName, bool bIsAudio)
{
    if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
    {
        UE_LOG(LogTemp, Log, TEXT("No saved settings found for %s, using defaults"), *SlotName);
        SaveSettings(SlotName, bIsAudio);
        return;
    }

    UBaseSaveGame* LoadedSave = Cast<UBaseSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    if (!LoadedSave) return;

    if (bIsAudio)
    {
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.MasterVolume")))
            CVar->Set(LoadedSave->MasterVolume);
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.MusicVolume")))
            CVar->Set(LoadedSave->MusicVolume);
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.SFXVolume")))
            CVar->Set(LoadedSave->SFXVolume);
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.VoiceVolume")))
            CVar->Set(LoadedSave->VoiceVolume);

        ApplyAudioSettings(LoadedSave);
    }
    else
    {
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("input.MouseSensitivity")))
            CVar->Set(LoadedSave->MouseSensitivity);
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("input.ControllerSensitivity")))
            CVar->Set(LoadedSave->ControllerSensitivity);

        ApplyInputSettings();
    }
}

void USettingsWidget::SaveSettings(const FString& SlotName, bool bIsAudio)
{
    UBaseSaveGame* SaveGame = Cast<UBaseSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameObject));
    if (!SaveGame) return;

    if (bIsAudio)
    {
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.MasterVolume")))
            SaveGame->MasterVolume = CVar->GetFloat();
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.MusicVolume")))
            SaveGame->MusicVolume = CVar->GetFloat();
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.SFXVolume")))
            SaveGame->SFXVolume = CVar->GetFloat();
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.VoiceVolume")))
            SaveGame->VoiceVolume = CVar->GetFloat();
    }
    else
    {
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("input.MouseSensitivity")))
            SaveGame->MouseSensitivity = CVar->GetFloat();
        if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("input.ControllerSensitivity")))
            SaveGame->ControllerSensitivity = CVar->GetFloat();
    }

    UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);
}

void USettingsWidget::ApplyAudioSettings(UBaseSaveGame* SaveGame)
{
    if (!DefaultSoundMix || !SaveGame) return;

    if (MasterSoundClass)
        UGameplayStatics::SetSoundMixClassOverride(GetWorld(), DefaultSoundMix, MasterSoundClass, SaveGame->MasterVolume, 1.0f, 0.0f, true);
    if (MusicSoundClass)
        UGameplayStatics::SetSoundMixClassOverride(GetWorld(), DefaultSoundMix, MusicSoundClass, SaveGame->MusicVolume, 1.0f, 0.0f, true);
    if (SFXSoundClass)
        UGameplayStatics::SetSoundMixClassOverride(GetWorld(), DefaultSoundMix, SFXSoundClass, SaveGame->SFXVolume, 1.0f, 0.0f, true);
    if (VoiceSoundClass)
        UGameplayStatics::SetSoundMixClassOverride(GetWorld(), DefaultSoundMix, VoiceSoundClass, SaveGame->VoiceVolume, 1.0f, 0.0f, true);

    UGameplayStatics::PushSoundMixModifier(GetWorld(), DefaultSoundMix);
}

void USettingsWidget::ApplyInputSettings()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController* PlayerController = It->Get())
        {
            if (ABaseCharacter* Character = Cast<ABaseCharacter>(PlayerController->GetPawn()))
            {
                Character->UpdateInputSensitivity();
            }
        }
    }
}

void USettingsWidget::OnSliderValueChanged(float Value, USoundClass* InSoundClass, const FString& CVarName)
{
    if (CVarName.IsEmpty()) return;

    if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(*CVarName))
    {
        CVar->Set(Value);

        if (CVarName.Contains("au."))
        {
            if (DefaultSoundMix && InSoundClass)
            {
                UGameplayStatics::SetSoundMixClassOverride(GetWorld(), DefaultSoundMix, InSoundClass, Value, 1.0f, 0.0f, true);
                UGameplayStatics::PushSoundMixModifier(GetWorld(), DefaultSoundMix);
            }
            SaveSettings(TEXT("AudioSettings"), true);
        }
        else if (CVarName.Contains("input."))
        {
            ApplyInputSettings();
            SaveSettings(TEXT("InputSettings"), false);
        }
    }
}

void USettingsWidget::CreateSettingsWidgets()
{
    if (VideoSettingsVB)
    {
        // Display Settings
        AddDisplayModeSetting();
        AddResolutionSetting();

        // Graphics Quality Settings
        AddQualitySetting(FText::FromString("Texture Quality"), TEXT("sg.TextureQuality"));
        AddQualitySetting(FText::FromString("Antialiasing Quality"), TEXT("sg.AntiAliasingQuality"));
        AddQualitySetting(FText::FromString("Postprocessing Quality"), TEXT("sg.PostProcessQuality"));
        AddQualitySetting(FText::FromString("Shadow Quality"), TEXT("sg.ShadowQuality"));
        AddQualitySetting(FText::FromString("Reflections Quality"), TEXT("sg.ReflectionQuality"));
        AddQualitySetting(FText::FromString("Shader Quality"), TEXT("sg.ShadingQuality"));
        AddQualitySetting(FText::FromString("Global Illumination Quality"), TEXT("sg.GlobalIlluminationQuality"));
        AddQualitySetting(FText::FromString("View Distance Quality"), TEXT("sg.ViewDistanceQuality"));
        AddQualitySetting(FText::FromString("Foliage Quality"), TEXT("sg.FoliageQuality"));
    }

    if (AudioSettingsVB)
    {
        // Audio Settings
        AddSliderSetting(AudioSettingsVB, FText::FromString("Master Volume"), TEXT("au.MasterVolume"));
        AddSliderSetting(AudioSettingsVB, FText::FromString("Music Volume"), TEXT("au.MusicVolume"));
        AddSliderSetting(AudioSettingsVB, FText::FromString("SFX Volume"), TEXT("au.SFXVolume"));
        AddSliderSetting(AudioSettingsVB, FText::FromString("Voice Volume"), TEXT("au.VoiceVolume"));
    }

    if (InputSettingsVB && SensitivityBox)
    {
        // Sensitivity Settings
        AddSliderSetting(SensitivityBox, FText::FromString("Mouse Sensitivity"), TEXT("input.MouseSensitivity"));
        AddSliderSetting(SensitivityBox, FText::FromString("Controller Sensitivity"), TEXT("input.ControllerSensitivity"));
    }
}

void USettingsWidget::AddDisplayModeSetting()
{
    if (VideoSettingsVB && SettingsEntryWidgetClass)
    {
        USettingsEntryWidget* DisplayModeWidget = CreateWidget<USettingsEntryWidget>(this, SettingsEntryWidgetClass);
        if (DisplayModeWidget)
        {
            DisplayModeWidget->InitializeSetting(
                FText::FromString("Display Mode"),
                ESettingWidgetType::DropDown,
                TEXT(""),
                EDisplaySettingType::DisplayMode
            );
            VideoSettingsVB->AddChild(DisplayModeWidget);
        }
    }
}

void USettingsWidget::AddResolutionSetting()
{
    if (VideoSettingsVB && SettingsEntryWidgetClass)
    {
        USettingsEntryWidget* ResolutionWidget = CreateWidget<USettingsEntryWidget>(this, SettingsEntryWidgetClass);
        if (ResolutionWidget)
        {
            ResolutionWidget->InitializeSetting(
                FText::FromString("Resolution"),
                ESettingWidgetType::DropDown,
                TEXT(""),
                EDisplaySettingType::Resolution
            );
            VideoSettingsVB->AddChild(ResolutionWidget);
        }
    }
}

void USettingsWidget::AddQualitySetting(const FText& DisplayName, const FString& CVarName)
{
    if (VideoSettingsVB && SettingsEntryWidgetClass)
    {
        USettingsEntryWidget* QualityWidget = CreateWidget<USettingsEntryWidget>(this, SettingsEntryWidgetClass);
        if (QualityWidget)
        {
            QualityWidget->InitializeSetting(
                DisplayName,
                ESettingWidgetType::DoubleButton,
                CVarName
            );
            VideoSettingsVB->AddChild(QualityWidget);
        }
    }
}

void USettingsWidget::AddSliderSetting(UVerticalBox* TargetVerticalBox, const FText& DisplayName, const FString& CVarName)
{
    if (TargetVerticalBox && SettingsEntryWidgetClass)
    {
        USettingsEntryWidget* SliderWidget = CreateWidget<USettingsEntryWidget>(this, SettingsEntryWidgetClass);
        if (SliderWidget)
        {
          
            SliderWidget->OnSliderValueChangedDelegate.AddDynamic(this, &USettingsWidget::OnSliderValueChanged);

            if (CVarName.Contains("au."))
            {
                if (CVarName == TEXT("au.MasterVolume"))
                    SliderWidget->AssociatedSoundClass = MasterSoundClass;
                else if (CVarName == TEXT("au.MusicVolume"))
                    SliderWidget->AssociatedSoundClass = MusicSoundClass;
                else if (CVarName == TEXT("au.SFXVolume"))
                    SliderWidget->AssociatedSoundClass = SFXSoundClass;
                else if (CVarName == TEXT("au.VoiceVolume"))
                    SliderWidget->AssociatedSoundClass = VoiceSoundClass;
            }

            SliderWidget->InitializeSetting(
                DisplayName,
                ESettingWidgetType::Slider,
                CVarName
            );
            TargetVerticalBox->AddChild(SliderWidget);
        }
    }
}

