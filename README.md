# Cinema4d MIDI Plugin

This repository contains a very simple (OSX-specifc) implementation of a bridge between the MIDI interface and Cinema4D presented as a Plugin.

# The plugin
It will expose two values from the very first MIDI device it finds: **note** and **value**.

From there you can wire it up using XPresso.

# Usage

Clone or download the contents of this repo into a folder that looks like this:

`/Applications/MAXON/CINEMA 4D R18/plugins/dev1/MidiPlugin2`

![Menu](http://vladgoran.ro/img/cinema4d-midi/1.png)

![Plugin](http://vladgoran.ro/img/cinema4d-midi/2.png)

Here is a [video](https://youtu.be/_MPKFVLxePY) of it working

# Work in progress

The plugin at the moment just provides the most basic of implementation.

The code needs some cleanup and I need to obtain a propper Plugin ID from MAXON.

As it is this is a proof of concept. 

Any help with it is welcome. 

# ACK

The plugin uses the excelent [rtmidi](https://github.com/thestk/rtmidi) library.

Thanks to @ursachec for the help and support


Get in touch: oportocala [\@] gmail com
