#!/usr/bin/env python

import os
import os.path
import sys
import subprocess


def build(source_path, build_path, install_path, targets):

    def _build():
        print("Building Package")
        build_dir = os.path.join(source_path, "build")
        os.chdir(build_dir)
        subprocess.run(["cmake", ".."])
        subprocess.run(["make"])

    def _install():
        # install
        pass

    _build()

    if "install" in (targets or []):
        _install()


if __name__ == '__main__':
    build(
        source_path=os.environ['REZ_BUILD_SOURCE_PATH'],
        build_path=os.environ['REZ_BUILD_PATH'],
        install_path=os.environ['REZ_BUILD_INSTALL_PATH'],
        targets=sys.argv[1:]
    )
