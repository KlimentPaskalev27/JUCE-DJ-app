/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 5 Mar 2022 5:12:45pm
    Author:  Alienware

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================]

/// <summary>
/// Constructor function for PlaylistComponent.
/// </summary>
/// <param name="_deckGUI1">DeckGUI object.</param>
/// <param name="_deckGUI2">Another DeckGUI object.</param>
/// <param name="_metadataParser">A DJAudioPlayer object to use to parse metadata of files.</param>
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
                                    DeckGUI* _deckGUI2,
                                    DJAudioPlayer* _metadataParser)
                                // the 2 decks and the DJAudioPlayer
                                : deckGUI1(_deckGUI1),
                                deckGUI2(_deckGUI2),
                                // an instance of the DJAudioPlayer to process metadata
                                metadataParser(_metadataParser)
{
    // search configuration
    addAndMakeVisible(searchBox);
    searchBox.setTextToShowWhenEmpty("Search for song name and Press Enter.", juce::Colours::cyan);
    searchBox.onReturnKey = [this] { searchPlaylist(searchBox.getText()); };

    //configure list of songs
    addAndMakeVisible(playlist);
    // setup table and load playlist from file
    playlist.getHeader().addColumn("Songs", 1, 1);
    playlist.getHeader().addColumn("Playtime", 2, 1);
    //add column to hold the close button
    playlist.getHeader().addColumn("", 3, 1);
    playlist.setModel(this);
    //load playlist (might have saved songs from before)
    loadPlaylist();

    addAndMakeVisible(importButton);
    importButton.addListener(this);

    addAndMakeVisible(decksLabel);
    decksLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    decksLabel.setText("Add song to left or right deck", juce::dontSendNotification);
    decksLabel.setColour(juce::Label::textColourId, juce::Colours::lightcyan);
    decksLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(addToPlayer1Button);
    addAndMakeVisible(addToPlayer2Button);
    addToPlayer1Button.addListener(this);
    addToPlayer2Button.addListener(this);
}

PlaylistComponent::~PlaylistComponent()
{
    // save added songs in the playlist when closing the application
    savePlaylist();
}

/// <summary>
/// Paints colours, fonts, background. Evrything that can be done with the Graphics class.
/// </summary>
/// <param name="g"></param>
void PlaylistComponent::paint (juce::Graphics& g)
{
    importButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::grey);
    addToPlayer1Button.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkcyan);
    addToPlayer2Button.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkcyan);
}

/// <summary>
/// Sets the bounds and sizes of each component inside PlaylistComponent.
/// </summary>
void PlaylistComponent::resized()
{
    double rowH = getHeight() / 20;
    searchBox.setBounds(0,  0 , getWidth(),  2 * rowH) ;

    playlist.setBounds(0, 2 * rowH, getWidth(), 13 * rowH);
    playlist.getHeader().setColumnWidth(1, 5 * getWidth() / 10);
    playlist.getHeader().setColumnWidth(2, 3 * getWidth() / 10);
    playlist.getHeader().setColumnWidth(3, 2 * getWidth() / 10);
    
    importButton.setBounds(0, 15 * rowH, getWidth(), 2 * rowH);
    decksLabel.setBounds(0, 17 * rowH, getWidth() , rowH);
    addToPlayer1Button.setBounds(0, 18 * rowH, getWidth() / 2, 2 * rowH) ;
    addToPlayer2Button.setBounds(getWidth() / 2, 18 * rowH, getWidth() / 2, 2 * rowH) ;
}

/// <summary>
/// Returns an integer, the number of objects of type Song in the songs vector.
/// </summary>
/// <returns>int number of rows</returns>
int PlaylistComponent::getNumRows()
{
    return songs.size();
}

/// <summary>
/// Used to set colours for the TableListBox that holds the songs info.
/// </summary>
void PlaylistComponent::paintRowBackground(juce::Graphics & g,
                        int rowNumber,
                        int width,
                        int height,
                        bool rowIsSelected) 
{
    if (rowIsSelected)
    {
        g.setColour(juce::Colours::white);
        g.fillAll(juce::Colours::darkcyan);
    }
    else {
        g.fillAll(juce::Colours::lightcyan);
    }
}

/// <summary>
/// Fills each cell in the TableListBox with content and styles it.
/// </summary>
void PlaylistComponent::paintCell(juce::Graphics & g,
                int rowNumber,
                int columnId,
                int width,
                int height,
                bool rowIsSelected)
{
    if (rowNumber < getNumRows())
    {
        if (columnId == 1)
        {
            g.drawText(songs[rowNumber].songname,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centredLeft,
                true
            );
        }
        if (columnId == 2)
        {
            g.drawText(songs[rowNumber].playtime,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centred,
                true
            );
        }
    }
}

/// <summary>
/// Reruns everytime content is added or removed to TableListBox and refreshes the cells' content.
/// </summary>
juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                            int columnId,
                                                            bool isRowSelected,
                                                            Component* existingComponentToUpdate)
{
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{ "X" };
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

/// <summary>
/// Checks which Button is clicked and fires different functions.
/// </summary>
/// <param name="button"></param>
void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &importButton)
    {
        DBG("Load button clicked");
        importToPlaylist();
        playlist.updateContent();
    }
    else if (button == &addToPlayer1Button)
    {
        DBG("Add to Player 1 clicked");
        loadInPlayer(deckGUI1);
    }
    else if (button == &addToPlayer2Button)
    {
        DBG("Add to Player 2 clicked");
        loadInPlayer(deckGUI2);
    }
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
        DBG(songs[id].songname + " removed from Playlist");
        deleteFromPlaylist(id);
        playlist.updateContent();
    }
}

