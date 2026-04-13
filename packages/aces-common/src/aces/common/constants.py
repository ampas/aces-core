# SPDX-License-Identifier: Apache-2.0
# Copyright Contributors to the ACES Project.
"""
Constants derived from the AMF XSD schemas.

Transform type prefix constants map to XSD simpleType definitions that
restrict which transform ID URN prefixes are valid per container type.
The v1 and v2 schemas define different allowed prefixes.
"""

# Transform types considered as input transforms (used by bulk_generator)
INPUT_TRANSFORM_TYPES: frozenset[str] = frozenset({"IDT", "CSC", "Input"})

# Transform types considered as output transforms (used by bulk_generator)
OUTPUT_TRANSFORM_TYPES: frozenset[str] = frozenset({"ODT", "Output"})

# Transform types considered as look transforms
LOOK_TRANSFORM_TYPES: frozenset[str] = frozenset({"LMT", "Look"})


# ──────────────────────────────────────────────────────────────────────
# AMF v2 XSD — allowed transform ID URN prefixes (v1.5 + v2.0 URNs)
# Source: packages/aces-amf-lib/src/aces.amf_lib/data/amf-schema/v2/acesMetadataFile.xsd
# ──────────────────────────────────────────────────────────────────────

# tnInputTransform (v2 XSD line 71): IDT, ACEScsc (v1.5) | Input, CSC (v2.0)
V2_INPUT_TRANSFORM_PREFIXES: frozenset[str] = frozenset({"IDT", "ACEScsc", "Input", "CSC"})
# tnOutputTransform (v2 XSD line 78): RRTODT (v1.5) | Output (v2.0)
V2_OUTPUT_TRANSFORM_PREFIXES: frozenset[str] = frozenset({"RRTODT", "Output"})
# tnLookTransform (v2 XSD line 64): LMT (v1.5) | Look (v2.0)
V2_LOOK_TRANSFORM_PREFIXES: frozenset[str] = frozenset({"LMT", "Look"})
# tnColorSpaceConversionTransform (v2 XSD line 92): ACEScsc (v1.5) | CSC (v2.0)
V2_CSC_TRANSFORM_PREFIXES: frozenset[str] = frozenset({"ACEScsc", "CSC"})
# tnInverseOutputTransform (v2 XSD line 85): InvRRTODT (v1.5) | InvOutput (v2.0)
V2_INVERSE_OUTPUT_TRANSFORM_PREFIXES: frozenset[str] = frozenset({"InvRRTODT", "InvOutput"})
# v1.x only sub-transforms (same in v2 XSD as v1 XSD — no v2.0 equivalents)
# tnReferenceRenderingTransform (v2 XSD line 100): RRT only
V2_RRT_TRANSFORM_PREFIXES: frozenset[str] = frozenset({"RRT"})
# tnInverseReferenceRenderingTransform (v2 XSD line 106): InvRRT only
V2_INVERSE_RRT_TRANSFORM_PREFIXES: frozenset[str] = frozenset({"InvRRT"})
# tnOutputDeviceTransform (v2 XSD line 112): ODT only
V2_ODT_TRANSFORM_PREFIXES: frozenset[str] = frozenset({"ODT"})
# tnInverseOutputDeviceTransform (v2 XSD line 118): InvODT only
V2_INVERSE_ODT_TRANSFORM_PREFIXES: frozenset[str] = frozenset({"InvODT"})


# ──────────────────────────────────────────────────────────────────────
# XSD regex patterns for transform IDs, keyed by transform type.
# Source: packages/aces-amf-lib/src/aces.amf_lib/data/amf-schema/v2/acesMetadataFile.xsd
#
# Each transform type maps to a list of regex patterns (from xs:pattern).
# A transform ID is valid if it matches ANY pattern for its type.
# The v2 XSD accepts both v1.5 and v2.0 URN forms.
# ──────────────────────────────────────────────────────────────────────

# Transform types that have no XSD pattern (library/utility transforms,
# not placed in AMF pipeline slots).
# TODO: Define patterns for these types.
TRANSFORM_TYPES_WITHOUT_PATTERN: frozenset[str] = frozenset({
    "ACESlib", "ACESutil", "Lib", "InvLMT", "InvLook",
})

TRANSFORM_ID_PATTERNS: dict[str, list[str]] = {
    # tnInputTransform
    "IDT": [
        r"urn:ampas:aces:transformId:v1\.5:IDT\.\S+\.\S+\.a\d+\.v\d+",
    ],
    "ACEScsc": [
        r"urn:ampas:aces:transformId:v1\.5:(ACEScsc\.\S+\.\S+\.a\d+\.v\d+|ACEScsc\.Academy\.\S+\.a\d+\.\d+\.\d+)",
    ],
    "Input": [
        r"urn:ampas:aces:transformId:v2\.0:Input\.\S+\.\S+\.a\d+\.v\d+",
    ],
    "CSC": [
        r"urn:ampas:aces:transformId:v2\.0:CSC\.\S+\.\S+\.a\d+\.v\d+",
    ],
    # tnOutputTransform
    "RRTODT": [
        r"urn:ampas:aces:transformId:v1\.5:(RRTODT\.\S+\.\S+\.a\d+\.v\d+|RRTODT\.Academy\.\S+\.a\d+\.\d+\.\d+)",
    ],
    "Output": [
        r"urn:ampas:aces:transformId:v2\.0:Output\.\S+\.\S+\.a\d+\.v\d+",
    ],
    # tnLookTransform
    "LMT": [
        r"urn:ampas:aces:transformId:v1\.5:(LMT\.\S+\.\S+\.a\d+\.v\d+|LMT\.Academy\.\S+\.a\d+\.v?\d+\.\d+)",
    ],
    "Look": [
        r"urn:ampas:aces:transformId:v2\.0:Look\.\S+\.\S+\.a\d+\.v\d+",
    ],
    # tnReferenceRenderingTransform / tnInverseReferenceRenderingTransform
    "RRT": [
        r"urn:ampas:aces:transformId:v1\.5:RRT\.a\d+\.\d+\.\d+",
    ],
    "InvRRT": [
        r"urn:ampas:aces:transformId:v1\.5:InvRRT\.a\d+\.\d+\.\d+",
    ],
    # tnOutputDeviceTransform / tnInverseOutputDeviceTransform
    "ODT": [
        r"urn:ampas:aces:transformId:v1\.5:(ODT\.\S+\.\S+\.a\d+\.v\d+|ODT\.Academy\.\S+\.a\d+\.\d+\.\d+)",
    ],
    "InvODT": [
        r"urn:ampas:aces:transformId:v1\.5:(InvODT\.\S+\.\S+\.a\d+\.v\d+|InvODT\.Academy\.\S+\.a\d+\.\d+\.\d+)",
    ],
    # tnInverseOutputTransform
    "InvRRTODT": [
        r"urn:ampas:aces:transformId:v1\.5:(InvRRTODT\.\S+\.\S+\.a\d+\.v\d+|InvRRTODT\.Academy\.\S+\.a\d+\.\d+\.\d+)",
    ],
    "InvOutput": [
        r"urn:ampas:aces:transformId:v2\.0:InvOutput\.\S+\.\S+\.a\d+\.v\d+",
    ],
}
