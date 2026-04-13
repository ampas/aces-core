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
from aces.common import TransformRegistry

class MyRegistry:
    def is_valid_transform_id(self, transform_id: str, *, version: str | None = None) -> bool: ...
    def get_transform_info(self, transform_id: str, *, version: str | None = None) -> dict | None: ...
    def list_transforms(self, *, category: str | None = None, version: str | None = None) -> list[dict]: ...
    def list_versions(self) -> list[str]: ...
    def get_equivalent_id(self, transform_id: str) -> str | None: ...
    def get_equivalent_ids(self, transform_id: str) -> list[str]: ...
```

### `TransformURN` (Dataclass)

Parses ACES transform URNs into their component fields per the [Transform ID specification](https://docs.acescentral.com/system-components/transform-id-spec/).

```
urn:ampas:aces:transformId:v{specVersion}:{TransformType}.{Namespace}.{Name}.a{ACESRelease}.v{TransformVersion}
```

```python
from aces.common import TransformURN

urn = TransformURN.parse("urn:ampas:aces:transformId:v2.0:CSC.Academy.ACES_to_ACEScct.a2.v1")

urn.raw                   # "urn:ampas:aces:transformId:v2.0:CSC.Academy.ACES_to_ACEScct.a2.v1"
urn.spec_version          # "v2.0"
urn.spec_major_version    # 2
urn.spec_minor_version    # 0
urn.transform_type        # "CSC"
urn.namespace             # "Academy"
urn.name                  # "ACES_to_ACEScct"
urn.version_suffix        # "a2.v1"
urn.aces_release_version  # 2
urn.transform_version     # "v1"
urn.is_v1                 # False
urn.is_v2                 # True
```

The version suffix format varies between ACES versions:

| ACES version | Suffix format | Example | `aces_release_version` | `transform_version` |
|---|---|---|---|---|
| v2.0 | `a{N}.v{N}` | `a2.v1` | `2` | `"v1"` |
| v1.x (semver) | `a{N}.{N}.{N}` | `a1.0.3` | `1` | `"0.3"` |
| v1.x (mixed) | `a{N}.v{N}.{N}` | `a1.v1.0` | `1` | `"v1.0"` |

`TransformURN.parse()` returns `None` for strings that are not valid ACES transform URNs (e.g. legacy short names like `ACEScsc.ACES_to_ACEScc.a1.0.3`).

#### Edge cases

Internal library and utility transforms (`ACESlib`, `ACESutil`) do not include a separate namespace field — their format is `{Type}.{Name}.{VersionSuffix}` rather than `{Type}.{Namespace}.{Name}.{VersionSuffix}`. For these transforms, the `name` field will contain the version suffix and `version_suffix` will be empty, so `aces_release_version` and `transform_version` will return `None`. These transforms are not typically referenced in AMF files.

```python
urn = TransformURN.parse("urn:ampas:aces:transformId:v1.5:ACESlib.Utilities.a1.0.3")
urn.transform_type        # "ACESlib"
urn.namespace             # "Utilities"   (actually the name, not a namespace)
urn.name                  # "a1.0.3"      (actually the version suffix)
urn.version_suffix        # ""
urn.aces_release_version  # None
urn.transform_version     # None
```

### `TransformInfo` (Dataclass)

Metadata for a single ACES transform:

```python
from aces.common import TransformInfo

# Fields: transform_id, user_name, transform_type, aces_version,
#         inverse_transform_id, previous_equivalent_ids
```

### Constants

```python
from aces.common import INPUT_TRANSFORM_TYPES, OUTPUT_TRANSFORM_TYPES

INPUT_TRANSFORM_TYPES   # frozenset{"IDT", "CSC", "Input"}
OUTPUT_TRANSFORM_TYPES  # frozenset{"ODT", "Output"}
```

## License

Apache-2.0
