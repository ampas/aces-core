# SPDX-License-Identifier: Apache-2.0
# Copyright Contributors to the ACES Project.
"""Protocol definitions for transform registry interfaces."""

from __future__ import annotations

from typing import Protocol, runtime_checkable


@runtime_checkable
class TransformRegistry(Protocol):
    """Protocol for transform ID registries.

    Implementations provide lookup and listing of known ACES transform URNs.
    """

    def is_valid_transform_id(self, transform_id: str, *, version: str | None = None) -> bool: ...

    def get_transform_info(self, transform_id: str, *, version: str | None = None) -> dict | None: ...

    def list_transforms(self, *, category: str | None = None, version: str | None = None) -> list[dict]: ...

    def list_versions(self) -> list[str]: ...
