/*
  ==============================================================================

    AxisModificator.cpp
    Created: 8 Mar 2022 9:22:00pm
    Author:  Alienware

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AxisModificator.h"
#include <iomanip>
#include <sstream>

//==============================================================================

/// <summary>
/// In the contructor, initialise the grid lines, the range and the coordinates.
/// </summary>
AxisModificator::AxisModificator()
{
    createGrid();
    setRange();
    initCoordinates(50.0f, 50.0f);
}

AxisModificator::~AxisModificator() {}
AxisModificator::EventListener::EventListener() {}
AxisModificator::EventListener::~EventListener() {}

/// <summary>
/// Styles components.
/// </summary>
/// <param name="g"></param>
void AxisModificator::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background
    g.setColour(juce::Colours::darkcyan);
    drawPlot(g);

    g.setColour(juce::Colours::cyan);
    drawMarker(g);
    if (markerRelocated)
    { 
        drawMarkerValueText(g);
    }

    // capture original range for reference when resizing
    setOriginalRange();
}

/// <summary>
/// Sets bounds of components.
/// </summary>
void AxisModificator::resized()
{
    initProporties();
    updateCoords();
}

/// <summary>
/// Detect whenever user presses down mouse inside the AxisModificator field box.
/// </summary>
/// <param name="event">Take in MouseEvent as parameter</param>
void AxisModificator::mouseDown(const juce::MouseEvent& event)
{
    DBG("Mouse pressed at: x = " << getX() << ", and at y = " << getY());
    markerRelocated = true;

    //Since user is inside the box, remove cursor so that user moves the cross in the field.
    setMouseCursor(juce::MouseCursor::NoCursor);

    //change the new coordinates by taking in the mouse cursor coordinates inside the box
    setCoordinates(float(event.getMouseDownX()), float(event.getMouseDownY()));

    listenerCallback();
    repaint();
}

/// <summary>
/// Whenever user moves cursor while pressing down mouse, change the coordinate values.
/// </summary>
/// <param name="event">Takes in a MouseEvent as parameter.</param>
void AxisModificator::mouseDrag(const juce::MouseEvent& event)
{
    DBG("Mouse moved to: x = " << getX() << ", and y = " << getY());
    //get position from event
    juce::Point<int> origPos(event.getPosition());

    //update coordinates
    float origX = float(origPos.getX());
    float origY = float(origPos.getY());
    setCoordinates(origX, origY);

    listenerCallback();
    repaint();
}

/// <summary>
/// When user no longer presses mouse down inside AxisModificator field, make the cursor visible again.
/// </summary>
/// <param name="event"></param>
void AxisModificator::mouseUp(const juce::MouseEvent& event)
{
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

/// <summary>
/// Call event listener and run axisModificatorValueChange() on itself to recognise changes in values.
/// </summary>
void AxisModificator::listenerCallback()
{
    eventlisteners.call(
        [this](EventListener& listener) 
        { 
            listener.axisModificatorValueChange(this); 
        }
    );
}

/// <summary>
/// Add an event listener.
/// </summary>
/// <param name="listener"></param>
void AxisModificator::addEventListener(EventListener* listener) 
{ 
    eventlisteners.add(listener);
}

/// <summary>
/// Remove event listener.
/// </summary>
/// <param name="listener"></param>
void AxisModificator::removeEventListener(EventListener* listener)
{ 
    eventlisteners.remove(listener);
}

/// <summary>
/// Retrieve the X axis coordinates.
/// </summary>
/// <returns>Returns a float.</returns>
float AxisModificator::getX() 
{ 
    return constrain(coordinates['x']); 
}

/// <summary>
/// Returns the Y axis coordinates.
/// </summary>
/// <returns>Returns a float.</returns>
float AxisModificator::getY()
{
    return invertCoordinates(constrain(coordinates['y']), range['min'], range['max']);
}

/// <summary>
/// Sets the grid lines inside of the AxisModificator box. Checks for odd numbers by using modulo 2 and removes 1 line if odd number is input.
/// In order to get symmetrical lines on each side of each axis.
/// </summary>
/// <param name="lineCount">Takes in an integer which represents how many lines should be in grid.</param>
void AxisModificator::createGrid(int lineCount)
{
    //make sure the lines are symmetrical on each axis side so use modulo 2 to remove any odd number grid lines
    if ( lineCount % 2 != 0 && lineCount % 2 == 1)
    { 
        lineCount = lineCount - 1;
    }
    gridLines = lineCount;
}

/// <summary>
/// Determines the upper and lower bounds of the values that can be returned in range.
/// </summary>
/// <param name="min">Float</param>
/// <param name="max">Float</param>
void AxisModificator::setRange(float min, float max)
{
    range['min'] = min;
    range['max'] = max;
}

/// <summary>
/// Used to set the initial coordinates of the cross (cursor) inside the AxisModificator grid box.
/// </summary>
/// <param name="origX">float</param>
/// <param name="origY">float</param>
void AxisModificator::initCoordinates(float origX, float origY)
{
    coordinates['x'] = origX;
    coordinates['y'] = origY;
}

/// <summary>
/// Used to set the x,y coordinates inside the grid.
/// </summary>
/// <param name="origX"></param>
/// <param name="origY"></param>
void AxisModificator::setCoordinates(float origX, float origY)
{
    if (checkIfInRawRange(origX, origY) )
    { 
        coordinates['x'] = origX;
        coordinates['y'] = origY;
    }
}

/// <summary>
/// Updates the coordinates from the original min and original max ones for each axis and calculates the new coordinates.
/// The new coordinates are then passed as parameters when setCoords() is called inside this function after calculus.
/// </summary>
void AxisModificator::updateCoords()
{
    //get ratios based off initial range
    double xRatio = double(coordinates['x'] / (originalRange['max'] - originalRange['min']));
    double yRatio = double(coordinates['y'] / (originalRange['max'] - originalRange['min']));

    // new x and y based off current size and previous ratio
    float newX = float(right * xRatio);
    float newY = float(bottom * yRatio);
    //set new coordinates
    setCoordinates(newX, newY);
}

/// <summary>
/// Sets the original coordinates.
/// </summary>
void AxisModificator::setOriginalRange()
{
    originalRange['min'] = getLocalBounds().getX();
    originalRange['max'] = getLocalBounds().getWidth();
}

/// <summary>
/// Illustrates the grid box and draws the axis.
/// </summary>
/// <param name="g"></param>
void AxisModificator::drawPlot(juce::Graphics& g)
{
    //draw grid
    g.setColour(juce::Colours::darkcyan);
    drawGrid(g);

    //draw axis
    g.setColour(juce::Colours::darkorange);
    drawAxis(g);

    //draw component outline
    g.setColour(juce::Colours::darkcyan);
    g.drawRect(getLocalBounds(), 5);
}

/// <summary>
/// Draws each line of the axis by using x,y coordinates to eastablish values.
/// </summary>
/// <param name="g"></param>
void AxisModificator::drawAxis(juce::Graphics& g)
{
    //draw x and y axis
    g.drawLine(left, middleY, right, middleY, 2);
    g.drawLine(middleX, left, middleX, bottom, 2);
}

/// <summary>
/// Draws the grid.
/// </summary>
/// <param name="g"></param>
void AxisModificator::drawGrid(juce::Graphics& g)
{
    // set up the length of dashes used for non-axis lines
    float dashLength[] = { 3, //the dash length 
                           5 // the space between dashes
                        };

    // below gridLines has +2 because we make up for grid lines at edge of box that are not visible
    float offset = float(getLocalBounds().getWidth() / (gridLines + 2));

    //for each grid line draw dashed lines  using coordinates
    for (int i = 0; i < gridLines; ++i)
    {
        // draw lines on the left and right side of Y
        g.drawDashedLine(juce::Line<float>(middleX - offset * i, top, middleX - offset * i, bottom),
            &dashLength[0], 2, 1.0, 0);
        g.drawDashedLine(juce::Line<float>(middleX + offset * i, top, middleX + offset * i, bottom),
            &dashLength[0], 2, 1.0, 0);

        // draw lines on the bottom and top sides of X
        g.drawDashedLine(juce::Line<float>(left, middleY - offset * i, right, middleY - offset * i),
            &dashLength[0], 2, 1.0, 0);
        g.drawDashedLine(juce::Line<float>(left, middleY + offset * i, right, middleY + offset * i),
            &dashLength[0], 2, 1.0, 0);
    }
}

/// <summary>
/// Draws the cross marker on the Axis box.
/// </summary>
/// <param name="g"></param>
void AxisModificator::drawMarker(juce::Graphics& g)
{
    // set the size of the cross
    float length = float(getLocalBounds().getWidth() / 20);

    // draw the lines of the cross marker using the x,y coordinates
    juce::Line<float> lineH(juce::Point<float>(coordinates['x'] - length, coordinates['y']),
        juce::Point<float>(coordinates['x'] + length, coordinates['y']));
    juce::Line<float> lineV(juce::Point<float>(coordinates['x'], coordinates['y'] - length),
        juce::Point<float>(coordinates['x'], coordinates['y'] + length));

    // set the thicknes of the cross marker lines
    g.drawLine(lineH, 1.2f);
    g.drawLine(lineV, 1.2f);
}

/// <summary>
/// Show the text with the X,Y values currently the cross marker is over.
/// </summary>
/// <param name="g"></param>
void AxisModificator::drawMarkerValueText(juce::Graphics& g)
{
    g.setFont(float(getWidth() / 10));
    //set where does the Y value text appears in terms of height inside the component
    int textHeight = int(g.getCurrentFont().getHeight() * 2);

    // Draw Y
    std::stringstream yStream;
    yStream << std::fixed << std::setprecision(2) << getY();
    g.drawText(yStream.str(), int(middleX), int(top), int(middleX), textHeight,
        juce::Justification::centredLeft, true);

    // Draw X
    std::stringstream xStream;
    xStream << std::fixed << std::setprecision(2) << getX();
    g.drawText(xStream.str(), int(middleX), int(middleY), int(middleX), textHeight,
        juce::Justification::centredRight, true);
}

/// <summary>
/// Sets the variables used for determining positions of cursor, grid, lines etc.
/// </summary>
void AxisModificator::initProporties()
{
    // recalculate all the settings
    //based on components borders
    top = float(getLocalBounds().getY());
    right = float(getLocalBounds().getRight());
    bottom = float(getLocalBounds().getBottom());
    left = float(getLocalBounds().getX());
    // based on axis
    middleY = float(getLocalBounds().getCentreY());
    middleX = float(getLocalBounds().getCentreX()); 
}

/// <summary>
/// Constrains the coordinates based on new and old range values.
/// </summary>
/// <param name="coordinates">Takes in a float parameter of coordinates.</param>
/// <returns>Returns a float.</returns>
float AxisModificator::constrain(float coordinates)
{
    float oldMinRange = float(getLocalBounds().getX());
    float oldMaxRange = float(getLocalBounds().getWidth());
    float oldRange = oldMaxRange - oldMinRange;
    float newRange = range['max'] - range['min'];

    float newValue = (((coordinates - oldMinRange) * newRange) / oldRange) + range['min'];
    return newValue;
}

/// <summary>
/// Inverts coord within a range between min and max.
/// </summary>
/// <param name="coord"></param>
/// <param name="min"></param>
/// <param name="max"></param>
/// <returns></returns>
float AxisModificator::invertCoordinates(float coordinates, float min, float max)
{
    return (min + max) - coordinates;
}

/// <summary>
/// Checks if the original coordinates are within the legal boundaries.
/// </summary>
/// <param name="origX"></param>
/// <param name="origY"></param>
/// <returns>Returns a True or Fales based on check.</returns>
bool AxisModificator::checkIfInRawRange(float origX, float origY)
{
    return (
        origX >= left &&
        origX <= right &&
        origY >= top &&
        origY <= bottom);
}