Images (v0.1.1)
=========
Reference images are provided in order to compare your implementation with the result of the CTL reference transforms.

The ACES images in the aces directory are encoded according to the ACES specification found in SMPTE 2065-1. However, due to file size concerns, the files do **not** follow the ACES container specification. Instead, the ACES files are stored in an OpenEXR container using PIZ compression. Again, **no attempt has been made to reconcile the files in this directory with any ACES container specification.**

The other images in this directory have also been compressed for faster pull from Github. 
The OCES images are OpenEXR files using PIZ compression. 
Images that are outputs from the ODT transforms are 16-bit TIFF files compressed with deflate (ZIP) encoding.

------------

Additionally, a few 25-frame image sequences can be downloaded from::

<ftp://acesguest:N525574e@oscars-ftp01.ristech.net/images>

**NOTE:** In each directory structure ``/images/camOrig/showName/sceneName/cameraName`` there exists a README file that details::

    - the encoding of the camera original files
    - the IDT used
    - exposure adjustments (if any)

Following the guidelines of these README files is required to obtain the corresponding ACES files.