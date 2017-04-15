{
  'targets': [
    {
      'target_name': 'node_unqlite',
      'include_dirs': ['unqlite', "<!(node -e \"require('nan')\")"],
      'sources': ['unqlite/unqlite.c', 'src/node_unqlite_async.cc', 'src/node_unqlite.cc'],
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
    },
    {
      'target_name': 'action_after_build',
      'type': 'none',
      'dependencies': [ 'node_unqlite' ],
      'copies': [
        {
          'destination': 'lib',
          'files': [
            '<@(PRODUCT_DIR)/node_unqlite.node'
          ]
        }
      ]
    }
  ]
}
