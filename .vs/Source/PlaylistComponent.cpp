/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 5 Mar 2022 5:12:45pm
    Author:  Alienware

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    //put some dummy data in the Track titles vector 
    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");
    trackTitles.push_back("Track 5");
    trackTitles.push_back("Track 6");

    //configure the table
    tableComponent.getHeader().addColumn("Track title", 1, 400);  //name of column, column id, and width
    //example second column      tableComponent.getHeader().addColumn("Artist", 1, 400);
    tableComponent.setModel(this);

    tableComponent.getHeader().addColumn("", 2, 400);  //name of column, column id, and width

    addAndMakeVisible(tableComponent);

}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    tableComponent.setBounds(0, 0, getWidth(), getHeight());

}
int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics & g,
                        int rowNumber,
                        int width,
                        int height,
                        bool rowIsSelected) 
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else {
        g.fillAll(juce::Colours::darkgrey);
    }

}

void PlaylistComponent::paintCell(juce::Graphics & g,
                int rowNumber,
                int columnId,
                int width,
                int height,
                bool rowIsSelected)
{
    g.drawText (trackTitles[rowNumber],
        2,
        0,
        width - 4,
        height,
        juce::Justification::centredLeft,
        true);
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                        int columnId,
                                                        bool isRowSelected,
                                                        Component* existingComponentToUpdate)
{
    //create a play button for each second column cell in each row
    // if the column is with ID 1
    if (columnId == 2)
    {
        //if the component has not been created
        if (existingComponentToUpdate == nullptr) 
        {
            // create an text button pointer to a constructor of a button
            juce::TextButton* btn = new juce::TextButton{ "Play" };

            //get an id of type juce string in order to later use
            juce::String id{ std::to_string(rowNumber) };
            //give that string to setComonentID for each btn
            btn->setComponentID(id);

            //add listener
            btn->addListener(this);
            // create a new component button
            existingComponentToUpdate = btn;

            //= = = = = Why we had to set an ID to the TextButton?  = = = = =
            // - Because we have no data member referring to the button, so we cannot use the method we've seen before to compare 
            // addresses between the pointer passed to the event listener and a data member.
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    // convert juce string back to standard string because we have a vector of std string for each row for the trackTitles IDs
    int id = std::stoi(button->getComponentID().toStdString());
    // juce::string  -> std::string -> int

    //get the track name on button click in console
    std::cout << "PlaylistComponent::buttonClicked " << trackTitles[id] << std::endl;

    //for output messages in VS code Immediate window terminal
    //Juce debug messages alternative to std::cout
    DBG("PlaylistComponent::buttonClicked " + trackTitles[id]);
}