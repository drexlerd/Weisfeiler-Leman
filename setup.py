
import os
import sys
import subprocess
import multiprocessing

from pathlib import Path

from setuptools import setup, find_packages, Extension
from setuptools.command.build_ext import build_ext

__version__ = "0.0.18"
HERE = Path(__file__).resolve().parent


# A CMakeExtension needs a sourcedir instead of a file list.
# The name must be the _single_ output extension from the CMake build.
# If you need multiple extensions, see scikit-build.
class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))

        # required for auto-detection of auxiliary "native" libs
        if not extdir.endswith(os.path.sep):
            extdir += os.path.sep

        build_type = "Debug" if os.environ.get('PYWL_DEBUG_BUILD') else "Release"
        print("Pywl build type:", build_type)

        temp_directory = Path.cwd() / self.build_temp
        # Create the temporary build directory, if it does not already exist
        os.makedirs(temp_directory, exist_ok=True)

        # Build dependencies
        subprocess.run(
            ["cmake", "-S", f"{ext.sourcedir}/dependencies", "-B", f"{str(temp_directory)}/dependencies/build", f"-DCMAKE_INSTALL_PREFIX={str(temp_directory)}/dependencies/installs"], cwd=str(temp_directory), check=True
        )

        subprocess.run(
            ["cmake", "--build", f"{str(temp_directory)}/dependencies/build", f"-j{multiprocessing.cpu_count()}"]
        )

        # Build dlplan
        cmake_args = [
            "-DBUILD_PYKWL=On",
            f"-DWL_VERSION_INFO={__version__}",
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            f"-DCMAKE_BUILD_TYPE={build_type}",  # not used on MSVC, but no harm
            f"-DCMAKE_PREFIX_PATH={str(temp_directory)}/dependencies/installs"
        ]
        build_args = []
        build_args += ["--target", ext.name]

        subprocess.run(
            ["cmake", "-S", ext.sourcedir, "-B", f"{str(temp_directory)}/build"] + cmake_args, cwd=str(temp_directory), check=True
        )
        subprocess.run(
            ["cmake", "--build", f"{str(temp_directory)}/build", f"-j{multiprocessing.cpu_count()}"] + build_args, cwd=str(temp_directory), check=True
        )


# The information here can also be placed in setup.cfg - better separation of
# logic and declaration, and simpler if you include description/version in a file.
setup(
    name="pykwl",
    version=__version__,
    author="Simon Stahlberg, Dominik Drexler",
    author_email="simon.stahlberg@gmail.com,dominik.drexler@liu.se",
    url="https://github.com/drexlerd/Weisfeiler-Leman",
    description="Weisfeiler-Leman library",
    long_description="",
    install_requires=["cmake>=3.21"],
    packages=find_packages(where="python/src"),
    package_dir={"": "python/src"},
    package_data={
        "": ["*.pyi"],
    },
    ext_modules=[CMakeExtension("_pykwl")],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
    extras_require={
        'test': [
            'pytest',
        ],
    }
)
