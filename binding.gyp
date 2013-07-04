{
  'targets': [
    {
      'target_name': 'node_unqlite',
      'include_dirs': ['unqlite'],
      'sources': ['unqlite/unqlite.c', 'src/node_unqlite.cc'],
      'cflags': ['-fexceptions'],
      'cflags_cc': ['-fexceptions'],
      'cflags!': ['-fno-exceptions'],
      'cflags_cc!': ['-fno-exception'],
      'conditions': [
        ['OS=="mac"', {
            'xcode_settings': {
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
              'GCC_ENABLE_CPP_RTTI': 'YES'
            }
          }
        ],
      ],
    }
  ]
}
