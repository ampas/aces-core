# aces-common

Shared protocols and types used across the ACES AMF Python packages.

This is a foundation package. Most users should install [aces-amf-utils](../aces-amf-utils/) or [aces-amf-lib](../aces-amf-lib/) instead.

## Installation

```bash
pip install aces-common
```

## Exports

### `TransformRegistry` (Protocol)

Interface for transform ID registries. Implemented by `ACESTransformRegistry` in [aces-transforms](../aces-transforms/).

```python
from aces_common import TransformRegistry

class MyRegistry:
    def is_valid_transform_id(self, transform_id: str, *, version: str | None = None) -> bool: ...
    def get_transform_info(self, transform_id: str, *, version: str | None = None) -> dict | None: ...
    def list_transforms(self, *, category: str | None = None, version: str | None = None) -> list[dict]: ...
```

### `TransformInfo` (Dataclass)

Metadata for a single ACES transform:

```python
from aces_common import TransformInfo

# Fields: transform_id, user_name, transform_type, aces_version,
#         inverse_transform_id, previous_equivalent_ids
```

### Constants

```python
from aces_common import INPUT_TRANSFORM_TYPES, OUTPUT_TRANSFORM_TYPES

INPUT_TRANSFORM_TYPES   # frozenset{"IDT", "CSC", "Input"}
OUTPUT_TRANSFORM_TYPES  # frozenset{"ODT", "Output"}
```

## License

Apache-2.0
