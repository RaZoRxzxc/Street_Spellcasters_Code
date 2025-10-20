// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/EditableTextBox.h"
#include "Engine/GameViewportClient.h"
#include "InputAction.h"
#include "InputCoreTypes.h"
#include "SettingsEntryWidget.generated.h"

class UTextBlock;
class UComboBoxKey;
class UHorizontalBox;
class UInputKeySelector;

UENUM(BlueprintType)
enum class ESettingWidgetType : uint8
{
	DropDown,
	SingleButton,
	DoubleButton,
	Slider,
	InputKeySelector
};

UENUM(BlueprintType)
enum class EDisplaySettingType : uint8
{
	DisplayMode,
	Resolution
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSliderValueChanged, float, Volume, USoundClass*, SoundClass, const FString&, CVarName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKeySelectedDelegate, FKey, NewKey, UInputAction*, InputAction);
UCLASS()
class STREET_SPELLCASTERS_API USettingsEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
   UFUNCTION(BlueprintCallable, Category = "Settings")
    void InitializeSetting(const FText& InDisplayName, ESettingWidgetType InWidgetType, const FString& InCVarName = "", EDisplaySettingType InDisplayType = EDisplaySettingType::Resolution);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESettingWidgetType WidgetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDisplaySettingType DisplayType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundClass* AssociatedSoundClass;

	UPROPERTY(BlueprintAssignable, Category = "Settings")
	FOnKeySelectedDelegate OnKeySelectedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Settings")
	FOnSliderValueChanged OnSliderValueChangedDelegate;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* AssociatedInputAction;
	
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnComboSelectionChanged(FName SelectedItem, ESelectInfo::Type SelectInfo);

    UFUNCTION()
    void OnLeftButtonClicked();

    UFUNCTION()
    void OnRightButtonClicked();

    UFUNCTION()
    void OnSingleButtonClicked();

    UFUNCTION()
    void OnSliderValueChanged(float Value);

    UFUNCTION()
    void OnKeySelected(FInputChord SelectedKey);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SettingNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UComboBoxKey* DropDownCombo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* SingleButtonBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SingleButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* DoubleButtonBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* LeftButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* RightButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* SliderBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USlider* SettingSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* InputKeyBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInputKeySelector* KeySelector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* QualityText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SliderValueText;

	FText DisplayName;
 
	FString CVarName;

private:
	
    void UpdateVisibility();
    void PopulateDisplayModes();
    void PopulateResolutions();
    void ApplyDisplayMode();
    void ApplyResolution();
    FString GetResolutionString(const FIntPoint& Resolution);
    int32 GetDisplayModeIndex(EWindowMode::Type Mode);
    void UpdateQualityText();
    IConsoleVariable* GetCVar() const;
    
    TArray<FString> DisplayModeOptions = { "Windowed", "Fullscreen", "Windowed Fullscreen" };
    EWindowMode::Type CurrentDisplayMode;
    
    TArray<FString> ResolutionOptions;
    FIntPoint CurrentResolution;

    int32 CurrentLevel;
    TArray<FText> QualityLevels = { FText::FromString("Low"), FText::FromString("Medium"), FText::FromString("High"), FText::FromString("Epic") };

    float CurrentSliderValue;
    float MinSliderValue = 0.0f;
    float MaxSliderValue = 1.0f;
};
