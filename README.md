## Academy Color Encoding System Core Transforms ##

[![CLA assistant](https://cla-assistant.io/readme/badge/ampas/aces-dev)](https://cla-assistant.io/ampas/aces-dev)

The Academy Color Encoding System (ACES) is a set of components that facilitates a wide range of motion picture and television workflows while eliminating the ambiguity of legacy file formats.  The system is designed to support both all-digital and hybrid film-digital motion picture workflows.

In ACES 2.0, the basic ACES components are divided across a few individual repositories:

* This repository (`aces-core`) houses the core transforms for ACES.
* A set of preset Output Transforms to common outputs are tracked at [aces-output](https://github.com/ampas/aces-output), along with reference images that can be used to verify the output of each transform
* Input Transforms are tracked at  [aces-input-and-colorspaces](https://github.com/ampas/aces-input-and-colorspaces)
* Look Transforms are collected in [aces-look](https://github.com/ampas/aces-look)
* AMF schema and example files can be found at [aces-amf](https://github.com/ampas/aces-amf)
* Documentation is written in markdown and tracked at [aces-docs](https://github.com/ampas/aces-docs). It is published using mkdocs to [ACEScentral](docs.acescentral.com).

Regular snapshots of the entire system bundled, tagged and can be downloaded from [aces](https://github.com/ampas/aces).

## Previous Versions
The full code history of the ACES pre-2.0 remains in the commit history of this repository and can be accessed by checking out the relevant branch and/or tagged commit to view the transforms at that previous version.

Tagged versions of ACES can be browsed in the [tag history](https://github.com/ampas/aces-dev/tags), including [ACES version 1.3](https://github.com/ampas/aces-dev/releases/tag/v1.3)

## Prerequisites ##

### Color Transformation Language ###

Color Transformation Language (CTL) can be downloaded from
https://github.com/ampas/CTL

## License ##
This project is licensed under the terms of the [LICENSE](./LICENSE.md) agreement.

## Contributing ##
Thank you for your interest in contributing to our project. Before any contributions can be accepted, we require contributors to sign a Contributor License Agreement (CLA) to ensure that the project can freely use your contributions. You can find more details and instructions on how to sign the CLA in the [CONTRIBUTING.md](./CONTRIBUTING.md) file.

## Support ## 
For support, please visit [ACESCentral.com](https://acescentral.com)
