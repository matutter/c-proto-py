
env = Environment(
    CPPPATH=['#nanopb', '#src']
  , CCFLAGS = ['-DDEBUG', '-DPB_ENABLE_MALLOC=1']
)

targets = env.SConscript(
  '#src/SConscript',
  exports='env',
  variant_dir='build1'
)

Alias('all', targets)
Default('all')

