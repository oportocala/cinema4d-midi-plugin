#define ID_MIDI_OBJECT 10233423

#include "MidiObject.hpp"
#include "c4d.h"
#include "omidi.h"

class MidiObject : public ObjectData
{
public:
    virtual Bool Init (GeListNode* node);
    static NodeData* Alloc(void) { return NewObjClear(MidiObject); }
};

Bool MidiObject::Init(GeListNode* node)
{
    
    BaseObject*		 op = (BaseObject*)node;
    BaseContainer* data = op->GetDataInstance();
    
    data->SetFloat(NOTE, 100.0);
    
    return true;
}

void registerMidiObject () {
    RegisterObjectPlugin(ID_MIDI_OBJECT,
                         "MidiObject",
                         OBJECT_GENERATOR,
                         MidiObject::Alloc,
                         "Omidi",
                         AutoBitmap("midi.tif"),
                         0
                         );
}
