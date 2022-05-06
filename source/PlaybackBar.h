/*
  ==============================================================================

    PlaybackBar.h
    Created: 13 Mar 2022 7:16:07pm
    Author:  Alienware

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PlaybackBar : public juce::LookAndFeel_V4 {
	public:
		void drawLinearSlider(juce::Graphics& g, 
							int x, 
							int y, 
							int width, 
							int height,
							float sliderPos, 
							float minSliderPos, 
							float maxSliderPos,
							const juce::Slider::SliderStyle style, 
							juce::Slider& slider) 
					override;
};
