# SPDX-License-Identifier: Apache-2.0
# Copyright Contributors to the ACES Project.
"""Shared ACES data types."""

from __future__ import annotations

from dataclasses import asdict, dataclass, field


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
