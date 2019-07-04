
env = Environment(
    CPPPATH=['#nanopb', '#src', '#']
  , CCFLAGS = ['-DDEBUG']
  , CXXFLAGS = ['-std=c++17']
  , CC = "clang"
  , CXX = "clang++"
)

targets = env.SConscript(
  '#src/SConscript',
  exports='env',
  variant_dir='build1'
)

Alias('all', targets)
Default('all')

