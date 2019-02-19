#!/usr/bin/python
from distutils.core import setup, Extension

module = Extension("QCrypt",sources = ["QCrypt.c"])
setup(name = "QCryptModule",
        version = "0.0",
        description = "Query Encryption",
        ext_modules = [module])
