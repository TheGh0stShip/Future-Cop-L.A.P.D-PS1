#!/usr/bin/env python3
"""Bridge the offline worker protocol to a localhost text-generation server."""

from __future__ import annotations

import argparse
import json
import sys
import urllib.error
import urllib.parse
import urllib.request


LOOPBACK_HOSTS = {"localhost", "127.0.0.1", "::1"}


def unwrap_json(text: str) -> str:
    text = text.strip()
    if text.startswith("```"):
        lines = text.splitlines()
        if lines and lines[0].startswith("```"):
            lines.pop(0)
        if lines and lines[-1].strip() == "```":
            lines.pop()
        text = "\n".join(lines).strip()
    value = json.loads(text)
    if not isinstance(value, dict):
        raise ValueError("generated response is not a JSON object")
    return json.dumps(value)


def prompt(request: dict) -> str:
    return (
        "Perform the requested byte-matching C reconstruction. Study the "
        "assembly oracle and improve the prior candidate using its compiler "
        "score and mismatch evidence. Emit exactly one JSON object with source, "
        "symbols, and note fields. Do not emit markdown or assembly source.\n\n"
        + json.dumps(request, indent=2, sort_keys=True)
    )


def query(url: str, model: str, request: dict, temperature: float,
          max_tokens: int, timeout: int) -> str:
    parsed = urllib.parse.urlparse(url)
    if parsed.scheme != "http" or parsed.hostname not in LOOPBACK_HOSTS:
        raise ValueError("offline mode requires an http localhost endpoint")
    body = json.dumps({
        "model": model,
        "messages": [
            {"role": "system", "content": (
                "You reconstruct maintainable ANSI C for a PlayStation matching "
                "decompilation. Return only the requested JSON object."
            )},
            {"role": "user", "content": prompt(request)},
        ],
        "temperature": temperature,
        "max_tokens": max_tokens,
        "response_format": {"type": "json_object"},
    }).encode()
    http_request = urllib.request.Request(
        url, data=body, headers={"Content-Type": "application/json"}, method="POST"
    )
    try:
        with urllib.request.urlopen(http_request, timeout=timeout) as response:
            reply = json.load(response)
    except urllib.error.HTTPError as error:
        detail = error.read().decode(errors="replace")[-2000:]
        raise RuntimeError(f"local server returned HTTP {error.code}: {detail}") from error
    try:
        content = reply["choices"][0]["message"]["content"]
    except (KeyError, IndexError, TypeError) as error:
        raise ValueError("local server response has no generated message") from error
    if not isinstance(content, str):
        raise ValueError("local server message is not text")
    return unwrap_json(content)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--url", default="http://127.0.0.1:8080/v1/chat/completions")
    parser.add_argument("--model", default="local")
    parser.add_argument("--temperature", type=float, default=0.4)
    parser.add_argument("--max-tokens", type=int, default=4096)
    parser.add_argument("--timeout", type=int, default=600)
    args = parser.parse_args()
    try:
        request = json.load(sys.stdin)
        if not isinstance(request, dict):
            raise ValueError("worker request is not a JSON object")
        print(query(args.url, args.model, request, args.temperature,
                    args.max_tokens, args.timeout))
    except (json.JSONDecodeError, OSError, RuntimeError, ValueError) as error:
        print(str(error), file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
