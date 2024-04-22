## Academy Color Encoding System Core Transforms ##

[![CLA assistant](https://cla-assistant.io/readme/badge/ampas/aces-dev)](https://cla-assistant.io/ampas/aces-dev)

The Academy Color Encoding System (ACES) is a set of components that facilitates a wide range of motion picture and television workflows while eliminating the ambiguity of legacy file formats.  The system is designed to support both all-digital and hybrid film-digital motion picture workflows.

The basic ACES components are:

* Color encoding and metric specifications, file format specifications, color
transformations, and an open source reference implementation 
    * This repository (`aces-dev`) will be renamed (`aces-core`) and houses the main rendering algorithms for ACES.
    * A set of preset Output Transforms to common outputs are tracked at [aces-output](https://github.com/ampas/aces-output)
    * Input Transforms are tracked at  [aces-input-and-colorspaces](https://github.com/ampas/aces-input-and-colorspaces)
    * Look Transforms are collected in [aces-look](https://github.com/ampas/aces-look)
    * AMF schema and example files can be found at [aces-amf](https://github.com/ampas/aces-amf)
    * Documentation is written in markdown and tracked at [aces-docs](https://github.com/ampas/aces-docs). It is published using mkdocs to [ACEScentral](docs.acescentral.com).

* Corresponding reference images
* Documentation on the system and software tools can be found in ['aces-docs'](https://github.com/ampas/aces-docs)

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
