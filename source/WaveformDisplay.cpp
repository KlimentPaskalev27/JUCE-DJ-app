/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 5 Mar 2022 2:14:41pm
    Author:  Alienware

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================

/// <summary>
/// The constructor function for the Waveform component which draws a waveform based on the audio file's properties.
/// </summary>
/// <param name="formatManagerToUse">An instance of the juce::AudioFormatManager to use for this.</param>
/// <param name="cacheToUse">The current cache loaded from the audio file.</param>
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse) :  //: to initialise variables 
                                 audioThumb(1000, formatManagerToUse, cacheToUse),
                                 fileLoaded(false),
                                 position(0)
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

/// <summary>
/// Draws and styles the component.
/// </summary>
/// <param name="g"></param>
void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::darkcyan); //colour of the wave
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    g.setColour (juce::Colours::darkcyan); // colour of box outline

    if (fileLoaded) {
        audioThumb.drawChannel(g,   //graphics
                                getLocalBounds(),  //size where to draw
                                0,   // start time
                                audioThumb.getTotalLength(),  //end time
                                0,  // channel
                                1.0f);  //vertical zoom factor 

        g.setColour(juce::Colours::cyan);
        if (position > 0 && getWidth() > 0) {
            g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
            //check for this to avoid errors
            DBG("Nothing negative in WaveformDisplay::paint");
        }
        else if (position < 0 && getWidth() > 0) {

            DBG("position is negative  in WaveformDisplay::paint ");
        }
        else if (position > 0 && getWidth() < 0) {
            DBG("getWidth() is negative  in WaveformDisplay::paint");
        }
        else {
            //avoid exception
            g.drawRect(0, 0, getWidth() / 20, getHeight());
            DBG("both are negative  in WaveformDisplay::paint");
        }
    }
    else {
        g.setFont(20.0f);
        g.drawText("Song not added yet...", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // Nothing to add here.
}

/// <summary>
/// Loads up the URL that is passed to this function and uses it to paint the waveform.
/// </summary>
/// <param name="audioURL"></param>
void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumb.clear(); //clear previous audio
    //check if the new audio file is loaded successfully
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));

    if (fileLoaded) {
        DBG("File was loaded successfully");
        repaint();
    }
    else {
        DBG("File was not loaded");
    }
}

/// <summary>
/// Repaints.
/// </summary>
/// <param name="source"></param>
void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    repaint();
}

/// <summary>
/// Changes the relative position of the playhead, i.e. the waveform square which tracks the playtime of current song.
/// </summary>
/// <param name="pos"></param>
void WaveformDisplay::setPositionRelative(double pos)
{
    //only repaint if the position has changed
    if (pos != position) {
        position = pos; // hold the input position in a local position var
        repaint();
    }
}