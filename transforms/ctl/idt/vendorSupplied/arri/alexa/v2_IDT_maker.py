#!/usr/bin/env python

import sys
from os import getenv
import os.path
from datetime import date
from numpy import *

# Globals
IDT_maker_version = "0.05"

nominalEI = 400.0
blackSignal = 0.003907
blackOffset = 0.000977
midGraySignal = 0.01
encodingGain = 0.256598
encodingOffset = 0.391007

def gainForEI(EI) :
    return EI / nominalEI

def encGainForEI(EI) :
    gain = gainForEI(EI)
    return (log(gain)/log(2) * (0.89 - 1.0) / 3 + 1) * encodingGain

def logExposure(EI, x) :
    gain = gainForEI(EI)
    encGain = encGainForEI(EI)
    midGray = midGraySignal + blackOffset
    return log10(((x - blackSignal) * gain + blackOffset) / midGray) * encGain + encodingOffset

def LogC2InverseParametersForEI(EI) :
    gain = gainForEI(EI)
    encGain = encGainForEI(EI)
    midGray = midGraySignal + blackOffset
    f16 = logExposure(EI, blackSignal)
    f17 = logExposure(EI, blackSignal + 1.0 / 4095.0)
    cut = f16
    d = encodingOffset
    c = encGain
    b = blackOffset / midGray
    a = (gain / midGray) / (gain * (0.18 / midGraySignal))
    f = f16
    e = 4095.0 * (f17 - f16) / (gain * (0.18 / midGraySignal))
    return { 'a' : a,
             'b' : b,
             'cut' : cut,
             'c' : c,
             'd' : d,
             'e' : e,
             'f' : f }

def emitLogC2InverseFunction(EI) :
    p = LogC2InverseParametersForEI(EI)
    print "float"
    print "normalizedLogC2ToRelativeExposure(float x) {"
    print "\tif (x > %f)" % p['cut']
    print "\t\treturn (pow(10,(x - %f) / %f) - %f) / %f;" % (p['d'], p['c'], p['b'], p['a'])
    print "\telse"
    print "\t\treturn (x - %f) / %f;" % (p['f'], p['e'])
    print "}"
    print ""

def emitHeader(myName, EI, CCT, logC) :
    print ""
    if logC == "logc" :
        print "// <ACEStransformID>IDT.ARRI.Alexa-v2-logC-EI%d-CCT%d.a1.v1</ACEStransformID>" % (EI,CCT)
        print "// <ACESuserName>ACES 1.0 Input - ARRI V2 LogC (EI%d, %dK)</ACESuserName>" % (EI,CCT)
        print ""
        print "// ARRI ALEXA IDT for ALEXA logC files"
    else :
        print "// <ACEStransformID>IDT.ARRI.Alexa-v2-raw-EI%d-CCT%d.a1.v1</ACEStransformID>" % (EI,CCT)
        print "// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI%d, %dK)</ACESuserName>" % (EI,CCT)
        print ""
        print "// ARRI ALEXA IDT for ALEXA linear files"
    print "//  with camera EI set to %d" % EI
    if CCT != "ignored" :
        print "//  and CCT of adopted white set to %dK" % CCT
    print "// Written by %s v%s on %s" % (myName, IDT_maker_version, date.today().strftime("%A %d %B %Y"))
    print ""

def emitRawSupport(CCT) :
    print "const float EI = %4.1f;" % EI
    print "const float black = 256.0 / 65535.0;"
    print "const float exp_factor = 0.18 / (0.01 * (400.0/EI));"
    print ""

def emitMain(EI, CCT, logC) :
    print "void main"
    print "(\tinput varying float rIn,"
    print "\tinput varying float gIn,"
    print "\tinput varying float bIn,"
    print "\tinput varying float aIn,"
    print "\toutput varying float rOut,"
    print "\toutput varying float gOut,"
    print "\toutput varying float bOut,"
    print "\toutput varying float aOut)"
    print "{"
    print ""
    M = getIDTMatrix(CCT)
    if logC == "logc" :
        print "\tfloat r_lin = normalizedLogC2ToRelativeExposure(rIn);"
        print "\tfloat g_lin = normalizedLogC2ToRelativeExposure(gIn);"
        print "\tfloat b_lin = normalizedLogC2ToRelativeExposure(bIn);"
        print ""
        print "\trOut = r_lin * %f + g_lin * %f + b_lin * %f;" % (M[0,0], M[0,1], M[0,2])
        print "\tgOut = r_lin * %f + g_lin * %f + b_lin * %f;" % (M[1,0], M[1,1], M[1,2])
        print "\tbOut = r_lin * %f + g_lin * %f + b_lin * %f;" % (M[2,0], M[2,1], M[2,2])
        print "\taOut = 1.0;"
    else :
        print "\t// convert to white-balanced, black-subtracted linear values"
        print "\tfloat r_lin = (rIn - black) * exp_factor;"
        print "\tfloat g_lin = (gIn - black) * exp_factor;"
        print "\tfloat b_lin = (bIn - black) * exp_factor;"
        print ""
        print "\t// convert to ACES primaries using CCT-dependent matrix"
        print "\trOut = r_lin * %f + g_lin * %f + b_lin * %f;" % (M[0,0], M[0,1], M[0,2])
        print "\tgOut = r_lin * %f + g_lin * %f + b_lin * %f;" % (M[1,0], M[1,1], M[1,2])
        print "\tbOut = r_lin * %f + g_lin * %f + b_lin * %f;" % (M[2,0], M[2,1], M[2,2])
        print "\taOut = 1.0;"
    print ""
    print "}"

def getIDTMatrix(cct) :
    '''
    Load video matrix coefficients and interpolate for CCT
    '''
    alexaMatrixFile = pathInExecutableDir("AlexaParameters-2013-Nov-13/Alexa_aces_matrix.txt")
    mtab = loadtxt(alexaMatrixFile, skiprows=3)
    i = searchsorted(mtab[...,0], cct)
    if i == size(mtab, 0) - 1:
        m = mtab[i, 1:]
    else :
        a = (1.0/cct - 1.0/mtab[i,0]) / (1.0/mtab[i+1,0] - 1.0/mtab[i,0])
        m = (1- a) * mtab[i, 1:] + a * mtab[i+1, 1:]
    return m.reshape((3,3))

def executableName() :
    (head, tail) = os.path.split(os.path.abspath(sys.argv[0]))
    return tail

def pathInExecutableDir(fileName) :
    (head, tail) = os.path.split(os.path.abspath(sys.argv[0]))
    return head + '/' + fileName

def usage(myName) :
    print "%s: usage is" % myName
    print "\t%s EI CCT logC|raw" % myName

if __name__ == '__main__':
    myName = executableName()
    if len(sys.argv) != 4 :
        usage(myName)
        sys.exit(2)

    EI = float(sys.argv[1])
    CCT = float(sys.argv[2])
    logC = sys.argv[3].lower()

    emitHeader(myName, EI, CCT, logC)
    if logC == "logc" :
        emitLogC2InverseFunction(EI)
    elif logC == "raw" :
        emitRawSupport(CCT)
    else :
        usage(myName)
        sys.exit(2)

    emitMain(EI, CCT, logC)
