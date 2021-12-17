========================================================================
    CONSOLE APPLICATION : OpenGL3DTemplate Project Overview
========================================================================

AppWizard has created this OpenGL3DTemplate application for you.

This file contains a summary of what you will find in each of the files that
make up your OpenGL3DTemplate application.


OpenGL3DTemplate.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

OpenGL3DTemplate.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

OpenGL3DTemplate.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named OpenGL3DTemplate.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

# Game description:

The scene has different 3D objects and a character collecting some presents. The goal is to collect the presents before the time ends.
Each has an animation key and the game has 3 different views: front, side and top view. 
<br/>
Instructions: 
<br/>
- Left arrow: moves the player to the left. 
- Right arrow: moves the player to the left. 
- Front arrow: moves the player to the front. 
- Back arrow: move sthe player to the back. <br/>
The player always faces the direction of its movement. 
<br/>
Animation keys: 
- 'f': moves the fence up and down.
- 't': makes the tree revolve around themselves 
- 'l': makes the lanterns rotate around the scene. 
- 'o': makes the snowman translate to the left and right back and forth.
- 'p': moves the presents up. 
<br/>
Once you reselect the key, the animation stops. 
<br/>
A sound is associated with each animation and once the player succeeds/ fails a different sound is run. <br/>
Happy Game!
<br/>
Here are some screen shots: 

