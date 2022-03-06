#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { if (granted) setAudioChannels ( 2 , 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);  //zero inputs (no mic), 2 outputs left and right channel;s
    }


    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);


    formatManager.registerBasicFormats(); //call the format manager, otherwise app wont know formats mp3... etc wont work
    // ie this is calling the registerBasicFormats 
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

/// <summary>
/// Play sound and load buffer when using audio file
/// </summary>
/// <param name="bufferToFill"></param>
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    //tell it to fill its buffer out on its own
    //transportSource.getNextAudioBlock(bufferToFill);
    //resampleSource.getNextAudioBlock(bufferToFill);

    //player1.getNextAudioBlock(bufferToFill);

    mixerSource.getNextAudioBlock(bufferToFill);
}

//below is when using sound synthesis vie code
//void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
//{
//    //I want this channel and I want to start writing this sample
//    auto* leftChan = bufferToFill.buffer->getWritePointer(0, //channel number
//                                                            bufferToFill.startSample);
//
//    //do same for right channel
//    auto* rightChan = bufferToFill.buffer->getWritePointer(1, //channel number
//    bufferToFill.startSample);
//
//    //fill it with samples         .numSamples check the numbers the buffer has to fill
//    for (auto i = 0; i < bufferToFill.numSamples; ++i) //++i increments and assigns, whereas i++ makes a copy, increments and assigns. ++i is better
//    {
//        //double sample = rand.nextDouble() * 0.25; //generates a random number. *0.25 is to scale down number changes like from -1 to 1 which will be loud
//        //the above generates whtie noise
//
//        //now to use the phase var
//        //double sample = fmod(phase, 0.2); //fmod is a modulo operation, gets the remainedr of phase. This is to get high to low pitches 
//        //so when phase reaches 0.2, it's going to become zero again. Like wave with drops /|
//
//        //lets make it a bit gentler on the ears
//        double sample = sin(phase) * 0.1; //gets a sine tone
//
//        leftChan[i] = sample;
//        rightChan[i] = sample;
//
//        //phase += 0.05;
//        //results in very high pitch sound. High pitch usually means it's cycling through the wave too fast. 
//        //so maybe reduce the increment
//        //phase += 0.005; 
//        // now let's attach the phase var to a slider
//        phase += dphase;
//
//    }
//
//
//    //this below fills buffer with zeros - silence
//    //bufferToFill.clearActiveBufferRegion();
//}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    //transportSource.releaseResources();

    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void MainComponent::resized()
{


    deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() /2 );
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() /2 );

    playlistComponent.setBounds(0, getHeight() / 2 , getWidth() , getHeight() / 2 );
}
