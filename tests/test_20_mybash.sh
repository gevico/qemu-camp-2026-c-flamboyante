#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")/../exercises/20_mybash"
./mybash ../../tests/mybash_cmd.sh
