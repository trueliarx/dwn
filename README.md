# dwn

A tiny, dependency-free HTTPS downloader for Windows. Written in plain C with WinINet, built to be as small as possible (3-4kb).

## Usage

```
dwn.exe <URL> <OutputFile>
```

**Example:**

```
dwn.exe https://example.com/file.zip file.zip
```

- `<URL>` — the HTTP/HTTPS URL to download.
- `<OutputFile>` — the local path to save the downloaded content to.

If the download succeeds, `dwn` prints `Download complete.` and exits with code `0`. On failure, it prints `Download failed.` and exits with code `1`.

## Features

- Single self-contained `.exe`, no installer, no runtime dependencies beyond what ships with Windows.
- Follows HTTP redirects, including cross-protocol redirects (HTTP → HTTPS and back).
- No CRT (C runtime) — built with a custom entry point for minimal binary size.
- No external command-line parsing libraries (`shell32.lib` free) — arguments are parsed manually.

## Building from source

Run build.bat

MSVC / CL is needed

## Security notes
- To be considered unsafe if downloading from any malicious server!
- `/GS-` and `/DYNAMICBASE:NO` disable stack-cookie protection and ASLR respectively. This is a deliberate size/security tradeoff intended for a small, trusted, personal-use CLI tool — **not recommended** for binaries that process untrusted or adversarial input, or that will be widely distributed.
- `dwn` does not verify TLS certificates beyond what WinINet does by default, and does not validate downloaded content (checksums, signatures). Verify anything you download from an untrusted source before running it.
