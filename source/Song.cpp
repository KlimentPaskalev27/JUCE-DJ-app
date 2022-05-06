/*
  ==============================================================================

    Song.cpp
    Created: 8 Mar 2022 12:07:26pm
    Author:  Alienware

  ==============================================================================
*/


#include "Song.h"
#include <filesystem>

/// <summary>
/// Constructs a Song object which can have a name and a playtime duration.
/// </summary>
/// <param name="_file">Takes in a file as parameter.</param>
Song::Song(juce::File _file) : file(_file),
                            // initialise the song name by removing the file extension from the initial string
                            songname(_file.getFileNameWithoutExtension()),
                            URL(juce::URL{ _file })
{
    DBG("Added a song with name: " << songname);
}

/// <summary>
/// Compares Song objects by their name
/// </summary>
/// <param name="other">Takes in a string name of a song to compare.</param>
/// <returns>Returns True or Fales.</returns>
bool Song::operator==(const juce::String& other) const
{
    return songname  == other;
}