# shared-memory-chat-application

### This application makes use of GTK library for integrating the code with frontend xml files.

#### In order to install GTK on Ubuntu :

- `$ sudo apt-get update -y`
- `$ sudo apt-get install -y gtk-sharp3`

#### Once that's done, to run the program:

- Open up two separate terminals in the same project directory.
- In the first terminal type in `` gcc `pkg-config --cflags gtk+-3.0` -o a.out main1.c `pkg-config --libs gtk+-3.0` `` to compile. (Ignore the warnings)
- To execute, type in `./a.out`.

- In the second terminal type in `` gcc `pkg-config --cflags gtk+-3.0` -o o.out main2.c `pkg-config --libs gtk+-3.0` `` to compile. (Ignore the warnings)
- Finally to execute, type in `./o.out`.

**_Have fun chatting! :)_**
