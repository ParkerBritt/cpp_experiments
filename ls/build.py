#!/usr/bin/env python

import os
import os.path
import sys
import subprocess
import shutil
import stat


def build(source_path, build_path, install_path, targets):

    def _build():
        print("Building Package")
        build_dir = os.path.join(source_path, "build")
        os.chdir(build_dir)
        subprocess.run(["cmake", ".."])
        subprocess.run(["make"])

    def _install():
        # clean
        print("Cleaning")
        print(os.listdir(install_path))
        for name in ("bin", "foo"):
            path = os.path.join(install_path, name)
            if(os.path.exists(path)):
                print("removing: ", path)
                shutil.rmtree(path)

        # install
        # find file
        print("Installing Package")
        src_path = os.path.join(source_path, "build/bin/pls")
        if(not os.path.exists(src_path)):
            raise Exception("source binary not found")

        bin_path = os.path.join(install_path, "bin")
        if(os.path.exists(bin_path)):
            print("Bin dir found")
        else:
            print("creating bin dir")
            os.mkdir(bin_path)
        dst_path = os.path.join(bin_path, "pls")
        shutil.copyfile(src_path, dst_path)
        mode = (stat.S_IRUSR | stat.S_IRGRP |
                stat.S_IXUSR | stat.S_IXGRP)
        os.chmod(dst_path, mode)

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
