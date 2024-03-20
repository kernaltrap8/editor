# editor
Simple CLI text editor in C
# notes 
`editor` is not meant to replace other CLI text editors such as `vim`, `nano`, or even `ed` :trollface:. it is simply a fun project i decided to take on. this was made by a child, so if it sucks, thats why.
# building
this project uses the `meson` build system. install it with your system package manager.

to build the project, run these commands:

```
meson setup build
```

to setup the meson build system

```
cd build
```

to change to the `build` directory

```
meson compile
```

to compile `editor`.
**DO NOT RUN THE `autogen.sh` SCRIPT! this is meant for developers!**