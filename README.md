# PrintFile

PrintFile is a utility to send files to a network printer using the AppSocket
proocol (also called Port 9100, RAW or JetDirect). It sends the file without
change, so the file should be in some format that the printer can understand.

The program is intended to be used in conjunction with VMware Player's ability
to redirect a parallel port to a file. I'm using that setup to get printing
working in a legacy DOS application.

The program has two modes of operation: either you can manually print a file
by using the *File* -> *Print File...* menu command, or you can have it use
a watch file.

## Watch File

If you activate the Watch File checkbox and configure a path, PrintFile will
watch that file. Once the file is created and data appears in it, after it
has been undisturbed for 3 seconds, PrintFile will send its contents to the
configured printer, then clear the file contents.

In Watch File mode, PrintFile will also minimize to the system tray instead
of exiting if the window is closed. Use *File* -> *Quit* to actually quit it.

## Supported Printers

This program only works with networked printers that support the AppSocket
protocol. As PrintFile sends files to the printer without trying to interpret
them in any way, the files you send to the printer need to be in a format that
the printer supports.
