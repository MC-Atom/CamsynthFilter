/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CamsynthFilterAudioProcessorEditor::CamsynthFilterAudioProcessorEditor (CamsynthFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    audioProcessor.socketServer->onSocketStatusChange = [this]() {
        repaint();
    };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible(audioProcessor.httpState);
    audioProcessor.httpState.setText("Waiting to Connect",juce::sendNotification);

    addAndMakeVisible(modeDropdown);
    modeDropdown.addItem("Low Pass Filter",1);
    modeDropdown.addItem("Hi Pass Filter",2);
    modeDropdown.addItem("Band Pass Filter",3);
    modeDropdown.setSelectedId(1);

    biasSlider.setSliderStyle (juce::Slider::SliderStyle::LinearHorizontal);
    biasSlider.setRange(1,99,1);
    biasSlider.setValue(20);
    audioProcessor.bias.store(20);
    biasSlider.addListener(this);
    addAndMakeVisible(biasSlider);

    midpointSlider.setSliderStyle (juce::Slider::SliderStyle::LinearHorizontal);
    midpointSlider.setRange(20,20000,1.0f);
    midpointSlider.setValue(20000);
    midpointSlider.addListener(this);
    midpointSlider.setSkewFactor(0.3);
    midpointSlider.setTextValueSuffix(" Hz");
    addAndMakeVisible(midpointSlider);

    resonanceSlider.setSliderStyle (juce::Slider::SliderStyle::LinearHorizontal);
    resonanceSlider.setRange(0,1,0.05f);
    resonanceSlider.setValue(0);
    resonanceSlider.addListener(this);
    //resonanceSlider.setTextValueSuffix(" db");
    addAndMakeVisible(resonanceSlider);

    scalarSlider.setSliderStyle (juce::Slider::SliderStyle::LinearHorizontal);
    scalarSlider.setRange(0,10,0.05f);
    scalarSlider.setValue(1);
    scalarSlider.addListener(this);
    scalarSlider.setSkewFactor(0.3);
    addAndMakeVisible(scalarSlider);

    addAndMakeVisible(modeLabel);
    addAndMakeVisible(biasLabel);
    addAndMakeVisible(midpointLabel);
    addAndMakeVisible(resonanceLabel);
    addAndMakeVisible(scalarLabel);

    modeLabel.attachToComponent(&modeDropdown,false);
    biasLabel.attachToComponent(&biasSlider,true);
    midpointLabel.attachToComponent(&midpointSlider,true);
    resonanceLabel.attachToComponent(&resonanceSlider,true);

    modeLabel.setText("Mode", juce::dontSendNotification);
    biasLabel.setText("Bias", juce::dontSendNotification);
    midpointLabel.setText("Midpoint", juce::dontSendNotification);
    resonanceLabel.setText("Resonance", juce::dontSendNotification);
}

void CamsynthFilterAudioProcessorEditor::sliderValueChanged (juce::Slider* slider) {
    if (slider == &biasSlider) {
        //audioProcessor.adsrParams.attack = slider->getValue();
        audioProcessor.bias.store(slider->getValue());
    }
    if (slider == &midpointSlider) {
        //audioProcessor.adsrParams.release = slider->getValue();
        audioProcessor.midpoint.store(slider->getValue());
    }
    if (slider == &resonanceSlider) {
        //audioProcessor.adsrParams.release = slider->getValue();
        audioProcessor.ladder.setResonance(slider->getValue());
    }
    if (slider == &scalarSlider) {
        //audioProcessor.adsrParams.release = slider->getValue();
        audioProcessor.filterScalar.store(slider->getValue());
    }
}

CamsynthFilterAudioProcessorEditor::~CamsynthFilterAudioProcessorEditor()
{
}

//==============================================================================
void CamsynthFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void CamsynthFilterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    constexpr int padding = 20;

    audioProcessor.httpState.setBounds(padding, padding,getWidth()-(padding*2), 20);

    modeDropdown.setBounds(padding+30, padding*3, (getWidth()/2)-(padding*2)-30,20);
    biasSlider.setBounds((getWidth()/2)+padding, padding*3, (getWidth()/2)-(padding*2),20);

    midpointSlider.setBounds((getWidth()/2)+padding, padding*6, (getWidth()/2)-(padding*2),20);
    resonanceSlider.setBounds((getWidth()/2)+padding, padding*8, (getWidth()/2)-(padding*2),20);
    scalarSlider.setBounds((getWidth()/2)+padding, padding*10, (getWidth()/2)-(padding*2),20);
    //rSlider.setBounds((getWidth()/2)+padding, padding*12, (getWidth()/2)-(padding*2),20);

    //aLabel.setBounds(padding, padding*4, (getWidth()/2)-(padding*2),20);
}
