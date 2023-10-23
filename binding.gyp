{
  "targets": [
    {
      "target_name": "nertc-electron-sdk",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        './nertc_sdk',
        './nertc_sdk/api',
        './nertc_sdk/nertc_sdk_Mac.framework/Headers/',
        './shared',
        './shared/libyuv/include',
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      'defines': [
        "NAPI_DISABLE_CPP_EXCEPTIONS",
        # 'NAPI_CPP_EXCEPTIONS',
        "NODE_ADDON_API_DISABLE_DEPRECATED",
        "NAPI_VERSION=4", # <=electron10.x  = 4 else 6
        # "NAPI_EXPERIMENTAL",
        # "NAPI_VERSION=6",
      ],
      "sources": [
          "./nertc_sdk_node/addon.cc", 
          './nertc_sdk_node/nertc_node_channel_event_handler.cpp',
          './nertc_sdk_node/nertc_node_channel_event_handler.h',
          "./nertc_sdk_node/nertc_node_channel.h",
          "./nertc_sdk_node/nertc_node_channel.cpp",
          "./nertc_sdk_node/nertc_node_engine.h",
          "./nertc_sdk_node/nertc_node_engine.cpp",
          './nertc_sdk_node/nertc_node_engine_helper.cpp',
          './nertc_sdk_node/nertc_node_engine_helper.h',
          './nertc_sdk_node/nertc_node_video_frame_provider.cpp',
          './nertc_sdk_node/nertc_node_video_frame_provider.h',
          './shared/sdk_helper/node_api_helper.h',
          './shared/sdk_helper/node_api_helper.cpp',
          "./shared/sdk_helper/nim_node_async_queue.h",
          "./shared/sdk_helper/nim_node_async_queue.cpp",
          './shared/sdk_helper/superfasthash.cpp',
          './nertc_sdk_node/nertc_node_engine_event_handler.h',
          './nertc_sdk_node/nertc_node_engine_event_handler.cpp',
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
          './shared/libyuv/source/video_common.cc',
          './shared/libyuv/source/scale_uv.cc',
          './shared/libyuv/source/scale_rvv.cc',
          './shared/libyuv/source/scale_rgb.cc',
          './shared/libyuv/source/scale_lsx.cc',
          './shared/libyuv/source/row_rvv.cc',
          './shared/libyuv/source/row_lsx.cc',
          './shared/libyuv/source/row_lasx.cc',
          './shared/libyuv/source/rotate_lsx.cc',
          './shared/libyuv/source/row_neon64.cc',
          './shared/libyuv/source/compare_neon.cc',
          './shared/libyuv/source/compare_neon64.cc',
          './shared/libyuv/source/scale_neon.cc',
          './shared/libyuv/source/scale_neon64.cc',
          './shared/libyuv/source/rotate_neon.cc',
          './shared/libyuv/source/rotate_neon64.cc',
          './shared/util/logger.h',
          './shared/util/logger.cpp',
          './shared/log/logging/logging.h',
          './shared/log/logging/logging.cc',
          './shared/log/logging/log_file_stream/log_file_stream.h',
          './shared/log/logging/log_file_stream/log_file_stream.cc',
          './shared/log/logging/log_file_stream/ring_buffer_mapping_file/ring_buffer_mapping_file.h',
          './shared/log/base/string_utils.h',
          './shared/log/base/string_utils.cc',
          './shared/log/base/file_utils.cc',
          './shared/log/base/file_utils.h',
          './shared/log/base/synchronization/rw_lock_wrapper.h',
          './shared/log/base/synchronization/rw_lock_wrapper.cc',
          './shared/log/base/file/platform_file.h',
          './shared/log/base/file/platform_file.cc',
          './shared/log/base/file/file.h',
          './shared/log/base/file/file.cc'
      ],
      'conditions': [
        [
          'OS=="win"',
          {
            'copies': [{
              'destination': '<(PRODUCT_DIR)',
              'files': [
                './nertc_sdk/dll/*',
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
                'RuntimeLibrary': '0',
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
              './shared/util/ConvertUTF.h',
              './shared/log/logging/log_file_stream/ring_buffer_mapping_file/ring_buffer_mapping_file_win.cc',
              './shared/log/base/synchronization/rw_lock_win.h',
              './shared/log/base/synchronization/rw_lock_win.cc',
              './shared/log/base/file/file_win.cc'
            ],
            'configurations': {
              'Release': {
                'msvs_settings': {
                  'VCCLCompilerTool': {
                    'RuntimeLibrary': '0',
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
                    'RuntimeLibrary': '0',
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
                './nertc_sdk/NERtcAiDenoise.framework',
                './nertc_sdk/NERtcAiHowling.framework',
                './nertc_sdk/NERtcAudio3D.framework',
                './nertc_sdk/NERtcBeauty.framework',
                './nertc_sdk/NERtcFaceDetect.framework',
                './nertc_sdk/NERtcFaceEnhance.framework',
                './nertc_sdk/NERtcnn.framework',
                './nertc_sdk/NERtcPersonSegment.framework',
                './nertc_sdk/NERtcScreenShareEnhance.framework',
                './nertc_sdk/NERtcSuperResolution.framework',
                './nertc_sdk/NERtcVideoDenoise.framework'
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
                'nertc_sdk_Mac.framework',
                'NERtcAiDenoise.framework',
                'NERtcAiHowling.framework',
                'NERtcAudio3D.framework',
                'NERtcBeauty.framework',
                'NERtcFaceDetect.framework',
                'NERtcFaceEnhance.framework',
                'NERtcnn.framework',
                'NERtcPersonSegment.framework',
                'NERtcScreenShareEnhance.framework',
                'NERtcSuperResolution.framework',
                'NERtcVideoDenoise.framework',
                # 'Foundation.framework',
                # 'nertc_sdk_Mac.framework',
                # 'NEFundation_Mac.framework',
                # 'NERtcBeauty.framework',
                # 'NERtcFaceDetect.framework',
                # 'NERtcnn.framework',
                # 'NERtcPersonSegment.framework',
                #'libNERTCPrivilegedTask.a',
                '-Wl,-rpath,@loader_path',
                '-ObjC'
              ]
            },
            'sources': [
              './nertc_sdk_node/NERTCPrivilegedTask.h',
              './shared/libyuv/source/compare_gcc.cc',
              './shared/libyuv/source/rotate_gcc.cc',
              './shared/libyuv/source/row_gcc.cc',
              './shared/libyuv/source/scale_gcc.cc',
              './shared/log/logging/log_file_stream/ring_buffer_mapping_file/ring_buffer_mapping_file_posix.cc',
              './shared/log/base/synchronization/rw_lock_posix.h',
              './shared/log/base/synchronization/rw_lock_posix.cc',
              './shared/log/base/file/file_posix.cc',
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
      ],
    }
  ]
}
