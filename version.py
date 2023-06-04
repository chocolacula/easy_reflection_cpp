import sys
import os
import subprocess
import json
import argparse
import enum

from sys import stderr
from sys import stdout

from typing import Optional
from typing import Tuple

DVERSION = "0.0.0"
DFILE = "vcpkg.json"
INDENT = 2

parser = argparse.ArgumentParser(description="version manipulating utility")
sub = parser.add_subparsers(dest="command")

# fmt: off
pget = sub.add_parser(
    "get",
    description=f"print current version from the file or {DVERSION} in case of errors",
)
pget.add_argument(
    "-f", "--file",
    help="specify manifest file"
)

pset = sub.add_parser(
    "set",
    description="write version to the file"
)
pset.add_argument(
    "version",
    help="semantic version in <major.minor.patch> format",
    type=str
)
pset.add_argument(
    "-t", "--tag",
    help="add a tag with version",
    action="store_true"
)
pset.add_argument(
    "-p", "--push",
    help="push changes",
    action="store_true"
)
pset.add_argument(
    "-f", "--file",
    help="specify manifest file"
)

version_components = ["major", "minor", "patch"]

pup = sub.add_parser(
    "up",
    description="up version"
)
pup.add_argument(
    "component",
    help="component of semantic version",
    type=str,
    choices=version_components,
)
pup.add_argument(
    "-t", "--tag",
    help="add a tag with version",
    action="store_true"
)
pup.add_argument(
    "-p", "--push",
    help="push changes",
    action="store_true"
)
pup.add_argument(
    "-f", "--file",
    help="specify manifest file"
)

pdown = sub.add_parser(
    "down",
    description="down version"
)
pdown.add_argument(
    "component",
    help="component of semantic version",
    type=str,
    choices=version_components,
)
pdown.add_argument(
    "-t", "--tag",
    help="add a tag with version",
    action="store_true"
)
pdown.add_argument(
    "-p", "--push",
    help="push changes",
    action="store_true"
)
pdown.add_argument(
    "-f", "--file",
    help="specify manifest file"
)
# fmt: on


def parse_version(string) -> Optional[Tuple[int, int, int]]:
    chunks = string.split(".")

    if len(chunks) != 3:
        stderr.write(f"version {string} is invalid\n")
        return None

    try:
        major = int(chunks[0])
        minor = int(chunks[1])
        patch = int(chunks[2])
    except ValueError:
        stderr.write(f"version {string} is invalid\n")
        return None

    if major < 0 or minor < 0 or patch < 0:
        stderr.write(f"version {string} is invalid\n")
        return None

    return (major, minor, patch)


class Context:
    def __init__(self, filepath):
        if filepath == None:
            filepath = os.path.dirname(os.path.realpath(__file__))
            filepath = os.path.join(filepath, DFILE)

        if os.path.exists(filepath):
            self.file = open(filepath, "r+")
            self.data = json.load(self.file)
        else:
            self.data = {}
            stderr.write("Manifest file doesn't exist\n")

    def write_version(self, version: str):
        self.data["version"] = version

        self.file.seek(0)
        self.file.truncate()
        json.dump(self.data, self.file, indent=INDENT)
        self.file.write("\n") # trailing line
        self.file.close()

    def get_version(self) -> Optional[Tuple[int, int, int]]:
        if len(self.data) == 0:
            return None

        if "version" in self.data:
            return parse_version(self.data["version"])
        else:
            stderr.write("Cannot find 'version' field in the file\n")
            return None

    def push(self, message, tag):
        subprocess.Popen(["git", "add", "vcpkg.json"]).wait()
        subprocess.Popen(["git", "commit", "-m", message]).wait()
        subprocess.Popen(["git", "push"]).wait()

        if tag:
            subprocess.Popen(["git", "tag", self.data["version"]]).wait()
            subprocess.Popen(["git", "push", "--tags"]).wait()


args = parser.parse_args()
ctx = Context(args.file)


def main(ctx, args):
    if args.command == "get":
        ver = ctx.get_version()

        if ver == None:
            stdout.write(DVERSION)
        else:
            stdout.write(".".join(map(str, ver)))

    elif args.command == "set":
        ver = parse_version(args.version)

        if ver == None:
            return

        v = ".".join(map(str, ver))
        ctx.write_version(v)
        if args.push:
            ctx.push(f"set version to {v}", args.tag)

    elif args.command == "up":
        ver = ctx.get_version()

        if ver == None:
            return

        major, minor, patch = ver

        if args.component == "major":
            major += 1
            minor, patch = 0, 0
        elif args.component == "minor":
            minor += 1
            patch = 0
        elif args.component == "patch":
            patch += 1

        v = f"{major}.{minor}.{patch}"
        ctx.write_version(v)

        if args.push:
            o = ".".join(map(str, ver))
            ctx.push(f"version up from {o} to {v}", args.tag)

    elif args.command == "down":
        ver = ctx.get_version()

        if ver == None:
            return

        major, minor, patch = ver

        if args.component == "major":
            major -= 1
        elif args.component == "minor":
            minor -= 1
        elif args.component == "patch":
            patch -= 1

        v = f"{major}.{minor}.{patch}"
        if major < 0 or minor < 0 or patch < 0:
            stderr.write(f"A version component cannot be negative {v}\n")
            return

        ctx.write_version(v)
        if args.push:
            o = ".".join(map(str, ver))
            ctx.push(f"version down from {o} to {v}", args.tag)


if __name__ == "__main__":
    main(ctx, args)
