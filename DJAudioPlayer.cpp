/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 5 Mar 2022 10:42:26am
    Author:  Alienware

  ==============================================================================
*/

#include "DJAudioPlayer.h"


DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
                            : formatManager(_formatManager) 
{
    //Default reverb settings
    reverbParameters.roomSize = 0;
    reverbParameters.damping = 0;
    reverbParameters.wetLevel = 0;
    reverbParameters.dryLevel = 1.0;
    reverbSource.setParameters(reverbParameters);
};

DJAudioPlayer::~DJAudioPlayer() {

};

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) 
{
    //formatManager.registerBasicFormats();
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
};

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) 
{
    //replace resampleSource with processed sound from reverb axis blocks
    reverbSource.getNextAudioBlock(bufferToFill);
};

void DJAudioPlayer::releaseResources() 
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbSource.releaseResources();
};

void DJAudioPlayer::loadURL(juce::URL audioURL) 
{
    //takes audio url input string and passes to formatManager and creates a Reader
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    //check if it successfully created the reader
    if (reader != nullptr) //good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());

    }
};

/// <summary>
/// Sets the volume output of the player.
/// </summary>
/// <param name="gain">Takes in a double as a value for the volume. Between 0 and 1.</param>
void DJAudioPlayer::setGain(double gain) 
{
    if (gain < 0 || gain > 1.0 || gain == 0) {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
        if (gain < 0)
        {
            gain = 1; //avoid exception error. Set minimum.
        }
    }
    else {
        transportSource.setGain(gain);
    }
};

/// <summary>
/// Controls the playback speed of the song. Controlled by a slider created in DeckGUI. Can speed up up to x3 times.
/// </summary>
/// <param name="ratio">Takes in a double as a value to be used to set the resampleSource, i.e. playback speed.</param>
void DJAudioPlayer::setSpeed(double ratio) 
{
    if (ratio < 0 || ratio > 3.0 || ratio == 0) {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 3" << std::endl;
        if (ratio < 0)
        {
            ratio = 1; //avoid exception error. Set minimum.
        }
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
};

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
};

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1 || pos == 0) {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
        if (pos < 0)
        {
            pos = 0.01; //avoid exception error. Set minimum.
        }
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start() 
{
    transportSource.start();
};

void DJAudioPlayer::stop() 
{
    transportSource.stop();
};


double DJAudioPlayer::getPositionRelative()
{
    // if we dont divide, it will return the position in seconds which is not relative
    //check for division by zero otherwise get error
    if (transportSource.getLengthInSeconds() == 0) {
        //std::cout << "Error, cannot divide by zero in DJAudioPlayer::getPositionRelative()" << std::endl;
        //DBG("Error in DJAudioPlayer::getPositionRelative(). transportSource.getLengthInSeconds() equals to zero ");
    }
    else {
        return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
    }
}


double DJAudioPlayer::getLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
}


void DJAudioPlayer::setRoomSize(float size)
{
    DBG("DJAudioPlayer::setRoomSize called");
    if (size < 0 || size > 1.0)
    {
        DBG("DJAudioPlayer::setRoomSize size should be between 0 and 1.0");
    }
    else {
        reverbParameters.roomSize = size;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setDamping(float dampingAmt)
{
    DBG("DJAudioPlayer::setDamping called");
    if (dampingAmt < 0 || dampingAmt > 1.0)
    {
        DBG("DJAudioPlayer::setDamping amount should be between 0 and 1.0");
    }
    else {
        reverbParameters.damping = dampingAmt;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setWetLevel(float wetLevel)
{
    DBG("DJAudioPlayer::setWetLevel called");
    if (wetLevel < 0 || wetLevel > 1.0)
    {
        DBG("DJAudioPlayer::setWetLevel level should be between 0 and 1.0");
    }
    else {
        reverbParameters.wetLevel = wetLevel;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setDryLevel(float dryLevel)
{
    DBG("DJAudioPlayer::setDryLevel called");
    if (dryLevel < 0 || dryLevel > 1.0)
    {
        DBG("DJAudioPlayer::setDryLevel level should be between 0 and 1.0");
    }
    else {
        reverbParameters.dryLevel = dryLevel;
        reverbSource.setParameters(reverbParameters);
    }
}