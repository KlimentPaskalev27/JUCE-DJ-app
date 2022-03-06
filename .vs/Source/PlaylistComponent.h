/*
  ==============================================================================

    PlaylistComponent.h
    Created: 5 Mar 2022 5:12:45pm
    Author:  Alienware

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                            public juce::TableListBoxModel,
                            public juce::Button::Listener
{
    public:
        PlaylistComponent();
        ~PlaylistComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        // TableListBoxModel virtual functions - pure minimum to inherit the class is to override those functions
        int getNumRows() override;
        void paintRowBackground(juce::Graphics &,
                                int rowNumber,
                                int width,
                                int height,
                                bool rowIsSelected) override;

        void paintCell(juce::Graphics &,
                        int rowNumber,
                        int columnId,
                        int width,
                        int height,
                        bool rowIsSelected) override;

        juce::Component* refreshComponentForCell(int rowNumber,
                                                int columnId,
                                                bool isRowSelected,
                                                Component* existingComponentToUpdate
                                            );

        void buttonClicked(juce::Button* button) override;


    private:

        juce::TableListBox tableComponent;

        std::vector<std::string> trackTitles;

        

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
