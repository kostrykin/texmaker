# Texmaker 4.5 Fork

Build statuses: ![master](https://travis-ci.org/kostrykin/texmaker.svg?branch=master) on `master` branch
and ![develop](https://travis-ci.org/kostrykin/texmaker.svg?branch=develop) on `develop` branch

## Features

The currently added features are:

  - the name of the temporary build-subdirectory can be chosen arbitrarily
  - support for automatic execution of a script file after Quick Build finishes successfully
  - the `BUILD.sh` script now prompts, whether installation should be performed

## Suggested Configuration

The following configuration ensures, that your working directory, where you have your `.tex` files, stays clean:

  - set `.build` to be the name of the build-subdirectory
  - download [this script](https://gist.github.com/kostrykin/dc63ae31e2b02a6448c891fed755c1ea) and put it e.g. into `/usr/local/bin/lift-latex-pdf.py`
  - use `/usr/local/bin/lift-latex-pdf.py .build %` as the post quick build script
