///////////////////////////////////////////////////////////////////////////
//
// File: main.cc
// Author: Andrew Howard
// Date: 4 Dec 2000
// Desc: Program entry point when not using a GUI
//
// CVS info:
//  $Source: /home/tcollett/stagecvs/playerstage-cvs/code/stage/src/main.cc,v $
//  $Author: ahoward $
//  $Revision: 1.7 $
//
// Usage:
//  (empty)
//
// Theory of operation:
//  (empty)
//
// Known bugs:
//  (empty)
//
// Possible enhancements:
//  (empty)
//
///////////////////////////////////////////////////////////////////////////

#include "../VERSION"
#include "world.hh"
#include <unistd.h>
#include <signal.h>


///////////////////////////////////////////////////////////////////////////
// Static vars

// Name of world file
//
char *world_file;

// Quit signal
//
bool quit = false;


///////////////////////////////////////////////////////////////////////////
// Handle quit signals
//
void sig_quit(int)
{
    quit = true;
}



///////////////////////////////////////////////////////////////////////////
// Parse the command line
//
bool parse_cmdline(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: stage WORLDFILE\n");
        return false;
    }

    // Extract the name of the file describing the world
    //
    world_file = argv[1];
    return true;
}


///////////////////////////////////////////////////////////////////////////
// Program entry
//
int main(int argc, char **argv)
{
    printf("** Stage v%s **\n", (char*) VERSION);

    // Parse the command line
    //
    if (!parse_cmdline(argc, argv))
        return 1;
    
    // Create the world
    //
    CWorld *world = new CWorld;

    // Load the world
    //
    if (!world->Load(world_file))
        return 0;
    
    // Start the world
    //
    if (!world->Startup())
    {
        printf("aborting\n");
        return 1;
    }

    // Register callback for quit (^C,^\) events
    //
    signal(SIGINT, sig_quit);
    signal(SIGQUIT, sig_quit);
    signal(SIGHUP, sig_quit);
    
    // Wait for a signal
    //
    while (!quit)
        pause();

    // Stop the world
    //
    world->Shutdown();

    // Destroy the world
    //
    delete world;

    return 0;
}


