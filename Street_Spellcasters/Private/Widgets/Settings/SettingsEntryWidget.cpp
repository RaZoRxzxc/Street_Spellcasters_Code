// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Settings/SettingsEntryWidget.h"
#include "Components/ComboBoxKey.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/InputKeySelector.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/GameUserSettings.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputCoreTypes.h" // For FKey and FInputChord
#include "InputAction.h"

void USettingsEntryWidget::InitializeSetting(const FText& InDisplayName, ESettingWidgetType InWidgetType, const FString& InCVarName, EDisplaySettingType InDisplayType)
{
    DisplayName = InDisplayName;
    WidgetType = InWidgetType;
    CVarName = InCVarName;
    DisplayType = InDisplayType;

    if (SettingNameText)
    {
        SettingNameText->SetText(DisplayName);
    }

    UpdateVisibility();

    if (WidgetType == ESettingWidgetType::DropDown)
    {
        if (UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings())
        {
            if (DisplayType == EDisplaySettingType::DisplayMode)
            {
                CurrentDisplayMode = GameUserSettings->GetFullscreenMode();
                PopulateDisplayModes();
                DropDownCombo->SetSelectedOption(FName(*DisplayModeOptions[GetDisplayModeIndex(CurrentDisplayMode)]));
            }
            else
            {
                CurrentResolution = GameUserSettings->GetScreenResolution();
                PopulateResolutions();
                FString CurrentResString = GetResolutionString(CurrentResolution);
                int32 CurrentIndex = ResolutionOptions.Find(CurrentResString);
                if (CurrentIndex != INDEX_NONE)
                {
                    DropDownCombo->SetSelectedOption(FName(*ResolutionOptions[CurrentIndex]));
                }
            }
        }
    }
    else if (WidgetType == ESettingWidgetType::DoubleButton)
    {
        IConsoleVariable* CVar = GetCVar();
        if (CVar)
        {
            CurrentLevel = CVar->GetInt();
            UpdateQualityText();
        }
    }
    else if (WidgetType == ESettingWidgetType::Slider)
    {
        if (SettingSlider)
        {
            IConsoleVariable* CVar = GetCVar();
            if (CVar && !CVarName.IsEmpty())
            {
                CurrentSliderValue = CVar->GetFloat();
            }
            else
            {
                CurrentSliderValue = 1.0f;
            }
        
            SettingSlider->SetValue(CurrentSliderValue);
            if (SliderValueText)
            {
                SliderValueText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentSliderValue * 100)));
            }
        }
    }
    else if (WidgetType == ESettingWidgetType::InputKeySelector && KeySelector && AssociatedInputAction)
    {
      
    }
}

void USettingsEntryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (DropDownCombo)
    {
        DropDownCombo->OnSelectionChanged.AddDynamic(this, &USettingsEntryWidget::OnComboSelectionChanged);
    }

    if (LeftButton)
    {
        LeftButton->OnClicked.AddDynamic(this, &USettingsEntryWidget::OnLeftButtonClicked);
    }

    if (RightButton)
    {
        RightButton->OnClicked.AddDynamic(this, &USettingsEntryWidget::OnRightButtonClicked);
    }

    if (SingleButton)
    {
        SingleButton->OnClicked.AddDynamic(this, &USettingsEntryWidget::OnSingleButtonClicked);
    }

    if (SettingSlider)
    {
        SettingSlider->OnValueChanged.AddDynamic(this, &USettingsEntryWidget::OnSliderValueChanged);
    }

    if (KeySelector)
    {
        KeySelector->OnKeySelected.AddDynamic(this, &USettingsEntryWidget::OnKeySelected);
    }

    UpdateVisibility();
}

