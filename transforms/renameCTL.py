#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Rename ACES CTL transforms in an ACES repository based on the ACEStransformID of each file

usage: renameCTL.py [-h] [-v] [-i PATH]

Function to rename ACES CTL transforms in an ACES repository based on the
ACEStransformID of each file

optional arguments:
  -h, --help            show this help message and exit
  -v, --verbose         display verbose output
  -i PATH, --inputPath PATH
                        root path where files to be renamed are found
"""

import os

__author__ = 'Alex Forsythe'
__copyright__ = 'Copyright (C) 2016 Academy of Motion Picture Arts and Sciences'
__maintainer__ = 'Academy of Motion Picture Arts and Sciences'
__email__ = 'acessupport@oscars.org'
__status__ = 'Production'

__major_version__ = '1'
__minor_version__ = '0'
__change_version__ = '0'
__version__ = '.'.join((__major_version__,
                        __minor_version__,
                        __change_version__))


def get_transform_id(filepath):

    f = open(filepath, 'r')
    filedata = f.read()
    f.close()

    for item in filedata.split("</ACEStransformID>"):
        if "<ACEStransformID>" in item:
            transform_id = item[item.find("<ACEStransformID>") + len("<ACEStransformID>"):]

    return transform_id


def update_import_statements(filepath, lib):

    f = open(filepath, 'r')
    filedata = f.read()
    f.close()

    for key in lib.keys():
        search_string = 'import "{}";'.format(key)
        replace_string = 'import "{}";'.format(lib[key])

        try:
            newdata
        except NameError:
            newdata = filedata.replace(search_string, replace_string)
        else:
            newdata = newdata.replace(search_string, replace_string)

    f = open(filepath, 'w')
    f.write(newdata)
    f.close()


def rename_ctl_file(filepath):

    path = os.path.dirname(filepath)
    transform_id = get_transform_id(filepath)
    filepath_new = os.path.join(path, transform_id + '.ctl')
    os.rename(filepath, filepath_new)

    return filepath_new


def build_fn_transform_id_lib(filepath, lib):

    fn = os.path.basename(filepath)
    key = os.path.splitext(fn)[0]
    transform_id = get_transform_id(filepath)
    lib[key] = transform_id

    return lib


def main(args):

    print("\nConverting ACES transform repository in " + args.rootdir + " to use transform IDs as file names ...")

    # Find ACESlib files an build a dictionary with the file name and transform ID
    lib_fn = {}
    for root, subFolders, files in os.walk(args.rootdir):
        for fn in (fn for fn in files if (fn.startswith("ACESlib") and fn.endswith(".ctl"))):
            lib_fn = build_fn_transform_id_lib(os.path.join(root, fn), lib_fn)

            if args.verbose:
                print("Getting transformID for lib file " + os.path.join(root, fn))

    # Update import statements and rename each file
    for root, subFolders, files in os.walk(args.rootdir):
        for fn in (fn for fn in files if fn.endswith(".ctl")):
            update_import_statements(os.path.join(root, fn), lib_fn)
            filepath_new = rename_ctl_file(os.path.join(root, fn))

            if args.verbose:
                print("Updating import statement in " + os.path.join(root, fn))
                print("Getting transformID for file " + os.path.join(root, fn))
                if os.path.join(root, fn) != filepath_new:
                    print("Renaming " + os.path.join(root, fn) + " " + filepath_new)
                else:
                    print("Filename already matches transformID")

    print("Conversion Complete")


if __name__ == '__main__':
    
    import argparse

    parser = argparse.ArgumentParser(description="Function to rename ACES CTL transforms in an ACES repository "
                                                 "based on the ACEStransformID of each file")
    parser.add_argument("-v", "--verbose", required=False, dest="verbose", action='store_true',
                        help="display verbose output", default=False)
    parser.add_argument("-i", "--inputPath", required=False, dest="rootdir",
                        help="root path where files to be renamed are found", metavar="PATH", default=os.getcwd())

    args = parser.parse_args()

    main(args)
