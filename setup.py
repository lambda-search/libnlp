import os
import re
import subprocess
import sys
import warnings

import setuptools
import setuptools.command.build_ext
import wheel.bdist_wheel

_this_dir = os.path.dirname(os.path.abspath(__file__))
_clib_dir = os.path.join(_this_dir, 'python', 'libnlp','clib')
_build_dir = os.path.join(_this_dir, 'build', 'python')

_cmake_file = os.path.join(_this_dir, 'CMakeLists.txt')
_author_file = os.path.join(_this_dir, 'AUTHORS')
_readme_file = os.path.join(_this_dir, 'README.md')

try:
    sys.path.insert(0, os.path.join(_this_dir, 'python'))

    import libnlp.normalize_cc  # noqa
    _libnlpcc_built = True
except ImportError:
    _libnlpcc_built = False


def get_version_info():
    version_info = ['0', '1', '5']
    version_pattern = re.compile(
        r'NLPCC_VERSION_(MAJOR|MINOR|REVISION) (\d+)')
    with open(_cmake_file, 'rb') as f:
        for l in f:
            match = version_pattern.search(l.decode('utf-8'))
            if not match:
                continue
            if match.group(1) == 'MAJOR':
                version_info[0] = match.group(2)
            elif match.group(1) == 'MINOR':
                version_info[1] = match.group(2)
            elif match.group(1) == 'REVISION':
                version_info[2] = match.group(2)
    version = '.'.join(version_info)
    return version


def get_author_info():
    if not os.path.isfile(_author_file):
        return 'jeff.li', 'lijippy@163.com'

    authors = []
    emails = []
    author_pattern = re.compile(r'(.+) <(.+)>')
    with open(_author_file, 'rb') as f:
        for line in f:
            match = author_pattern.search(line.decode('utf-8'))
            if not match:
                continue
            authors.append(match.group(1))
            emails.append(match.group(2))

    if len(authors) == 0:
        return 'jeff.li', 'lijippy@163.comm'

    return ', '.join(authors), ', '.join(emails)

def get_long_description():
    with open(_readme_file, 'rb') as f:
        return f.read().decode('utf-8')


version_info = get_version_info()
author_info = get_author_info()

def build_libnlpcc():
    if _libnlpcc_built:
        return  # Skip building binary file
    print('building libnlp.cc into %s' % _build_dir)

    is_windows = sys.platform == 'win32'

    # Make build directories
    if is_windows:
        subprocess.call('md {}'.format(_build_dir), shell=True)
        subprocess.call('md {}'.format(_clib_dir), shell=True)
    else:
        subprocess.call('mkdir -p {}'.format(_build_dir), shell=True)
        subprocess.call('mkdir -p {}'.format(_clib_dir), shell=True)

    # Configure
    cmake_args = [
        '-DENABLE_BENCHMARK:BOOL=OFF',
        '-DENABLE_TESTING:BOOL=OFF',
        '-DENABLE_PYTHON:BOOL=ON',
        '-DCMAKE_BUILD_TYPE=Release',
        '-DPKG_VERSION={}'.format(version_info),
        '-DCMAKE_INSTALL_PREFIX={}'.format(_clib_dir),
        '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={}'.format(_clib_dir),
        '-DPYTHON_EXECUTABLE={}'.format(sys.executable),
    ]

    if is_windows:
        cmake_args += \
            ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE={}'.format(_clib_dir)]
        if sys.maxsize > 2**32:
            cmake_args += ['-A', 'x64']

    cmd = ['cmake', '-B', _build_dir] + cmake_args
    errno = subprocess.call(cmd)
    assert errno == 0, 'Configure failed'

    # Build
    cmd = [
        'cmake', '--build', _build_dir,
        '--config', 'Release',
        '--target', 'install'
    ]
    errno = subprocess.call(cmd)
    assert errno == 0, 'Build failed'

    # Empty __init__.py file has to be created
    # to make libnlp.cc.clib a module
    with open('{}/__init__.py'.format(_clib_dir), 'w'):
        pass


class NlpCCExtension(setuptools.Extension, object):
    def __init__(self, name, sourcedir=''):
        setuptools.Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class BuildExtCommand(setuptools.command.build_ext.build_ext, object):
    def build_extension(self, ext):
        if isinstance(ext, NlpCCExtension):
            build_libnlpcc()
        else:
            super(BuildExtCommand, self).build_extension(ext)


class BDistWheelCommand(wheel.bdist_wheel.bdist_wheel, object):
    """Custom bdsit_wheel command that will change
    default plat-name based on PEP 425 and PEP 513
    """

    @staticmethod
    def _determine_platform_tag():
        if sys.platform == 'win32':
            if 'amd64' in sys.version.lower():
                return 'win-amd64'
            return sys.platform

        if sys.platform == 'darwin':
            _, _, _, _, machine = os.uname()
            return 'macosx-10.14-{}'.format(machine)

        if os.name == 'posix':
            _, _, _, _, machine = os.uname()
            return 'manylinux1-{}'.format(machine)

        warnings.warn(
            'Windows macos and linux are all not detected, '
            'Proper distribution name cannot be determined.')
        from distutils.util import get_platform
        return get_platform()

    def initialize_options(self):
        super(BDistWheelCommand, self).initialize_options()
        self.plat_name = self._determine_platform_tag()


packages = ['libnlp', 'libnlp.normalize', 'libnlp.normalize.cc', 'libnlp.clib']


setuptools.setup(
    name='libnlp',
    version=version_info,
    author=author_info[0],
    author_email=author_info[1],
    description=" conversion between Traditional and Simplified Chinese",
    long_description=get_long_description(),
    long_description_content_type="text/markdown",
    url="https://github.com/lambda-search/libnlp",

    packages=packages,
    package_dir={'libnlp': 'python/libnlp'},
    package_data={str('libnlp'): [
        'clib/normalize_cc*',
        'clib/share/libnlp/normalize/*',
    ]},
    ext_modules=[NlpCCExtension('libnlp.clib.normalize_cc', 'python')],
    cmdclass={
        'build_ext': BuildExtCommand,
        'bdist_wheel': BDistWheelCommand
    },

    classifiers=[
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'Intended Audience :: Science/Research',
        'Natural Language :: Chinese (Simplified)',
        'Natural Language :: Chinese (Traditional)',
        'Programming Language :: Python',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: Apache Software License',
        'Topic :: Scientific/Engineering',
        'Topic :: Software Development',
        'Topic :: Software Development :: Libraries',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: Software Development :: Localization',
        'Topic :: Text Processing :: Linguistic',
    ],
    license='Apache License 2.0',
    keywords=['libnlp', 'nlp', 'normalize', 'convert', 'chinese']
)
