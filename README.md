<!-- SPDX-License-Identifier: Apache-2.0 -->
<!-- Copyright Contributors to the ACES Project. -->

# ACES Core Transforms

[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
[![CLA
assistant](https://cla-assistant.io/readme/badge/aces-aswf/aces-core)](https://cla-assistant.io/aces-aswf/aces-core)

This repository houses core CTL library functions used by the ACES system. These
functions provide the foundational building blocks upon which higher-level ACES
transforms are constructed, including:

- Rendering algorithms used in ACES Output Transforms (e.g., tonescale, chroma
  compress, gamut compress)

- Utility functions for common operations (e.g., clamping, matrix math)

- Fundamental color science operations (e.g., transfer functions, matrix
  conversions, chromatic adaptation)

> [!IMPORTANT] 
> This repository was named **`aces-dev`** in versions of ACES prior to the 2.0
release, and preserves the commit history for those earlier ACES versions. <br>
Use the [tag history](https://github.com/aces-aswf/aces-core/tags) to access specific
ACES versions from before v2.0.

## Contributing

Before the project can accept any code submissions through GitHub, you must fulfill these prerequisites:

1. **Contributor License Agreement (CLA):** All contributors **must** have a signed CLA on file to ensure the project can freely use your contributions.

2. **Developer Certificate of Origin (DCO):** All commits **must** be signed off (e.g., `git commit -s`) to verify that you have the right to submit the code.

3. **AI Assistance Disclosure:** While not currently blocked by CI checks, any commits or PRs built with AI assistance are expected to include an `Assisted-by: TOOL/MODEL` line to maintain transparency and human accountability.

Please see [Contributing Guidelines](https://github.com/aces-aswf/.github/blob/main/CONTRIBUTING.md) for more details.

## Reporting Issues

### General Issues
To report a problem with any transforms in ACES core, please open an
[issue](https://github.com/aces-aswf/aces-core/issues) in this repository.

### Security
If the issue is sensitive in nature or a security related issue, please do not
report in the issue tracker. Instead refer to [SECURITY](https://github.com/aces-aswf/.github/blob/main/SECURITY.md) for more information about the project security policy.

## Governance

This repository is part of ACES, a project governed by the Academy Software Foundation.

For details about how the ACES project operates, please see
[GOVERNANCE](https://github.com/aces-aswf/.github/blob/main/GOVERNANCE.md).

## License

The ACES Project is licensed under the [Apache 2.0 license](./LICENSE).