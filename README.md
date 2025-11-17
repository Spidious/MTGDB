# MTGDB
---
A simple Magic: The Gathering card database and search.

## Info
Information is pulled from Scryfall using the [Scryfall API](https://scryfall.com/docs/api).
Data from the API is stored into the database.

## Dependencies
- CMAKE 
- VCPKG (usually handled in IDE)
    - [Setup Instructions](https://github.com/microsoft/vcpkg?tab=readme-ov-file#get-started)

## Setup
_This is easier when using an IDE such as Visual Studio or CLion_
1. Select IDE and clone Repository
2. Visual Studio:
   1. Ensure C/C++ toolkit is installed, must have VCPKG
3. CLion:
   1. Enable VCPKG tool and add to project. 
   2. In CLion CMAKE profile settings cut/copy the path from CMAKE options and define the environment variable VCPKG_ROOT below with it.
   3. Set CMAKE options to `--preset <preset>`
   4. Reload CMAKE project and delete the original CLION CMAKE output folder.

## Available Presets
_Defined in CMakePresets.json_
- `x64-windows-release`
- `x64-windows-debug`
- `x64-linux-release`
- `x64-linux-debug`