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
        './shared',
        './shared/libyuv/include',
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      'defines': [
        "NAPI_DISABLE_CPP_EXCEPTIONS",
        "NODE_ADDON_API_DISABLE_DEPRECATED",
        "NAPI_VERSION=4"
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
               './shared/log/logging/logging.cc',
               './shared/log/logging/log_file_stream/log_file_stream.h',
               './shared/log/logging/log_file_stream/log_file_stream.cc',
               './shared/log/logging/log_file_stream/ring_buffer_mapping_file/ring_buffer_mapping_file.h',
               './shared/log/logging/log_file_stream/ring_buffer_mapping_file/ring_buffer_mapping_file_win.cc',
               './shared/log/base/string_utils.h',
               './shared/log/base/string_utils.cc',
               './shared/log/base/file_utils.cc',
               './shared/log/base/file_utils.h',
               './shared/log/base/synchronization/rw_lock_wrapper.h',
               './shared/log/base/synchronization/rw_lock_wrapper.cc',
               './shared/log/base/synchronization/rw_lock_win.h',
               './shared/log/base/synchronization/rw_lock_win.cc',
               './shared/log/base/file/platform_file.h',
               './shared/log/base/file/platform_file.cc',
               './shared/log/base/file/file.h',
               './shared/log/base/file/file.cc',
               './shared/log/base/file/file_win.cc',
               './shared/util/logger.h',
               './shared/util/logger.cpp'
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
                './nertc_sdk/nertc_sdk_Mac.xcframework',
                './nertc_sdk/NERtcAiDenoise.xcframework',
                './nertc_sdk/NERtcAiHowling.xcframework',
                './nertc_sdk/NERtcAudio3D.xcframework',
                './nertc_sdk/NERtcBeauty.xcframework',
                './nertc_sdk/NERtcFaceDetect.xcframework',
                './nertc_sdk/NERtcFaceEnhance.xcframework',
                './nertc_sdk/NERtcnn.xcframework',
                './nertc_sdk/NERtcPersonSegment.xcframework',
                './nertc_sdk/NERtcScreenShareEnhance.xcframework',
                './nertc_sdk/NERtcSuperResolution.xcframework',
                './nertc_sdk/NERtcVideoDenoise.xcframework'
              ]
            }],
            'defines': [
            ],
            'include_dirs': [
              './nertc_sdk/nertc_sdk_Mac.xcframework/macos-arm64_x86_64/nertc_sdk_Mac.framework/Headers/'
            ],
            'library_dirs': [
              './nertc_sdk/nertc_sdk_Mac.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcAiDenoise.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcAiHowling.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcAudio3D.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcBeauty.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcFaceDetect.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcFaceEnhance.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcnn.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcPersonSegment.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcScreenShareEnhance.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcSuperResolution.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcVideoDenoise.xcframework/macos-arm64_x86_64'
            ],
            'mac_framework_dirs': [
              './nertc_sdk/nertc_sdk_Mac.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcAiDenoise.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcAiHowling.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcAudio3D.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcBeauty.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcFaceDetect.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcFaceEnhance.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcnn.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcPersonSegment.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcScreenShareEnhance.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcSuperResolution.xcframework/macos-arm64_x86_64',
              './nertc_sdk/NERtcVideoDenoise.xcframework/macos-arm64_x86_64'
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
               './shared/log/logging/logging.cc',
               './shared/log/logging/log_file_stream/log_file_stream.h',
               './shared/log/logging/log_file_stream/log_file_stream.cc',
               './shared/log/logging/log_file_stream/ring_buffer_mapping_file/ring_buffer_mapping_file.h',
               './shared/log/logging/log_file_stream/ring_buffer_mapping_file/ring_buffer_mapping_file_posix.cc',
               './shared/log/base/string_utils.h',
               './shared/log/base/string_utils.cc',
               './shared/log/base/file_utils.cc',
               './shared/log/base/file_utils.h',
               './shared/log/base/synchronization/rw_lock_wrapper.h',
               './shared/log/base/synchronization/rw_lock_wrapper.cc',
               './shared/log/base/synchronization/rw_lock_posix.h',
               './shared/log/base/synchronization/rw_lock_posix.cc',
               './shared/log/base/file/platform_file.h',
               './shared/log/base/file/platform_file.cc',
               './shared/log/base/file/file.h',
               './shared/log/base/file/file.cc',
               './shared/log/base/file/file_posix.cc',
               './shared/util/logger.h',
               './shared/util/logger.cpp'
             ],
            'xcode_settings': {
              'ARCHS': [ 'x86_64', 'arm64' ],
              'MACOSX_DEPLOYMENT_TARGET': '10.15',
              'EXECUTABLE_EXTENSION': 'node',
              'FRAMEWORK_SEARCH_PATHS': [
                '$(inherited)',
                '$(SRCROOT)/build/Release',
                '$(SRCROOT)/build/Release/nertc_sdk_Mac.xcframework/macos-arm64_x86_64',
                '$(SRCROOT)/build/Release/NERtcAiDenoise.xcframework/macos-arm64_x86_64',
                '$(SRCROOT)/build/Release/NERtcAiHowling.xcframework/macos-arm64_x86_64',
                '$(SRCROOT)/build/Release/NERtcAudio3D.xcframework/macos-arm64_x86_64',
                '$(SRCROOT)/build/Release/NERtcBeauty.xcframework/macos-arm64_x86_64',
                '$(SRCROOT)/build/Release/NERtcFaceDetect.xcframework/macos-arm64_x86_64',
                '$(SRCROOT)/build/Release/NERtcFaceEnhance.xcframework/macos-arm64_x86_64',
                '$(SRCROOT)/build/Release/NERtcnn.xcframework/macos-arm64_x86_64',
                '$(SRCROOT)/build/Release/NERtcPersonSegment.xcframework/macos-arm64_x86_64',
                '$(SRCROOT)/build/Release/NERtcScreenShareEnhance.xcframework/macos-arm64_x86_64',
                '$(SRCROOT)/build/Release/NERtcSuperResolution.xcframework/macos-arm64_x86_64',
                '$(SRCROOT)/build/Release/NERtcVideoDenoise.xcframework/macos-arm64_x86_64'
              ],
              'DEBUG_INFORMATION_FORMAT': 'dwarf-with-dsym',
              'OTHER_CFLAGS': [
                '-fvisibility=hidden'
              ],
              'CODE_SIGN_IDENTITY': '-',
              'CODE_SIGNING_REQUIRED': 'NO',
              'CODE_SIGNING_ALLOWED': 'NO',
              "LD_RUNPATH_SEARCH_PATHS": [
                 "@loader_path",
                 "@loader_path/nertc_sdk_Mac.xcframework/macos-arm64_x86_64",
                 "@loader_path/NERtcAiDenoise.xcframework/macos-arm64_x86_64",
                 "@loader_path/NERtcAiHowling.xcframework/macos-arm64_x86_64",
                 "@loader_path/NERtcAudio3D.xcframework/macos-arm64_x86_64",
                 "@loader_path/NERtcBeauty.xcframework/macos-arm64_x86_64",
                 "@loader_path/NERtcFaceDetect.xcframework/macos-arm64_x86_64",
                 "@loader_path/NERtcFaceEnhance.xcframework/macos-arm64_x86_64",
                 "@loader_path/NERtcnn.xcframework/macos-arm64_x86_64",
                 "@loader_path/NERtcPersonSegment.xcframework/macos-arm64_x86_64",
                 "@loader_path/NERtcScreenShareEnhance.xcframework/macos-arm64_x86_64",
                 "@loader_path/NERtcSuperResolution.xcframework/macos-arm64_x86_64",
                 "@loader_path/NERtcVideoDenoise.xcframework/macos-arm64_x86_64"
               ]
            }
          }
        ],
        [
          'OS=="linux"',
          {
            'defines': [
              'LINUX',
              '_GNU_SOURCE',
              '_REENTRANT'
            ],
            'link_settings': {
              'libraries': [
                '-lnertc_sdk',
                '-lpthread',
                '-ldl',
                '-lrt',
                '-lm'
              ]
            },
            'ldflags': [
              '-Wl,-rpath,$ORIGIN',
              '-Wl,-rpath,$ORIGIN/../'
            ],
            'sources': [
              './shared/libyuv/source/compare_gcc.cc',
              './shared/libyuv/source/rotate_gcc.cc',
              './shared/libyuv/source/row_gcc.cc',
              './shared/libyuv/source/scale_gcc.cc',
              './shared/log/logging/logging.h',
              './shared/log/logging/logging_linux.cc',
              './shared/util/logger.h',
              './shared/util/logger.cpp'
            ],
            'cflags': [
              '-fPIC',
              '-fvisibility=hidden',
              '-Wall',
              '-Wextra'
            ],
            'cflags_cc': [
              '-fPIC',
              '-fvisibility=hidden',
              '-Wall',
              '-Wextra'
            ],
            'cflags!': [
              '-std=gnu++20'
            ],
            'cflags_cc!': [
              '-std=gnu++20'
            ],
            'make_global_settings': [
              ['CXX', 'g++'],
              ['CC', 'gcc']
            ],
            'conditions': [
              [
                'target_arch=="x64"',
                {
                  'copies': [
                    {
                      'destination': '<(PRODUCT_DIR)',
                      'files': [
                        './nertc_sdk/nertc_linux_v5.7.4/x86_64/lib/libnertc_sdk.so',
                        './nertc_sdk/nertc_linux_v5.7.4/x86_64/lib/libNERtcPersonSegment.so',
                        './nertc_sdk/nertc_linux_v5.7.4/x86_64/lib/libNERtcSuperResolution.so',
                        './nertc_sdk/nertc_linux_v5.7.4/x86_64/lib/libNERtcAiDenoise.so'
                      ]
                    }
                  ],
                  'include_dirs': [
                    './nertc_sdk/nertc_linux_v5.7.4/x86_64/include'
                  ],
                  'library_dirs': [
                    './nertc_sdk/nertc_linux_v5.7.4/x86_64/lib',
                    '<(PRODUCT_DIR)'
                  ]
                }
              ],
              [
                'target_arch=="arm64"',
                {
                  'copies': [
                    {
                      'destination': '<(PRODUCT_DIR)/linux/lib',
                      'files': [
                        './nertc_sdk/nertc_linux_v5.7.4/arm64/lib/libnertc_sdk.so',
                        './nertc_sdk/nertc_linux_v5.7.4/arm64/lib/libNERtcPersonSegment.so',
                        './nertc_sdk/nertc_linux_v5.7.4/arm64/lib/libNERtcSuperResolution.so',
                        './nertc_sdk/nertc_linux_v5.7.4/arm64/lib/libNERtcAiDenoise.so'
                      ]
                    },
                    {
                      'destination': '<(PRODUCT_DIR)/linux/include',
                      'files': [
                        './nertc_sdk/nertc_linux_v5.7.4/arm64/include/*'
                      ]
                    }
                  ],
                  'include_dirs': [
                    './nertc_sdk/nertc_linux_v5.7.4/arm64/include'
                  ],
                  'library_dirs': [
                    './nertc_sdk/nertc_linux_v5.7.4/arm64/lib',
                    '<(PRODUCT_DIR)/linux/lib'
                  ]
                }
              ]
            ]
          }
        ]
      ]
    }
  ]
}
