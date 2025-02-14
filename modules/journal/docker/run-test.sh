#!/bin/bash

# Runs a test container of the Journal module.
#
# To build an image for it first run ./modules/journal/docker/build-test.sh (from project root).
#
# Usage:
#
# ./modules/journal/docker/run-test.sh [--with-dependency-tests]
#

set -x # Print commands as they run.

docker run \
  --rm \
  --tty \
  --interactive \
  taconez/journal:test
set +x
