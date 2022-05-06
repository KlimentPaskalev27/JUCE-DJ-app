/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 5 Mar 2022 10:42:26am
    Author:  Alienware

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource {
    public:
        DJAudioPlayer(juce::AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        void loadURL(juce::URL audioURL);
        void setGain(double gain);
        void setSpeed(double ratio);
        void setPosition(double posInSecs);
        void setPositionRelative(double pos); // so that the positions is not absolute from 1-10 seconds, but relative, 1-10 ,moves from begining to end

        void start();
        void stop();

        /** Get the relative positions of the playhead*/
        double getPositionRelative();

        double getLengthInSeconds();

        /**Sets the amount of reverb*/
        void setRoomSize(float size);
        /**Sets the amount of reverb*/
        void setDamping(float dampingAmt);
        /**Sets the amount of reverb*/
        void setWetLevel(float wetLevel);
        /**Sets the amount of reverb*/
        void setDryLevel(float dryLevel);

    private:
        juce::AudioFormatManager& formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        juce::AudioTransportSource transportSource;
        juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

        juce::ReverbAudioSource reverbSource{ &resampleSource, false };
        juce::Reverb::Parameters reverbParameters;
};