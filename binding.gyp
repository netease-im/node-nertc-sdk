{
  "targets": [
    {
      'target_name': 'nertc-electron-sdk',
      'include_dirs': [
      './shared',
      './shared/libyuv/include',
      './nertc_sdk',
      './nertc_sdk/api',
      './nertc_sdk/nertc_sdk_Mac.framework/Headers/'
      ],
      'sources': [
        './nertc_sdk_node/nertc_node_engine.cpp',
        './nertc_sdk_node/nertc_node_engine.h',
        './nertc_sdk_node/nertc_node_engine_helper.cpp',
        './nertc_sdk_node/nertc_node_engine_helper.h',
        './nertc_sdk_node/nertc_node_engine_event_handler.cpp',
        './nertc_sdk_node/nertc_node_engine_event_handler.h',
        './nertc_sdk_node/nertc_node_ext.cpp',
        './nertc_sdk_node/nertc_node_ext.h',
        './nertc_sdk_node/nertc_node_video_frame_provider.cpp',
        './nertc_sdk_node/nertc_node_video_frame_provider.h',
        './shared/sdk_helper/nim_node_helper.h',
        './shared/sdk_helper/nim_node_helper.cpp',
        './shared/sdk_helper/nim_node_async_queue.h',
        './shared/sdk_helper/nim_node_async_queue.cpp',
        './shared/sdk_helper/nim_event_handler.h',
        './shared/sdk_helper/nim_event_handler.cpp',
        './shared/sdk_helper/superfasthash.cpp',
        './shared/libyuv/source/compare_common.cc',
        './shared/libyuv/source/compare.cc',
        './shared/libyuv/source/convert_argb.cc',
        './shared/libyuv/source/convert_from_argb.cc',
        './shared/libyuv/source/convert_from.cc',
        './shared/libyuv/source/convert_jpeg.cc',
        './shared/libyuv/source/convert_to_argb.cc',
        './shared/libyuv/source/convert_to_i420.cc',
        './shared/libyuv/source/convert.cc',
        './shared/libyuv/source/cpu_id.cc',
        './shared/libyuv/source/mjpeg_decoder.cc',
        './shared/libyuv/source/mjpeg_validate.cc',
        './shared/libyuv/source/planar_functions.cc',
        './shared/libyuv/source/rotate_any.cc',
        './shared/libyuv/source/rotate_argb.cc',
        './shared/libyuv/source/rotate_common.cc',
        './shared/libyuv/source/rotate.cc',
        './shared/libyuv/source/row_any.cc',
        './shared/libyuv/source/row_common.cc',
        './shared/libyuv/source/scale_any.cc',
        './shared/libyuv/source/scale_argb.cc',
        './shared/libyuv/source/scale_common.cc',
        './shared/libyuv/source/scale.cc',
        './shared/libyuv/source/video_common.cc'
      ],
      'conditions': [
        [
          'OS=="win"',
          {
            'copies': [{
              'destination': '<(PRODUCT_DIR)',
              'files': [
                './nertc_sdk/dll/nertc_sdk.dll',
                './nertc_sdk/dll/protoopp.dll',
                './nertc_sdk/dll/SDL2.dll',
                './nertc_sdk/dll/libfreetype-6.dll',
                './nertc_sdk/dll/libjpeg-9.dll',
                './nertc_sdk/dll/libpng16-16.dll',
                './nertc_sdk/dll/libtiff-5.dll',
                './nertc_sdk/dll/libwebp-7.dll',
                './nertc_sdk/dll/SDL2_image.dll',
                './nertc_sdk/dll/SDL2_ttf.dll',
                './nertc_sdk/dll/zlib1.dll',
              ]
            }],
            'defines': [
              'WIN32',
              'WIN32_LEAN_AND_MEAN'
            ],
            'library_dirs': [
              './nertc_sdk/lib/'
            ],
            'link_settings': {
              'libraries': [
                '-lnertc_sdk.lib',
                '-lWs2_32.lib'
              ]
            },
            'msvs_settings': {
              'VCCLCompilerTool': {
                'AdditionalOptions': [
                  '/utf-8'
                ]
              }
            },
            'defines!': [
              '_USING_V110_SDK71_',
              '_HAS_EXCEPTIONS=0'
            ],
            'sources': [
              './shared/libyuv/source/compare_win.cc',
              './shared/libyuv/source/rotate_win.cc',
              './shared/libyuv/source/row_win.cc',
              './shared/libyuv/source/scale_win.cc',
              './shared/util/windows_helper.h',
              './shared/util/windows_helper.cpp',   
              './shared/util/string_util.h',
              './shared/util/string_util.cpp',    
              './shared/util/ConvertUTF.c',
              './shared/util/ConvertUTF.h'
            ],
            'configurations': {
              'Release': {
                'msvs_settings': {
                  'VCCLCompilerTool': {
                    'RuntimeLibrary': '2',
                    'Optimization': '2',
                    'EnableIntrinsicFunctions': 'true',
                    'DebugInformationFormat': '3',
                    'AdditionalOptions': [
                    ]
                  }
                },
              },
              'Debug': {
                'msvs_settings': {
                  'VCCLCompilerTool': {
                    'RuntimeLibrary': '3',
                    'AdditionalOptions': [
                    ]
                  }
                },
              }
            }
          }
        ],
        [
          'OS=="mac"',
          {
            'copies': [{
              'destination': '<(PRODUCT_DIR)',
              'files': [
                './nertc_sdk/nertc_sdk_Mac.framework',
                './nertc_sdk/NEFundation_Mac.framework'
              ]
            }],
            'defines': [
            ],
            'mac_framework_dirs': [
              '../nertc_sdk/'
            ],
            'library_dirs': [
              '../nertc_sdk/'
            ],
            'link_settings': {
              'libraries': [
                'Foundation.framework',
                'nertc_sdk_Mac.framework',
                'NEFundation_Mac.framework',
                '-Wl,-rpath,@loader_path',
                '-ObjC'
              ]
            },
            'sources': [
              './nertc_sdk_node/NERTCPrivilegedTask.h',
              './shared/libyuv/source/compare_gcc.cc',
              './shared/libyuv/source/rotate_gcc.cc',
              './shared/libyuv/source/row_gcc.cc',
              './shared/libyuv/source/scale_gcc.cc'
            ],
            'xcode_settings': {
              'ARCHS': [ 'x86_64' ],
              'MACOSX_DEPLOYMENT_TARGET': '10.14',
              'EXCUTABLE_EXTENSION': 'node',
              'FRAMEWORK_SEARCH_PATHS': [
                './nertc_sdk/bin/darwin'
              ],
              'DEBUG_INFORMATION_FORMAT': 'dwarf-with-dsym',
              'OTHER_CFLAGS': [
              ],
            }
          }
        ]
      ]
    }
  ]
}