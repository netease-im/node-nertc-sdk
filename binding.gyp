{
  "targets": [
    {
      'target_name': 'nertc-sdk',
      'include_dirs': [
      './shared',
      './shared/libyuv/include',
      './nertc_sdk',
      './nertc_sdk/api'
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
            'defines': [
              'WIN32',
              'WIN32_LEAN_AND_MEAN'
            ],
            'library_dirs': [
              './nertc_sdk/libs/windows/x86/'
            ],
            'link_settings': {
              'libraries': [
                '-lnertc_sdk.lib',
                '-lWs2_32.lib'
              ]
            },
            'msvs_settings': {
              'VCCLCompilerTool': {
                # 'WarningLevel': '3',
                # 'DisableSpecificWarnings': ['4819'],
                # 'WarnAsError': 'false',
                # 'ExceptionHandling': '0',
                'AdditionalOptions': [
                  # '/EHsc',
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
              './shared/libyuv/source/scale_win.cc'
            ],
            'configurations': {
              'Release': {
                'msvs_settings': {
                  'VCCLCompilerTool': {
                    # 多线程 DLL (/MD)
                    'RuntimeLibrary': '2',
                    # 完全优化 /Os
                    'Optimization': '2',
                    # 使用内部函数 /Oi
                    'EnableIntrinsicFunctions': 'true',
                    # 程序数据库 (/Zi)
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
                    # 'WarningLevel': '3',
                    # 'DisableSpecificWarnings': ['4819'],
                    # 'WarnAsError': 'false',
                    # 'ExceptionHandling': '0',
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
            'defines': [
            ],
            'mac_framework_dirs': [
              '../nertc_sdk/bin/darwin'
            ],
            'library_dirs': [
              '../nertc_sdk/bin/darwin'
            ],
            'link_settings': {
              'libraries': [
                'Foundation.framework',
                'nertc_sdk_Mac.framework',
                'NEFundation_Mac.framework',
                '-rpath ./macsdk/'
                ]
            }, 
            'sources': [
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
              ],
              'DEBUG_INFORMATION_FORMAT': 'dwarf-with-dsym',
              'OTHER_CFLAGS': [
              ],
            }# xcode_settings 
          }
        ]
      ]
    }
  ]
}