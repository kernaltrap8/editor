# editor
Simple CLI text editor in C
# notes 
`editor` is not meant to replace other CLI text editors such as `vim`, `nano`, or even `ed` :trollface:. it is simply a fun project i decided to take on. this was made by a child, so if it sucks, thats why.
# Building
This project uses the `meson` build system. install it with your system package manager.

To build the project, run these commands:

```
meson setup build
```

To setup the meson build system

```
cd build
```

To change to the `build` directory

```
meson compile
```

To compile `editor`.

**DO NOT RUN THE `autogen.sh` SCRIPT! This is meant for developers!**
# Installation
Run
```
meson install -C build/
```
This will install `editor` to `/usr/local/bin/editor`.
