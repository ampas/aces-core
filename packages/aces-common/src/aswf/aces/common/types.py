# SPDX-License-Identifier: Apache-2.0
# Copyright Contributors to the ACES Project.
"""Shared ACES data types."""

from __future__ import annotations

import re
from dataclasses import asdict, dataclass, field


_TRANSFORM_URN_RE = re.compile(
    r"^urn:ampas:aces:transformId:"
    r"v(\d+)\.(\d+):"                         # spec version: v2.0, v1.5
    r"([A-Za-z][A-Za-z0-9]*)"                  # transform type: CSC, Output, IDT, ACEScsc...
    r"\.([^.]+)"                               # namespace: Academy, Arri, Red...
    r"\.(.+)"                                  # name + version suffix
    r"$"
)

# Matches the trailing version suffix: a2.v1, a1.0.3, a1.v1.0, etc.
_VERSION_SUFFIX_RE = re.compile(
    r"^(.+)\.(a\d+(?:\.(?:v?\d+))+)$"
)

@dataclass(frozen=True)
class TransformURN:
    """Parsed ACES transform URN per the Transform ID specification.

    Format: urn:ampas:aces:transformId:v{specVersion}:{TransformType}.{Namespace}.{Name}.{VersionSuffix}

    Example (v2.0):
        urn:ampas:aces:transformId:v2.0:CSC.Academy.ACES_to_ACEScct.a2.v1

    Example (v1.x):
        urn:ampas:aces:transformId:v1.5:ACEScsc.Academy.ACES_to_ACEScct.a1.0.3
    """

    raw: str
    spec_version: str
    spec_major_version: int
    spec_minor_version: int
    transform_type: str
    namespace: str
    name: str
    version_suffix: str

    @classmethod
    def parse(cls, urn: str) -> TransformURN | None:
        """Parse a URN string. Returns None if not a valid ACES transform URN."""
        m = _TRANSFORM_URN_RE.match(urn)
        if m is None:
            return None
        spec_major, spec_minor, transform_type, namespace, name_and_version = m.groups()

        # Split the trailing version suffix (e.g. "ACES_to_ACEScct.a2.v1" -> "ACES_to_ACEScct", "a2.v1")
        vs = _VERSION_SUFFIX_RE.match(name_and_version)
        if vs:
            name, version_suffix = vs.groups()
        else:
            name = name_and_version
            version_suffix = ""

        return cls(
            raw=urn,
            spec_version=f"v{spec_major}.{spec_minor}",
            spec_major_version=int(spec_major),
            spec_minor_version=int(spec_minor),
            transform_type=transform_type,
            namespace=namespace,
            name=name,
            version_suffix=version_suffix,
        )

    @property
    def is_v1(self) -> bool:
        return self.spec_major_version == 1

    @property
    def is_v2(self) -> bool:
        return self.spec_major_version == 2

    @property
    def aces_release_version(self) -> int | None:
        """ACES release major version from the suffix (e.g. 2 from 'a2.v1')."""
        m = re.match(r"a(\d+)", self.version_suffix)
        return int(m.group(1)) if m else None

    @property
    def transform_version(self) -> str | None:
        """Transform version from the suffix (e.g. 'v1' from 'a2.v1', '0.3' from 'a1.0.3')."""
        m = re.match(r"a\d+\.(.*)", self.version_suffix)
        return m.group(1) if m else None

    def __str__(self) -> str:
        return self.raw


@dataclass
class TransformInfo:
    """Information about a single ACES transform."""

    transform_id: str
    user_name: str
    transform_type: str
    aces_version: str | None = None
    inverse_transform_id: str | None = None
    previous_equivalent_ids: list[str] = field(default_factory=list)

    def to_dict(self) -> dict:
        """Return a plain dict representation."""
        return asdict(self)
