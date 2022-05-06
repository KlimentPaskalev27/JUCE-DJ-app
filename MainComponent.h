#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  :  public juce::AudioAppComponent  
{
public:
    //==============================================================================
    MainComponent(); // (NewProjectAudioProcessor&);
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    //NewProjectAudioProcessor& audioProcessor;


    DJAudioPlayer player1{ formatManager };
    DeckGUI deckGUI1{ 1, &player1 , formatManager , thumbCache };

    DJAudioPlayer player2{ formatManager };
    DeckGUI deckGUI2{ 2, &player2 , formatManager , thumbCache };

    juce::MixerAudioSource mixerSource;

    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{ 100 }; // it will store up to a 100 waveforms at a time

    DJAudioPlayer metadataParser{ formatManager };
    PlaylistComponent playlistComponent{ &deckGUI1, &deckGUI2, &metadataParser };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
