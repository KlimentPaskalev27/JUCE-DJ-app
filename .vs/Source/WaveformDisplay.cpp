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
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse) :  //: to initialise variables 
                                 audioThumb(1000, formatManagerToUse, cacheToUse),
                                 fileLoaded(false),
                                 position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::orange);
    

    if (fileLoaded) {
        audioThumb.drawChannel(g,   //graphics
                                getLocalBounds(),  //size where to draw
                                0,   // start time
                                audioThumb.getTotalLength(),  //end time
                                0,  // channel
                                1.0f);  //vertical zoom factor 

        g.setColour(juce::Colours::lightgreen);
        if (position > 0 && getWidth() > 0) {
            g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
            DBG("Nothing negative in WaveformDisplay::paint");
        }
        else if (position < 0 && getWidth() > 0) {

            DBG("position is negative  in WaveformDisplay::paint ");
        }
        else if (position > 0 && getWidth() < 0) {
            DBG("getWidth() is negative  in WaveformDisplay::paint");
        }
        else {
            g.drawRect(0, 0, getWidth() / 20, getHeight());
            DBG("both are negative  in WaveformDisplay::paint");
        }
        
    }
    else {
        g.setFont(20.0f);
        g.drawText("Load not loaded...", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumb.clear();

    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));

    if (fileLoaded) {
        std::cout << "WaveformDisplay::loadURL Loaded" << std::endl;
        DBG("File was loaded successfully");
        repaint();
    }
    else {
        std::cout << "WaveformDisplay::loadURL Not Loaded !!!!" << std::endl;
        DBG("File was not loaded");
    }
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    //only repaint if the position has changed
    if (pos != position) {
        position = pos; // hold the input position in a local position var
        repaint();
    }
}