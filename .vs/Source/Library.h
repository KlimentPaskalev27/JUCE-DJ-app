/*
  ==============================================================================

    Library.h
    Created: 6 Mar 2022 1:09:38pm
    Author:  Alienware

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>

//==============================================================================
/*
*/
class Library  : public juce::Component
{
    public:
        Library();
        ~Library() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        void addTrack();

    private:

        //list of added tracks
        std::vector<juce::AudioThumbnail> trackCaches;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Library)
};
