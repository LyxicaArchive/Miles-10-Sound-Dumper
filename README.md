# Miles-10-Sound-Dumper
Greetings, I've now finished the first beta version of my miles 10 sound dumper. I think I have most bugs ironed out, but obviously please let me know if something is broken. 

In order to use the sound dumper, you'll have to place it inside the Apex Legends folder, right where r5apex.exe is. Then using the command line you can play any sound (as far as I know) and dump them. 
```
Code:
MSD                                     -- Open MSD to play sounds
MSD <EventID>                           -- Dump an event ID
MSD <EventIDStart> <EventIDEnd>         -- Dump a range of sounds, inclusive
MSD -l                                  -- Display list of all event IDs and names contained in audio files
```


Issues:
* Because I'm hooking into calls that fill a DirectSoundBuffer, sound data is gathered in real time. This means that dumping each event requires actually playing it. Dumping the entire event list could take quite a while, and some events don't actually end but rather loop forever (press ESC to stop recording a loop event once you've gathered enough sound data). 
* Events can be composed of multiple different source material sounds that are chosen at random. For example, a taunt event might have three different voice lines but only one of them will be dumped when you request it. I don't know at the moment how to detect multiple samples, or how to select a specific one. Just something to keep in mind. 

Misc:
* I think every event is playable now, but some events that are listed don't have data (eg most titanfall 2 sound events).
