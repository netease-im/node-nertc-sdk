{
  "targets": [
    {
      'target_name': 'nertc-sdk',
      'include_dirs': [
      './shared',
      './shared/libyuv/include'
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
            'copies': [{
              'destination': '<(PRODUCT_DIR)',
              'files': [
                './nertc_sdk/bin/windows/x86/nertc_sdk.dll',
                './nertc_sdk/bin/windows/x86/SDL2.dll'
              ]
            }],
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
            'include_dirs': [
              './nertc_sdk/api/'
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
            'copies': [{
              'destination': '<(PRODUCT_DIR)',
              'files': [
                './nertc_sdk/bin/darwin/nertc_sdk_Mac.framework',
                './nertc_sdk/bin/darwin/NEFundation_Mac.framework'
              ]
            }],
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
            'include_dirs': [
              './nertc_sdk/',
              './nertc_sdk/bin/darwin/nertc_sdk_Mac.framework/Headers/',
              './nertc_sdk/bin/darwin/NEFundation_Mac.framework/Headers/'
            ],
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
    },
    # {
    #   'target_name': 'nim_electron_sdk',
    #   'includes': [
    #     # './common.gypi',
    #   ],
    #   'include_dirs': [
    #   ],
    #   'sources': [ 
    #     './nim_sdk_node/nim_node_engine.cpp',
    #     './nim_sdk_node/nim_node_engine.h',
    #     './shared/sdk_helper/nim_node_helper.h',
    #     './shared/sdk_helper/nim_node_helper.cpp',
    #     './shared/sdk_helper/nim_node_async_queue.h',
    #     './shared/sdk_helper/nim_node_async_queue.cpp',
    #     './shared/sdk_helper/nim_event_handler.h',
    #     './shared/sdk_helper/nim_event_handler.cpp',
    #     './nim_sdk_node/api/nim_node_client.h',
    #     './nim_sdk_node/api/nim_node_client.cpp',
    #     './nim_sdk_node/api/nim_node_client_event_handler.h',
    #     './nim_sdk_node/api/nim_node_client_event_handler.cpp',
    #     './nim_sdk_node/helper/nim_node_client_helper.h',
    #     './nim_sdk_node/helper/nim_node_client_helper.cpp',
    #     './nim_sdk_node/api/nim_node_talk.h',
    #     './nim_sdk_node/api/nim_node_talk.cpp',
    #     './nim_sdk_node/api/nim_node_talk_event_handler.h',
    #     './nim_sdk_node/api/nim_node_talk_event_handler.cpp',
    #     './nim_sdk_node/helper/nim_node_talk_helper.h',
    #     './nim_sdk_node/helper/nim_node_talk_helper.cpp',
    #     './nim_sdk_node/api/nim_node_team.h',
    #     './nim_sdk_node/api/nim_node_team.cpp',
    #     './nim_sdk_node/api/nim_node_team_event_handler.h',
    #     './nim_sdk_node/api/nim_node_team_event_handler.cpp',
    #     './nim_sdk_node/helper/nim_node_team_helper.h',
    #     './nim_sdk_node/helper/nim_node_team_helper.cpp',
    #     './nim_sdk_node/api/nim_node_super_team.h',
    #     './nim_sdk_node/api/nim_node_super_team.cpp',
    #     './nim_sdk_node/api/nim_node_super_team_event_handler.h',
    #     './nim_sdk_node/api/nim_node_super_team_event_handler.cpp',
    #     './nim_sdk_node/helper/nim_node_super_team_helper.h',
    #     './nim_sdk_node/helper/nim_node_super_team_helper.cpp',
    #     './nim_sdk_node/api/nim_node_session.h',
    #     './nim_sdk_node/api/nim_node_session.cpp',
    #     './nim_sdk_node/api/nim_node_session_event_handler.h',
    #     './nim_sdk_node/api/nim_node_session_event_handler.cpp',
    #     './nim_sdk_node/helper/nim_node_session_helper.h',
    #     './nim_sdk_node/helper/nim_node_session_helper.cpp',
    #     './nim_sdk_node/api/nim_node_msglog.h',
    #     './nim_sdk_node/api/nim_node_msglog.cpp',
    #     './nim_sdk_node/api/nim_node_msglog_event_handler.h',
    #     './nim_sdk_node/api/nim_node_msglog_event_handler.cpp',
    #     './nim_sdk_node/helper/nim_node_msglog_helper.h',
    #     './nim_sdk_node/helper/nim_node_msglog_helper.cpp',
    #     './nim_sdk_node/api/nim_node_sysmsg.h',
    #     './nim_sdk_node/api/nim_node_sysmsg.cpp',
    #     './nim_sdk_node/api/nim_node_sysmsg_event_handler.h',
    #     './nim_sdk_node/api/nim_node_sysmsg_event_handler.cpp',
    #     './nim_sdk_node/helper/nim_node_sysmsg_helper.h',
    #     './nim_sdk_node/helper/nim_node_sysmsg_helper.cpp',
    #     './nim_sdk_node/api/nim_node_user.h',
    #     './nim_sdk_node/api/nim_node_user.cpp',
    #     './nim_sdk_node/api/nim_node_user_event_handler.h',
    #     './nim_sdk_node/api/nim_node_user_event_handler.cpp',
    #     './nim_sdk_node/helper/nim_node_user_helper.h',
    #     './nim_sdk_node/helper/nim_node_user_helper.cpp',
    #     './nim_sdk_node/api/nim_node_friend.h',
    #     './nim_sdk_node/api/nim_node_friend.cpp',
    #     './nim_sdk_node/api/nim_node_friend_event_handler.h',
    #     './nim_sdk_node/api/nim_node_friend_event_handler.cpp',
    #     './nim_sdk_node/helper/nim_node_friend_helper.h',
    #     './nim_sdk_node/helper/nim_node_friend_helper.cpp',
    #     './nim_sdk_node/api/nim_node_global.h',
    #     './nim_sdk_node/api/nim_node_global.cpp',
    #     './nim_sdk_node/api/nim_node_global_event_handler.h',
    #     './nim_sdk_node/api/nim_node_global_event_handler.cpp',
    #     './nim_sdk_node/helper/nim_node_global_helper.h',
    #     './nim_sdk_node/helper/nim_node_global_helper.cpp',
    #     './nim_sdk_node/api/nim_node_tool.h',
    #     './nim_sdk_node/api/nim_node_tool.cpp',
    #     './nim_sdk_node/api/nim_node_tool_event_handler.h',
    #     './nim_sdk_node/api/nim_node_tool_event_handler.cpp',
    #     './nim_sdk_node/helper/nim_node_tool_helper.h',
    #     './nim_sdk_node/helper/nim_node_tool_helper.cpp',
    #     './nim_sdk_node/api/nim_node_subscribe_event.h',
    #     './nim_sdk_node/api/nim_node_subscribe_event.cpp',
    #     './nim_sdk_node/api/nim_node_subscribe_event_handler.h',
    #     './nim_sdk_node/api/nim_node_subscribe_event_handler.cpp',
    #     './nim_sdk_node/helper/nim_node_subscribe_helper.h',
    #     './nim_sdk_node/helper/nim_node_subscribe_helper.cpp',
    #     './nim_sdk_node/api/nim_node_nos.h',
    #     './nim_sdk_node/api/nim_node_nos.cpp',
    #     './nim_sdk_node/api/nim_node_nos_event_handler.h',
    #     './nim_sdk_node/api/nim_node_nos_event_handler.cpp',
    #     './nim_sdk_node/helper/nim_node_nos_helper.h',
    #     './nim_sdk_node/helper/nim_node_nos_helper.cpp',
    #     './nim_sdk_node/api/nim_node_data_sync.h',
    #     './nim_sdk_node/api/nim_node_data_sync.cpp',
    #     './nim_sdk_node/api/nim_node_data_event_handler.h',
    #     './nim_sdk_node/api/nim_node_data_event_handler.cpp',
    #     './nim_sdk_node/api/nim_node_online_session.h',
    #     './nim_sdk_node/api/nim_node_online_session.cpp',
    #     './nim_sdk_node/api/nim_node_online_session_event_handler.h',
    #     './nim_sdk_node/api/nim_node_online_session_event_handler.cpp',
    #     './nim_sdk_node/helper/nim_node_online_session_helper.h',
    #     './nim_sdk_node/helper/nim_node_online_session_helper.cpp',
    #     './nim_sdk_node/api/nim_node_pass_through_proxy.h',
    #     './nim_sdk_node/api/nim_node_pass_through_proxy.cpp',
    #     './nim_sdk_node/api/nim_node_pass_service_event_handler.h',
    #     './nim_sdk_node/api/nim_node_pass_service_event_handler.cpp',
    #   ],
    #   'conditions': [
        # [
        #   'OS=="win"',
        #   {
        #     'copies': [{
        #       'destination': '<(PRODUCT_DIR)',
        #       'files': [
        #         './nim_sdk/bin/x86_dlls/nim_sdk_cpp_wrapper_dll.dll',
        #         './nim_sdk/bin/x86_dlls/nim.dll',
        #         # './nim_sdk/bin/x86_dlls/nim_data_client.dll',
        #         # './nim_sdk/bin/x86_dlls/nim_tools_http.dll',
        #         # './nim_sdk/bin/x86_dlls/nim_audio.dll'
        #       ]
        #     }],
        #     'defines': [
        #       'NIM_WIN_DESKTOP_ONLY_SDK',
        #       'NIM_SDK_DLL_IMPORT',
        #       'CPPWRAPPER_DLL',
        #       'WIN32',
        #       'WIN32_LEAN_AND_MEAN'
        #     ],
        #     'library_dirs': [
        #       './nim_sdk/bin/x86_dlls/',
        #       './nim_sdk/libs/x86/md/'
        #     ],
        #     'link_settings': {
        #       'libraries': [
        #         '-lnim_sdk_cpp_wrapper_dll.lib',
        #         '-lcpp_wrapper_util.lib'
        #       ]
        #     },
        #     'msvs_settings': {
        #       'VCCLCompilerTool': {
        #         # 'WarningLevel': '3',
        #         # 'DisableSpecificWarnings': ['4819'],
        #         # 'WarnAsError': 'false',
        #         # 'ExceptionHandling': '0',
        #         'AdditionalOptions': [
        #           # '/EHsc',
        #           '/utf-8'
        #         ]            
        #       }
        #     },
        #     'defines!': [
        #       # '_USING_V110_SDK71_',
        #       # '_HAS_EXCEPTIONS=0'
        #     ],
        #     'sources': [
        #     ],
        #     'include_dirs': [
        #       './nim_sdk'
        #     ],
        #     'configurations': {
        #       'Release': {
        #         'msvs_settings': {
        #           'VCCLCompilerTool': {
        #             # 多线程 DLL (/MD)
        #             'RuntimeLibrary': '2',
        #             # 完全优化 /Os
        #             'Optimization': '2',
        #             # 使用内部函数 /Oi
        #             'EnableIntrinsicFunctions': 'true',
        #             # 程序数据库 (/Zi)
        #             'DebugInformationFormat': '3',
        #             'AdditionalOptions': [
        #             ]            
        #           }
        #         },
        #       },
        #       'Debug': {
        #         'msvs_settings': {
        #           'VCCLCompilerTool': {
        #             'RuntimeLibrary': '3',
        #             # 'WarningLevel': '3',
        #             # 'DisableSpecificWarnings': ['4819'],
        #             # 'WarnAsError': 'false',
        #             # 'ExceptionHandling': '0',
        #             'AdditionalOptions': [
        #             ]            
        #           }
        #         },
        #       }
        #     }
        #   }
        # ],
    #     [
    #       'OS=="mac"',
    #       {
    #         'copies': [{
    #           'destination': '<(PRODUCT_DIR)',
    #           'files': [
    #             './nim_sdk/bin/darwin/nim_sdk_Mac.framework',
    #             './nim_sdk/bin/darwin/core_Mac.framework'
    #           ]
    #         }],
    #         'defines': [
    #           'NIM_SDK_DLL_IMPORT',
    #           'OS_MACOSX'
    #         ],
    #         'mac_framework_dirs': [
    #           '../nim_sdk/bin/darwin'
    #         ],
    #         'library_dirs': [
    #           '../nim_sdk/bin/darwin'
    #         ],
    #         'link_settings': {
    #           'libraries': [
    #             'libcpp_wrapper_util_Mac.a',
    #             'libnim_wrapper_Mac.a',
    #             'Foundation.framework',
    #             'nim_sdk_Mac.framework',
    #             'core_Mac.framework',
    #             # '-rpath /Users/surebeli/Documents/code/electron-sdk/build/Release/'
    #             '-rpath ./static/nim_sdk/darwin/'
    #             ]
    #         },
    #         'defines!': [
    #           # '_NOEXCEPT',
    #           '-std=c++11'
    #         ],	
    #         'sources': [
    #         ],
    #         'include_dirs': [
    #           './nim_sdk/',
    #           './nim_sdk/bin/darwin/nim_sdk_Mac.framework/Headers/',
    #           './nim_sdk/bin/darwin/core_Mac.framework/Headers/'
    #         ],
    #         'xcode_settings': {
    #           # 'ALWAYS_SEARCH_USER_PATHS': 'NO',
    #           #'i386', 'x86_64'
    #           'ARCHS': [ 'x86_64' ],
    #           'MACOSX_DEPLOYMENT_TARGET': '10.14',
    #           #'CC': 'clang',
    #           #'GCC_VERSION': 'com.apple.compilers.llvm.clang.1_0',
    #           #'CLANG_CXX_LANGUAGE_STANDARD': 'c++0x',
    #           # libstdc++, c++11, libc++
    #           #'CLANG_CXX_LIBRARY': 'libstdc++',
    #           #'GCC_ENABLE_OBJC_GC': 'unsupported',
    #           'EXCUTABLE_EXTENSION': 'node',
    #           'FRAMEWORK_SEARCH_PATHS': [
    #             './nim_sdk/bin/osx'
    #           ],
    #           #'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
    #           #'GCC_SYMBOLS_PRIVATE_EXTERN': 'NO',
    #           'DEBUG_INFORMATION_FORMAT': 'dwarf-with-dsym',
    #           #'DEPLOYMENT_POSTPROCESSING': 'YES',
    #           'OTHER_CFLAGS': [
    #             #'-fno-eliminate-unused-debug-symbols',
    #             #'-mmacosx-version-min=10.8',
    #             # compile use oc++
    #             #'-x objective-c++',
    #           ],
    #           #'WARNING_CFLAGS': ['-Wno-deprecated-declarations'],
    #           #'WARNING_CFLAGS!': ['-Wall', '-Wextra',],
    #           #'WARNING_CXXFLAGS': ['-Wstrict-aliasing', '-Wno-deprecated',],
    #         }# xcode_settings
    #       }
    #     ]
    #   ]
    # }
  ]
}