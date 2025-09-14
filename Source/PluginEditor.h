/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CamsynthFilterAudioProcessorEditor  : public juce::AudioProcessorEditor,
public juce::Slider::Listener
{
public:
    CamsynthFilterAudioProcessorEditor (CamsynthFilterAudioProcessor&);
    ~CamsynthFilterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CamsynthFilterAudioProcessor& audioProcessor;

    juce::ComboBox modeDropdown;
    juce::Slider biasSlider;
    juce::Slider midpointSlider;
    juce::Slider resonanceSlider;
    juce::Slider scalarSlider;

    juce::Label modeLabel;
    juce::Label biasLabel;
    juce::Label midpointLabel;
    juce::Label resonanceLabel;
    juce::Label scalarLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CamsynthFilterAudioProcessorEditor)
};
