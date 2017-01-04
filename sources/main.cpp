#define ID_HELLOTEST 10233422
#define ID_MIDI_OBJECT 10233423

#include "c4d.h"
#include "c4d_memory_mp.h"
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include "RtMidi.h"
#include "c4d_symbols.h"
#include "MidiPlugin.h"
#include <thread>
#include <string>

#define DEFAULT_COLOR Vector(0.5)


class MidiPlugin : public ObjectData
{
public:
    virtual Bool Init (GeListNode* node);
//    virtual Bool Message (GeListNode* node, Int32 type, void* data);
    static NodeData* Alloc(void) { return NewObjClear(MidiPlugin); }
    
};

void myCallback( double deltatime, std::vector< unsigned char > *message, void * node ) {
    BaseObject *op = (BaseObject *) node;
    BaseContainer* data = op->GetDataInstance();
    
    // GePrint(String::IntToString((int)message->at(1)));
    data->SetInt32(MIDIPLUGIN_NOTE, (int)message->at(1));
    data->SetFloat(MIDIPLUGIN_NOTE_VALUE, (int)message->at(2));
    
    // DRAWFLAGS_NO_THREAD|
    // DrawViews( DRAWFLAGS_FORCEFULLREDRAW );
    
    //GePrint("ce urmeaza este valoarea citita din parametru");
    //GePrint(String::IntToString((int)data->GetInt32(MIDIPLUGIN_NOTE)));
    //op->Message(MSG_UPDATE);
    //SendCoreMessage(COREMSG_CINEMA, BaseContainer(COREMSG_CINEMA_FORCE_AM_UPDATE));
    
    EventAdd(EVENT_FORCEREDRAW);
    
    // op->SetDirty(DIRTYFLAGS_ALL);
    // std::cout << "get dirty: ";
    // std::cout << data->GetDirty();
    // std::cout << "\n";
    
    // unsigned int nBytes = message->size();
    //for ( unsigned int i=0; i<nBytes; i++ ) {
        // std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
    //}
    // if ( nBytes > 0 )
        // std::cout << "stamp = " << deltatime << std::endl;
}

Bool MidiPlugin::Init(GeListNode* node){
    BaseObject*		 op = (BaseObject*)node;
    BaseContainer* data = op->GetDataInstance();
    
    RtMidiIn  *midiin = new RtMidiIn();
    ((BaseContainer*)data)->SetInt32(MIDIPLUGIN_NOTE, 11);
    
    // Check available ports.
    unsigned int nPorts = midiin->getPortCount();
    if ( nPorts == 0 ) {
        std::cout << "No ports available!\n";
    }
    midiin->openPort( 0 );
    // Set our callback function.  This should be done immediately after
    // opening the port to avoid having incoming messages written to the
    // queue.
    midiin->setCallback( &myCallback, (void *) node);
    // Don't ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes( false, false, false );

    
    return true;
}

class HelloWorldTest : public CommandData {
public:
    virtual Bool Execute(BaseDocument *doc)
    {
        RtMidiIn  *midiin = 0;
        midiin = new RtMidiIn();
        
        std::string str = midiin->getPortName(0);
        char * cstr = new char [str.length()+1];
        std::strcpy (cstr, str.c_str());
        
        String msg = cstr;
        MessageDialog(msg);
        
        
        
        GePrint("This is my first plugin");
        return TRUE;
    }
};

#define ID_MIDIPLUGIN   1000001

Bool PluginStart(void)
{
    RegisterObjectPlugin(IDS_MIDIPLUGIN,
                         GeLoadString(IDS_MIDIPLUGIN),
                         OBJECT_GENERATOR,
                         MidiPlugin::Alloc,
                         "MidiPlugin",
                         AutoBitmap("midi.tif"),
                         0
                         );
    
    //RegisterCommandPlugin(IDS_MIDIPLUGIN, GeLoadString(IDS_MIDIPLUGIN), 0, AutoBitmap("midi.tif"), String("Midi Plugin (CommandPlugin)"), NewObjClear(MidiPlugin));

    RegisterCommandPlugin(ID_HELLOTEST,
                          "Hello World",
                          0,
                          NULL,
                          String("Hello World"),
                          new HelloWorldTest
                          );
    return true;
}

void PluginEnd(void)
{
}

Bool PluginMessage(Int32 id, void *data)
{
    switch (id) {
        case C4DPL_INIT_SYS:
            if (!resource.Init()) { // don't start plugin without resource
                return false;
            }
            return true;
            
        case C4DMSG_PRIORITY:
            return true;
            
    }
    
    return false;
}