void USettingsEntryWidget::UpdateVisibility()
{
    if (DropDownCombo && DropDownCombo->GetParent()) 
        Cast<UHorizontalBox>(DropDownCombo->GetParent())->SetVisibility(WidgetType == ESettingWidgetType::DropDown ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    
    if (SingleButtonBox) 
        SingleButtonBox->SetVisibility(WidgetType == ESettingWidgetType::SingleButton ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    
    if (DoubleButtonBox) 
        DoubleButtonBox->SetVisibility(WidgetType == ESettingWidgetType::DoubleButton ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    
    if (SliderBox) 
        SliderBox->SetVisibility(WidgetType == ESettingWidgetType::Slider ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    
    if (InputKeyBox) 
        InputKeyBox->SetVisibility(WidgetType == ESettingWidgetType::InputKeySelector ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void USettingsEntryWidget::OnComboSelectionChanged(FName SelectedItem, ESelectInfo::Type SelectInfo)
{
    FString SelectedString = SelectedItem.ToString();
    
    if (DisplayType == EDisplaySettingType::DisplayMode)
    {
        int32 Index = DisplayModeOptions.Find(SelectedString);
        switch (Index)
        {
            case 0: CurrentDisplayMode = EWindowMode::Windowed; break;
            case 1: CurrentDisplayMode = EWindowMode::Fullscreen; break;
            case 2: CurrentDisplayMode = EWindowMode::WindowedFullscreen; break;
        }
        ApplyDisplayMode();
    }
    else
    {
        int32 XPos = SelectedString.Find(TEXT("x"));
        FString WidthStr = SelectedString.Left(XPos);
        FString HeightStr = SelectedString.Mid(XPos + 1);
        CurrentResolution.X = FCString::Atoi(*WidthStr);
        CurrentResolution.Y = FCString::Atoi(*HeightStr);
        ApplyResolution();
    }
}

void USettingsEntryWidget::OnLeftButtonClicked()
{
    IConsoleVariable* CVar = GetCVar();
    if (CVar)
    {
        CurrentLevel = FMath::Clamp(CurrentLevel - 1, 0, QualityLevels.Num() - 1);
        CVar->Set(CurrentLevel);
        UpdateQualityText();
    }
}

void USettingsEntryWidget::OnRightButtonClicked()
{
    IConsoleVariable* CVar = GetCVar();
    if (CVar)
    {
        CurrentLevel = FMath::Clamp(CurrentLevel + 1, 0, QualityLevels.Num() - 1);
        CVar->Set(CurrentLevel);
        UpdateQualityText();
    }
}

void USettingsEntryWidget::OnSingleButtonClicked()
{
    // Implement functionality for single button
}

void USettingsEntryWidget::OnSliderValueChanged(float Value)
{
    CurrentSliderValue = Value;
    
    if (SliderValueText)
    {
        SliderValueText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentSliderValue * 100)));
    }
    
    if (!CVarName.IsEmpty())
    {
        if (IConsoleVariable* CVar = GetCVar())
        {
            CVar->Set(CurrentSliderValue);
        }
    }
    
    OnSliderValueChangedDelegate.Broadcast(CurrentSliderValue, AssociatedSoundClass, CVarName);
}

void USettingsEntryWidget::OnKeySelected(FInputChord SelectedKey)
{
    
}

void USettingsEntryWidget::PopulateDisplayModes()
{
    if (DropDownCombo)
    {
        DropDownCombo->ClearOptions();
        for (const FString& Mode : DisplayModeOptions)
        {
            DropDownCombo->AddOption(FName(*Mode));
        }
    }
}

void USettingsEntryWidget::PopulateResolutions()
{
    if (DropDownCombo)
    {
        DropDownCombo->ClearOptions();
        ResolutionOptions.Empty();

        TArray<FIntPoint> CommonResolutions = {
            FIntPoint(1920, 1080), FIntPoint(2560, 1440), FIntPoint(3840, 2160),
            FIntPoint(1366, 768), FIntPoint(1600, 900), FIntPoint(1680, 1050),
            FIntPoint(1280, 720), FIntPoint(1024, 768), FIntPoint(800, 600),
            FIntPoint(1440, 900), FIntPoint(1920, 1200)
        };

        CommonResolutions.Sort([](const FIntPoint& A, const FIntPoint& B) {
            return (A.X * A.Y) > (B.X * B.Y);
        });

        for (const FIntPoint& Res : CommonResolutions)
        {
            FString ResString = GetResolutionString(Res);
            ResolutionOptions.Add(ResString);
            DropDownCombo->AddOption(FName(*ResString));
        }
    }
}

void USettingsEntryWidget::ApplyDisplayMode()
{
    if (UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings())
    {
        GameUserSettings->SetFullscreenMode(CurrentDisplayMode);
        GameUserSettings->ApplySettings(false);
    }
}

void USettingsEntryWidget::ApplyResolution()
{
    if (UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings())
    {
        GameUserSettings->SetScreenResolution(CurrentResolution);
        GameUserSettings->ApplySettings(false);
    }
}

FString USettingsEntryWidget::GetResolutionString(const FIntPoint& Resolution)
{
    return FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
}

int32 USettingsEntryWidget::GetDisplayModeIndex(EWindowMode::Type Mode)
{
    switch (Mode)
    {
        case EWindowMode::Windowed: return 0;
        case EWindowMode::Fullscreen: return 1;
        case EWindowMode::WindowedFullscreen: return 2;
        default: return 1;
    }
}

void USettingsEntryWidget::UpdateQualityText()
{
    if (QualityText && CurrentLevel >= 0 && CurrentLevel < QualityLevels.Num())
    {
        QualityText->SetText(QualityLevels[CurrentLevel]);
    }
}

IConsoleVariable* USettingsEntryWidget::GetCVar() const
{
    return IConsoleManager::Get().FindConsoleVariable(*CVarName);
}