# Guile Play

A game library for GNU Guile

## Dependencies

* GNU Guile
* libsdl2
* libsdl-image

## Compiling

### Linux

#### Dependencies

Install the dependencies.  On Debian/Ubuntu this can be done with the following command

```sh
sudo apt install libguile-dev libsdl2-dev libsdl-image-dev
```

#### Compiling

Run `make` in the directory.  This will compile the native extension.


#### Test compilation by running the example application

```
chmod +x game.scm
./game.scm
```

### Windows

Compilation on Windows is not currently supported.  This is a planned future feature.