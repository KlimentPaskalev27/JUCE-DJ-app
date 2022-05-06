/*
  ==============================================================================

    DeckGUI.cpp
    Created: 5 Mar 2022 12:22:12pm
    Author:  Alienware

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(int _id, 
                DJAudioPlayer* _player,
                juce::AudioFormatManager & formatManagerToUse,
                juce::AudioThumbnailCache & cacheToUse 
            ) : id(_id),
            player(_player),
            waveformDisplay(formatManagerToUse, cacheToUse)


{
    // In your constructor, you should add any child components, and
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(axisModificator1);
    addAndMakeVisible(axisModificator2);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(posLabel);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);


    playButton = new juce::DrawableButton("playButton", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
    playButtonImage = juce::Drawable::createFromImageData(BinaryData::play_svg, BinaryData::play_svgSize);
    pauseButtonImage = juce::Drawable::createFromImageData(BinaryData::pause_svg, BinaryData::pause_svgSize);
    playButton->setImages(playButtonImage.get(), playButtonImage.get(), playButtonImage.get(), nullptr,
        pauseButtonImage.get(), pauseButtonImage.get(), pauseButtonImage.get(), nullptr
    );
    addAndMakeVisible(playButton);
    playButton->addListener(this);


    forwardButton = new juce::DrawableButton("forwardButton", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
    forwardButtonImage = juce::Drawable::createFromImageData(BinaryData::forward_svg, BinaryData::forward_svgSize);
    forwardButton->setImages(forwardButtonImage.get(), forwardButtonImage.get(), forwardButtonImage.get(), nullptr,
        forwardButtonImage.get(), forwardButtonImage.get(), forwardButtonImage.get(), nullptr
    );
    addAndMakeVisible(forwardButton);
    forwardButton->addListener(this);


    reverseButton = new juce::DrawableButton("reverseButton", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
    reverseButtonImage = juce::Drawable::createFromImageData(BinaryData::reverse_svg, BinaryData::reverse_svgSize);
    reverseButton->setImages(reverseButtonImage.get(), reverseButtonImage.get(), reverseButtonImage.get(), nullptr,
        reverseButtonImage.get(), reverseButtonImage.get(), reverseButtonImage.get(), nullptr
    );
    addAndMakeVisible(reverseButton);
    reverseButton->addListener(this);


    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    axisSlider.addListener(this);
    axisModificator1.addEventListener(this);
    axisModificator2.addEventListener(this);

    // set range, default values, labels and decimal point precision for sliders
    double vol = 0.5;
    volSlider.setValue(vol);
    volSlider.setRange(0.0, 1.0);
    volSlider.setNumDecimalPlacesToDisplay(2);
    volSlider.setTextValueSuffix(" Vol.");
    volLabel.attachToComponent(&volSlider, true);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 80, 20);
    //make volume slider rotary
    volSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    volSlider.setTooltip("Drag slider to control volume");

    double speed = 1.0;
    speedSlider.setValue(speed);
    speedSlider.setRange(0.0, 3.0);
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setTextValueSuffix(" Rate");
    speedLabel.attachToComponent(&speedSlider, true);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 80, 20);
    //make speed slider vertical
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    speedSlider.setTooltip("Drag slider to control the playrate speed of the song");

    posSlider.setRange(0.0, 1.0);
    posSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    posSlider.setSliderStyle(juce::Slider::LinearBar);
    posLabel.setText("Playback Control", juce::dontSendNotification);
    posSlider.setTooltip("Drag slider to control where the song should play from");
    

    //configure the slider for AxisModificator
    axisSlider.setRange(0.0, 1.0);
    axisSlider.setNumDecimalPlacesToDisplay(2);
    axisModificator1.setTooltip("x: damping\ny: room size");
    axisModificator2.setTooltip("x: dry level\ny: wet level");

    //parameter is how often to call the timer function in miliseconds
    startTimer(300);
}

DeckGUI::~DeckGUI() 
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    sliderLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);

    volSlider.setLookAndFeel(&sliderLookAndFeel);
    posSlider.setLookAndFeel(&playbackBarLAF);
    speedSlider.setLookAndFeel(&sliderLookAndFeel);

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.setColour (juce::Colours::darkcyan);
    g.setFont (14.0f);
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 20;
    posLabel.setBounds(0, 0, getWidth(), rowH);
    posSlider.setBounds(0,  rowH, getWidth(), rowH);
    waveformDisplay.setBounds(0, 2* rowH, getWidth(), 4 * rowH );
    
    axisModificator1.setBounds(0, 6 * rowH , getWidth() / 2, 5 * rowH );
    axisModificator2.setBounds(getWidth() / 2, 6 * rowH , getWidth() / 2, 5 * rowH );
    volSlider.setBounds(0, 11 * rowH ,   2 * getWidth() / 3, 5 * rowH );
    speedSlider.setBounds(2* getWidth() / 3, 11 * rowH , getWidth() / 3, 5 * rowH );

    reverseButton->setBounds(0, 16 * rowH, getWidth() / 4, 4 * rowH);
    playButton->setBounds(getWidth() / 4, 16 * rowH, 2 * getWidth() / 4, 4 * rowH);
    
    forwardButton->setBounds(3 * getWidth() / 4, 16 * rowH, getWidth() / 4, 4 * rowH);
}



/// <summary>
/// Checks which button is pressed and runs different functions.
/// </summary>
/// <param name="button">juce::Button* button</param>
void DeckGUI::buttonClicked( juce::Button* button)
{
    if (button == playButton )
    {
        isOn = !isOn;
        if (isOn)
        {
            player->start();
        }
        else if (!isOn) {
            player->stop();
        }

        playButton->setToggleState(isOn, juce::NotificationType::dontSendNotification);
        repaint();
    }

    else if (button == forwardButton)
    {
        std::cout << "Forward button was clicked " << std::endl;
        if (player->getPositionRelative() < 0.95)
        {
            player->setPositionRelative(player->getPositionRelative() + 0.05);
        }
        DBG("player->getPositionRelative() is " << player->getPositionRelative());
    }
    else if (button == reverseButton)
    {
        std::cout << "Reverse button was clicked " << std::endl;
        //Only allow if song has been playing long ehough
        if (player->getPositionRelative() > 0.05)
        {
            player->setPositionRelative(player->getPositionRelative() - 0.05);
        }
        DBG("player->getPositionRelative() is " << player->getPositionRelative());
    }
}


void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray &files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if (files.size() == 1)
    {
        player->loadURL(juce::URL{ juce::File{ files[0] } });
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
    posSlider.setValue(player->getPositionRelative());
}

void DeckGUI::loadFile(juce::URL audioURL)
{
    DBG("DeckGUI::loadFile called");
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);

}

/// <summary>
/// If the AxisModificator box is interacted with, check which box it is, then pass new x,y coordinates to the player.
/// </summary>
/// <param name="axisModificator"></param>
void DeckGUI::axisModificatorValueChange(AxisModificator* axisModificator)
{
    if (axisModificator == &axisModificator1)
    {
        DBG("Deck " << id << ": axisModificator1 was clicked");
        player->setRoomSize(axisModificator->getY());
        player->setDamping(axisModificator->getX());
    }
    if (axisModificator == &axisModificator2)
    {
        DBG("Deck " << id << ": axisModificator2 was clicked");
        player->setWetLevel(axisModificator->getY());
        player->setDryLevel(axisModificator->getX());
    }
}
