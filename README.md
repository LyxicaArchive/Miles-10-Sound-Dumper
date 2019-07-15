# Miles-10-Sound-Dumper
Greetings, I've now finished the first beta version of my miles 10 sound dumper. I think I have most bugs ironed out, but obviously please let me know if something is broken. 

In order to use the sound dumper, you'll have to place it inside the Apex Legends folder, right where r5apex.exe is. Then using the command line you can play any sound (as far as I know) and dump them. 

The syntax for command line operation is 

```
OPTIONS:

      --folder=[/audio/ship]            Folder containing Miles audio files
                                        (mprj, mbnk, mstr)
      -o[/miles_audio],
      --out=[/miles_audio]              Folder to place the audio files in
      -l, --list                        List all event IDs and names contained
                                        in the Mile's bank
      -m, --mute                        Mute audio while recording events
      EVENT IDs...                      Enter either one or two event IDs.
                                        Entering only one will cause that event
                                        to be recorded. Entering two event IDs
                                        will record every event between the two
                                        event IDs.
      ADVANCED
        --noise=[0x2000]                  Adjust the noise floor when detecting
                                          silence. Any samples below this value
                                          will be considered silent.
      -h, --help                        Display this help menu
      "--" can be used to terminate flag options and force all following
      arguments to be treated as positional options

```


Issues:
* Because I'm hooking into calls that fill a DirectSoundBuffer, sound data is gathered in real time. This means that dumping each event requires actually playing it. Dumping the entire event list could take quite a while, and some events don't actually end but rather loop forever (press ESC to stop recording a loop event once you've gathered enough sound data). 
* Events can be composed of multiple different source material sounds that are chosen at random. For example, a taunt event might have three different voice lines but only one of them will be dumped when you request it. I don't know at the moment how to detect multiple samples, or how to select a specific one. Just something to keep in mind. 

Misc:
* I think every event is playable now, but some events that are listed don't have data (eg most titanfall 2 sound events).
