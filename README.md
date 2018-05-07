# Guile Play

A game library for GNU Guile

## Dependencies

* GNU Guile
* libsdl2
* libsdl-image
* libsdl-ttf
* libsdl-mixer

## Compiling

### Linux

#### Dependencies

Install the dependencies.  On Debian/Ubuntu this can be done with the following command

```sh
sudo apt install libguile-dev libsdl2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev  libsdl-ttf2.0-dev
```

#### Compiling

Run `make` in the directory.  This will compile the native extension.


#### Test compilation by running the example application

```
guile game.scm
```

### Windows

Compilation on Windows is not currently supported.  This is a planned future feature.
