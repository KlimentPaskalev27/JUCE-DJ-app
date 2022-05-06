/*
  ==============================================================================

    DeckGUI.h
    Created: 5 Mar 2022 12:22:12pm
    Author:  Alienware

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "AxisModificator.h"
#include "PlaybackBar.h"


//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer,
                 public AxisModificator::EventListener
{
    public:
        DeckGUI(int _id,
            DJAudioPlayer* player,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse
        ); 
        ~DeckGUI() override;

        void paint (juce::Graphics&) override;
        void resized() override;
        void buttonClicked(juce::Button* button) ;
        void sliderValueChanged(juce::Slider* slider);
        bool isInterestedInFileDrag(const juce::StringArray &files) override;
        void filesDropped(const juce::StringArray &files, int x, int y) override;
        void timerCallback() override;
        void axisModificatorValueChange(AxisModificator* axisModificator) override;

    private:

        int id;
        // to toggle between play and pause state of button
        bool isOn = false;

        juce::LookAndFeel_V4 sliderLookAndFeel; //slider styles
        PlaybackBar playbackBarLAF; //play progress bar look and feel

        juce::DrawableButton* forwardButton;
        std::unique_ptr<juce::Drawable> forwardButtonImage;
        std::unique_ptr<juce::Drawable> forwardButtonImageDown;


        juce::DrawableButton* reverseButton;
        std::unique_ptr<juce::Drawable> reverseButtonImage;
        std::unique_ptr<juce::Drawable> reverseButtonImageDown;

        juce::DrawableButton* playButton;

        std::unique_ptr<juce::Drawable> playButtonImage;
        std::unique_ptr<juce::Drawable> playButtonImageDown;
        std::unique_ptr<juce::Drawable> pauseButtonImage;
        std::unique_ptr<juce::Drawable> pauseButtonImageDown;


        juce::Label volLabel;
        juce::Label speedLabel;
        juce::Label posLabel;
        juce::Slider volSlider;
        juce::Slider speedSlider;
        juce::Slider posSlider;

        juce::Slider axisSlider;
        AxisModificator axisModificator1;
        AxisModificator axisModificator2;

        juce::SharedResourcePointer<juce::TooltipWindow> sharedTooltip;

        DJAudioPlayer* player;
        WaveformDisplay waveformDisplay;

        void loadFile(juce::URL audioURL);
        friend class PlaylistComponent; //allow access from PlaylistComponent to private members of this class DeckGUI

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI);
};
