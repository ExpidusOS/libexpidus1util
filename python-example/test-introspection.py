#!/usr/bin/env python3

# You can generate python documentation with the following commands:
# g-ir-doc-tool --language Python -o ./html ../libexpidus1util/libexpidus1util-1.0.gir
# yelp ./html/

import gi.repository
# Set the search path to use the newly generated introspection files
gi.require_version('GIRepository', '2.0')
from gi.repository import GIRepository
GIRepository.Repository.prepend_search_path('../libexpidus1util/')
# Now import 4util
gi.require_version('libexpidus1util', '1.0')
from gi.repository import libexpidus1util

# print out some stuff to see if it works
print("homedir: ", libexpidus1util.get_homedir())
print("get_dir_localized: ", libexpidus1util.get_dir_localized(libexpidus1util.get_homedir()))
print("version: ", libexpidus1util.version_string())
