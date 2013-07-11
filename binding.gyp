{
  'conditions': [
      ['OS=="win"', {
        'variables': {
          'copy_command%': 'copy',
        },
      },{
        'variables': {
          'copy_command%': 'cp',
        },
      }]
  ],
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
    },
    {
      'target_name': 'action_after_build',
      'type': 'none',
      'dependencies': [ 'node_unqlite' ],
      'actions': [
        {
          'action_name': 'move_node_module',
          'inputs': [
            '<@(PRODUCT_DIR)/node_unqlite.node'
          ],
          'outputs': [
            'lib/node_unqlite.node'
          ],
          'action': ['<@(copy_command)', '<@(PRODUCT_DIR)/node_unqlite.node', 'lib/node_unqlite.node']
        }
      ]
    }
  ]
}
