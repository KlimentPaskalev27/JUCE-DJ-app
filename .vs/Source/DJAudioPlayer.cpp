/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 5 Mar 2022 10:42:26am
    Author:  Alienware

  ==============================================================================
*/

#include "DJAudioPlayer.h"


DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
                            : formatManager(_formatManager) {

};

DJAudioPlayer::~DJAudioPlayer() {

};

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) 
{
    //formatManager.registerBasicFormats();
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
};

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) 
{
    //transportSource.getNextAudioBlock(bufferToFill);
    resampleSource.getNextAudioBlock(bufferToFill);
};

void DJAudioPlayer::releaseResources() 
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
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

void DJAudioPlayer::setGain(double gain) 
{
    if (gain < 0 || gain > 1.0) {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
};

void DJAudioPlayer::setSpeed(double ratio) 
{
    if (ratio < 0 || ratio > 100.0) {
        std::cout << "DJAudioPlayer::setGain ratio should be between 0 and 100" << std::endl;
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
    if (pos < 0 || pos > 1) {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
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
        std::cout << "Error, cannot divide by zero in DJAudioPlayer::getPositionRelative()" << std::endl;
        //DBG("Error in DJAudioPlayer::getPositionRelative(). transportSource.getLengthInSeconds() equals to zero ");
    }
    else {
        return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
    }

    /*if (transportSource.getTotalLength() == 0) {
       
        DBG("transportSource.getTotalLength() equals to zero ");
    }
    else {
        DBG("transportSource.getTotalLength() equals to " + transportSource.getTotalLength());
        return transportSource.getCurrentPosition() / transportSource.getTotalLength();
    }*/
    
}
