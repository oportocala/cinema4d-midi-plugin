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


class MidiPlugin : public ObjectData {
public:
    virtual Bool Init (GeListNode* node);
    static NodeData* Alloc(void) { return NewObjClear(MidiPlugin); }
};

void myCallback( double deltatime, std::vector< unsigned char > *message, void * node ) {
    BaseObject *op = (BaseObject *) node;
    BaseContainer* data = op->GetDataInstance();
    
    data->SetInt32(MIDIPLUGIN_NOTE, (int)message->at(1));
    data->SetFloat(MIDIPLUGIN_NOTE_VALUE, (int)message->at(2));
    
    EventAdd(EVENT_FORCEREDRAW);
}

Bool MidiPlugin::Init(GeListNode* node) {
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
    midiin->setCallback( &myCallback, (void *) node);
    // Don't ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes( false, false, false );

    return true;
}

class Identify : public CommandData {
public:
    virtual Bool Execute(BaseDocument *doc)
    {
        RtMidiIn  *midiin = 0;
        midiin = new RtMidiIn();
        
        std::string str = midiin->getPortName(0);
        char * cstr = new char [str.length() + 1];
        std::strcpy (cstr, str.c_str());
        
        String msg = cstr;
        MessageDialog(msg);

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
                         "Midi Plugin",
                         AutoBitmap("midi.tif"),
                         0
                         );

    RegisterCommandPlugin(ID_HELLOTEST,
                          "Identify",
                          0,
                          NULL,
                          String("Identify"),
                          new Identify
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
