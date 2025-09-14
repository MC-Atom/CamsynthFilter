/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SocketServer.h"

//==============================================================================
/**
*/
class CamsynthFilterAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    CamsynthFilterAudioProcessor();
    ~CamsynthFilterAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //juce::dsp::FIR::Filter<float>::CoefficientsPtr firCoefPnr =  juce::dsp::FIR::Coefficients<NumericType>::Ptr
    //juce::dsp::ProcessorDuplicator<juce::dsp::FIR::Filter<float>,juce::dsp::FIR::Coefficients<float>> fir;

	std::unique_ptr<SocketServer> socketServer;
    juce::dsp::LadderFilter<float> ladder;
    std::atomic<int> cutFreq;
    std::atomic<int> bias;
    std::atomic<int> midpoint;
    std::atomic<float> filterScalar;
    juce::Label httpState;

    float filterCutoff { 0.5 };

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CamsynthFilterAudioProcessor)
};
