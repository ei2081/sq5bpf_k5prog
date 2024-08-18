
1. [osx notes](#osx-notes)
1. [build](#build)
1. [execution](#execution)

# OSx notes
Compiled as per README on OSx 13.6.1

## wrong serial port device for OSx
After running `./k5prog -r -v -p /dev/tty.usbserial-14110` the program just hangs. 
By adding some debug prints we can see if hangs after the call to `open` in `int openport(char *port,speed_t speed)`

Digging around we find this [ convo on SO](https://stackoverflow.com/questions/66465448/opening-usb-device-for-communication-in-c)  and learn from the comments that  `/dev/tty` is "for incoming calls. So open waits until a call arrives", and we need (at least on osx) to use `/dev/cu.*` for sending a request to a device without waiting.

## Attribute setting error
After running `./k5prog -r -v -p /dev/cu.usbserial-14110` the program flies through the open only to fail on the `tcsetattr` in the same function with message `tcsetattr error 22 Invalid argument`.

Hack/Fix attempts:
- Updating the calls to `my_termios.c_cflag` doesn't seem to affect things.
- Removing the call to `cfmakeraw`  doesn't seem to affect things.
- Removing the call to `cfsetospeed` lets the program continue through `k5_prepare` where it retries ca. 9 times with `read_timeout` each go and eventually stops with `Failed to init radio`.

TBC Ref: https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c



# build
```console
make clean
make osx
```

# execution
```console
./k5prog -r -v -p /dev/cu.usbserial-123456
```

# build flow ei2081
### clean first
```console
sq5bpf_k5prog % make clean
rm k5prog
sq5bpf_k5prog %
```

## then build
i duplicated the default build in to try variatons of flags. 
```console
sq5bpf_k5prog % make osx
gcc -g -O2 -Wall  k5prog.c -o k5prog
sq5bpf_k5prog % 
```

## find my serial port
note the discussion referenced [in the README.md](./README.md) section `## OSx notes` about using `/dev/cu*` instead of `/dev/tty*` for outgoing comms.
```console
sq5bpf_k5prog % ls /dev/tty*usb*                          
/dev/tty.usbserial-14110
```

## run 01
```
sq5bpf_k5prog % ./k5prog -r -v -p /dev/cu.usbserial-14110
Quansheng UV-K5 EEPROM programmer v0.9 (c) 2023 Jacek Lipkowski <sq5bpf@lipkowski.org>

tcsetattr error 22 Invalid argument
Open /dev/cu.usbserial-14110 failed
sq5bpf_k5prog % 
```

## run 02
I added some debugging to zero in on the above error.

We can see it enters `main`, goes into `openport()`, and seems to be fine with [getting attributes via tcgetattr](https://pubs.opengroup.org/onlinepubs/007904975/functions/tcgetattr.html), [flushing](https://pubs.opengroup.org/onlinepubs/007904975/functions/tcflush.html)
only to eventually fails on [setting attributes via tcsetattr](https://pubs.opengroup.org/onlinepubs/007904975/functions/tcsetattr.html).

As I write this i wonder about call order, but....

The debug build is:
```console
sq5bpf_k5prog % make osxd
gcc -g -O2 -Wall -D DEBUG=1 k5prog.c -o k5prog
```

The debug run is the same as before, but with this output:
```console
sq5bpf_k5prog % ./k5prog -r -v -p /dev/cu.usbserial-14110
Quansheng UV-K5 EEPROM programmer v0.9 (c) 2023 Jacek Lipkowski <sq5bpf@lipkowski.org>

*** openport...
*** openport.open...
*** .../openport.open
*** >openport: fd !< 0
*** openport.tcgetattr...
*** .../openport.tcgetattr
*** openport.tcflush...
*** .../openport.tcflush
*** >flag setting for my_termios
*** openport.cfmakeraw...
*** .../openport.cfmakeraw
*** openport.cfsetospeed...
*** .../openport.cfsetospeed
*** openport.tcsetattr...
tcsetattr error 22 Invalid argument
*** .../openport
Open /dev/cu.usbserial-14110 failed
sq5bpf_k5prog % 
```
