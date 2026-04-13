# SPDX-License-Identifier: Apache-2.0
"""Tests for TransformURN parsing."""

import pytest

from aces.common.types import TransformURN


class TestTransformURNParse:
    def test_parse_v15_csc(self):
        urn = TransformURN.parse("urn:ampas:aces:transformId:v1.5:ACEScsc.Academy.ACES_to_ACEScct.a1.0.3")
        assert urn is not None
        assert urn.spec_version == "v1.5"
        assert urn.spec_major_version == 1
        assert urn.spec_minor_version == 5
        assert urn.transform_type == "ACEScsc"
        assert urn.namespace == "Academy"
        assert urn.name == "ACES_to_ACEScct"
        assert urn.version_suffix == "a1.0.3"
        assert urn.is_v1 is True
        assert urn.is_v2 is False

    def test_parse_v20_csc(self):
        urn = TransformURN.parse("urn:ampas:aces:transformId:v2.0:CSC.Academy.ACES_to_ACEScct.a2.v1")
        assert urn is not None
        assert urn.spec_version == "v2.0"
        assert urn.spec_major_version == 2
        assert urn.spec_minor_version == 0
        assert urn.transform_type == "CSC"
        assert urn.namespace == "Academy"
        assert urn.name == "ACES_to_ACEScct"
        assert urn.version_suffix == "a2.v1"
        assert urn.is_v1 is False
        assert urn.is_v2 is True

    def test_parse_v20_output(self):
        urn = TransformURN.parse("urn:ampas:aces:transformId:v2.0:Output.Academy.Rec709-D65_100nit_in_Rec709-D65_BT1886.a2.v1")
        assert urn is not None
        assert urn.transform_type == "Output"
        assert urn.namespace == "Academy"
        assert urn.name == "Rec709-D65_100nit_in_Rec709-D65_BT1886"
        assert urn.version_suffix == "a2.v1"

    def test_parse_v15_lmt(self):
        urn = TransformURN.parse("urn:ampas:aces:transformId:v1.5:LMT.Academy.ReferenceGamutCompress.a1.v1.0")
        assert urn is not None
        assert urn.transform_type == "LMT"
        assert urn.namespace == "Academy"
        assert urn.name == "ReferenceGamutCompress"
        assert urn.version_suffix == "a1.v1.0"
        assert urn.is_v1 is True

    def test_parse_v15_idt_manufacturer(self):
        urn = TransformURN.parse("urn:ampas:aces:transformId:v1.5:IDT.ARRI.ARRI-LogC4.a1.v1")
        assert urn is not None
        assert urn.transform_type == "IDT"
        assert urn.namespace == "ARRI"
        assert urn.name == "ARRI-LogC4"
        assert urn.version_suffix == "a1.v1"

    def test_parse_v20_look(self):
        urn = TransformURN.parse("urn:ampas:aces:transformId:v2.0:Look.Academy.ReferenceGamutCompress.a2.v1")
        assert urn is not None
        assert urn.transform_type == "Look"
        assert urn.name == "ReferenceGamutCompress"
        assert urn.version_suffix == "a2.v1"

    def test_parse_returns_none_for_invalid(self):
        assert TransformURN.parse("not-a-urn") is None
        assert TransformURN.parse("") is None
        assert TransformURN.parse("urn:ampas:aces:transformId:v1.5:") is None

    def test_parse_returns_none_for_legacy_short_name(self):
        assert TransformURN.parse("ACEScsc.ACES_to_ACEScc.a1.0.3") is None

    def test_str_returns_raw(self):
        raw = "urn:ampas:aces:transformId:v2.0:CSC.Academy.ACES_to_ACEScct.a2.v1"
        urn = TransformURN.parse(raw)
        assert str(urn) == raw

    def test_aces_release_version_v20(self):
        urn = TransformURN.parse("urn:ampas:aces:transformId:v2.0:CSC.Academy.ACES_to_ACEScct.a2.v1")
        assert urn.aces_release_version == 2
        assert urn.transform_version == "v1"

    def test_aces_release_version_v15_semver(self):
        urn = TransformURN.parse("urn:ampas:aces:transformId:v1.5:ACEScsc.Academy.ACES_to_ACEScct.a1.0.3")
        assert urn.aces_release_version == 1
        assert urn.transform_version == "0.3"

    def test_aces_release_version_v15_mixed(self):
        urn = TransformURN.parse("urn:ampas:aces:transformId:v1.5:LMT.Academy.ReferenceGamutCompress.a1.v1.0")
        assert urn.aces_release_version == 1
        assert urn.transform_version == "v1.0"

    def test_aces_release_version_empty_suffix(self):
        """ACESlib transforms may not parse a version suffix."""
        urn = TransformURN.parse("urn:ampas:aces:transformId:v1.5:ACESlib.ODT_Common.a1.1.0")
        assert urn.aces_release_version is None
        assert urn.transform_version is None

    def test_frozen(self):
        urn = TransformURN.parse("urn:ampas:aces:transformId:v2.0:CSC.Academy.ACES_to_ACEScct.a2.v1")
        with pytest.raises(AttributeError):
            urn.spec_version = "v3.0"