/// <summary>
/// Creates a .csv file to hold all the songs we've added to the playlist. Is ran on app closing.
/// </summary>
void PlaylistComponent::savePlaylist()
{
    // create .csv to save playlist
    std::ofstream myPlaylist("playlist.csv");

    // save playlist to file
    for (Song& t : songs)
    {
        myPlaylist << t.file.getFullPathName() << "," << t.playtime << "\n";
    }
}

/// <summary>
/// Checks if the .csv with a saved playlist of songs exists. If it does, read it and add those files as a Song object and add to the songs vector which is the playlist.
/// </summary>
void PlaylistComponent::loadPlaylist()
{
    // create input stream from saved playlist
    std::ifstream myPlaylist("playlist.csv");
    std::string filePath;
    std::string length;

    // Read data, line by line
    if (myPlaylist.is_open())
    {
        while (getline(myPlaylist, filePath, ',')) {
            juce::File file{ filePath };
            Song newTrack{ file };
            getline(myPlaylist, length);
            newTrack.playtime = length;
            songs.push_back(newTrack);
        }
    }
    myPlaylist.close();
}

/// <summary>
/// Pass the selected song from the songs vector to a DeckGUI object so it can load it to be played.
/// </summary>
/// <param name="DeckGUI* deckGUI"></param>
void PlaylistComponent::loadInPlayer(DeckGUI* deckGUI)
{
    int selectedRow{ playlist.getSelectedRow() };
    if (selectedRow != -1)
    {
        DBG("Adding: " << songs[selectedRow].songname << " to Player");
        deckGUI->loadFile(songs[selectedRow].URL);
    }
    else
    {
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
            "Add to Deck Information:",
            "Please select a track to add to deck",
            "OK",
            nullptr
        );
    }
}

/// <summary>
/// Gives access to user to browse for a file from their PC and load that in as a Song object and add to the songs vector (the playlist).
/// </summary>
void PlaylistComponent::importToPlaylist()
{
    DBG("PlaylistComponent::importToPlaylist called");

    //initialize file chooser
    juce::FileChooser chooser{ "Select files" };
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (const juce::File& file : chooser.getResults())
        {
            juce::String fileNameWithoutExtension{ file.getFileNameWithoutExtension() };
            if (!isInPlaylist(fileNameWithoutExtension)) // if not already loaded
            {
                Song newTrack{ file };
                juce::URL audioURL{ file };
                newTrack.playtime = getLength(audioURL);
                songs.push_back(newTrack);
                DBG("loaded file: " << newTrack.songname);
            }
            else // display info message
            {
                juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon,
                    "Load information:",
                    fileNameWithoutExtension + " already loaded",
                    "OK",
                    nullptr
                );
            }
        }
    }
}

/// <summary>
/// Checks if a user-specified string is a valid name of a song that is currently added to the songs vector. 
/// </summary>
/// <param name="juce::String fileNameWithoutExtension">Takes in a string from the user - song name.</param>
/// <returns>Bool: True or False</returns>
bool PlaylistComponent::isInPlaylist(juce::String fileNameWithoutExtension)
{
    return (std::find(songs.begin(), songs.end(), fileNameWithoutExtension) != songs.end());
}

/// <summary>
/// Removes a Song from the songs vector by its id.
/// </summary>
/// <param name="id">integer id of a Song.</param>
void PlaylistComponent::deleteFromPlaylist(int id)
{
    songs.erase(songs.begin() + id);
}

/// <summary>
/// Loads the audio file from URL, parses it for how many seconds it is long, converts to minutes and returns.
/// </summary>
/// <param name="juce::URL audioURL">Takes in a URL for an audio file.</param>
/// <returns>Returns a string which represents the length of a song.</returns>
juce::String PlaylistComponent::getLength(juce::URL audioURL)
{
    metadataParser->loadURL(audioURL);
    double seconds{ metadataParser->getLengthInSeconds() };
    juce::String minutes{ secondsToMinutes(seconds) };
    return minutes;
}

/// <summary>
/// Converts seconds to minutes. Length of a song.
/// </summary>
/// <param name="double seconds"></param>
/// <returns></returns>
juce::String PlaylistComponent::secondsToMinutes(double seconds)
{
    //find seconds and minutes and make into string
    int secondsRounded{ int(std::round(seconds)) };
    juce::String min{ std::to_string(secondsRounded / 60) };
    juce::String sec{ std::to_string(secondsRounded % 60) };

    if (sec.length() < 2) // if seconds is 1 digit or less
    {
        //add '0' to seconds until seconds is length 2
        sec = sec.paddedLeft('0', 2);
    }
    return juce::String{ min + ":" + sec };
}

/// <summary>
/// Searches for an existing Song in the playlist to match a name input.
/// </summary>
/// <param name="juce::String query">Input name of song by user. String.</param>
void PlaylistComponent::searchPlaylist(juce::String query)
{
    DBG("Searching playlist for: " << query);
    if (query != "")
    {
        int rowNumber = whereInPlaylist(query);
        playlist.selectRow(rowNumber);
    }
    else
    {
        playlist.deselectAllRows();
    }
}

/// <summary>
/// Finds the song by its name from the search and retrieves that song's ID. ID is then used for removal of song or other functions.
/// </summary>
/// <param name="juce::String query">String- name of song.</param>
/// <returns>Returns an integer id.</returns>
int PlaylistComponent::whereInPlaylist(juce::String query)
{
    // finds index where track title contains searchText
    auto it = find_if(songs.begin(), songs.end(),
        [&query](const Song& obj) {return obj.songname.contains(query); });
    int i = -1;

    if (it != songs.end())
    {
        i = std::distance(songs.begin(), it);
    }

    return i;
}