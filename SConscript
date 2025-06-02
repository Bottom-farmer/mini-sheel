import os
from building import *

src     = Glob('src/*.c') + Glob('platform/rt_thread/*.c')
CPPPATH          = [os.path.join(GetCurrentDir(), 'inc')]

group = DefineGroup('mini-sheel', src, depend = ['PKG_USING_MINI_SHEEL'], CPPPATH = CPPPATH)

Return('group')
