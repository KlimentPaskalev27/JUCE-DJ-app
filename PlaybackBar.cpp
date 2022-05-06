/*
  ==============================================================================

    PlaybackBar.cpp
    Created: 13 Mar 2022 7:16:07pm
    Author:  Alienware

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaybackBar.h"

//==============================================================================

/// <summary>
/// Create a custom LookAndFeel for a PlaybackBar which shows the progress of how long the song has been playing.
/// </summary>
void PlaybackBar::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
	float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) 
{
	g.fillAll(slider.findColour(juce::Slider::backgroundColourId));

	if (style == juce::Slider::LinearBar || style == juce::Slider::LinearBarVertical)
	{
		juce::Path p;

		if (style == juce::Slider::LinearBarVertical)
		{
			p.addRectangle((float)x, sliderPos, (float)width, 1.0f + height - sliderPos);
		} else {
			p.addRectangle((float)x, (float)y, sliderPos - x, (float)height);
		}
			
		g.setColour(juce::Colours::cyan);
		g.fillPath(p);

		auto lineThickness = juce::jmin(15.0f, juce::jmin(width, height) * 0.45f) * 0.1f;
		g.drawRect(slider.getLocalBounds().toFloat(), lineThickness);
	}
	else {
		drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
	}
}
