// var UNDEFINED = 'undefined';
const NERtcEngine = require('../js/api/index').default
const nertcEngine = new NERtcEngine
// const nertcEngine2 = new NERtcEngine
const assert = require('assert');
const axios = require('axios');
const { add } = require('lodash');
const { stringify } = require('querystring');
const path = require('path')

var generateID = (len) => {
  if (typeof (len) === 'undefined' || len == null || typeof (len) !== 'number') {
    len = 6
  }
  let code = ''
  let codeLength = len
  let random = [1, 2, 3, 4, 5, 6, 7, 8, 9]
  for (var i = 0; i < codeLength; i++) {
    let index = Math.floor(Math.random() * 9)
    code += random[index]
  }
  return code
}

//todo
//playEffect

var g_token = ''
const g_app_key = `d60ed3e24a542526ada4f3bb23f14ac2`
const g_prd_server = "https://nrtc.netease.im/demo/getChecksum.action";
var g_uid = parseInt(generateID(6))
var g_channel = generateID(6)
var g_video_capture_devices = ""

async function getToken(){
  let data = {}
  await axios
    .post(
      g_prd_server + "?uid=" + g_uid + "&appkey=" + g_app_key,
      data,
      {
        header: {
          "Content-Type": "application/x-www-form-urlencoded;charset=utf-8",
        },
      }
    )
    .then((res) => {
      if (res.data.code !== 200) {
        addlog(`getToken error, res.data.code${res.data.code}`);
        return;
      }
      g_token = res.data.checksum;
      addlog(`getToken succ, g_token:${g_token}`);
      return;
    })
    .catch((e) => {
      addlog(`getToken, e:${e}`);
    });
}

function addlog(log){
  console.log(log)
}

let ret = 0
describe('nertc test', ()=>{
  describe('start test', ()=>{
    it('initialize', ()=>{
      let context = {}
      context.app_key = g_app_key
      context.log_dir_path = './'
      context.log_level = 3
      context.log_file_max_size_KBytes = 100*1024*1024
      ret = nertcEngine.initialize(context)
      // let ret2 = nertcEngine2.initialize(context)
      addlog(`nertcEngine.initialize: ret:${ret}`)
      assert.strictEqual(ret, 0)
    });

/*************************加入通话前接口******************************/
    it('startRecordDeviceTest', ()=>{
      // nertcEngine.once('onLocalAudioVolumeIndication', function (volume) {
      //   addlog(`onLocalAudioVolumeIndication, result:${volume}`)
      //   done()
      // })

      ret = nertcEngine.startRecordDeviceTest(100)
      addlog(`nertcEngine.startRecordDeviceTest. ret:${ret}`)

      ret = nertcEngine.stopRecordDeviceTest()
      addlog(`nertcEngine.stopRecordDeviceTest. ret:${ret}`)
      assert.strictEqual(ret, 0)
    });

    // it('startPlayoutDeviceTest', ()=>{
    //   ret = nertcEngine.startPlayoutDeviceTest()

    // });


    it('setClientRole', ()=>{
      ret = nertcEngine.setClientRole(1)
      addlog(`nertcEngine.setClientRole:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('setChannelProfile', ()=>{
      ret = nertcEngine.setChannelProfile(0)
      addlog(`nertcEngine.setChannelProfile:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('setAudioProfile',()=>{
      addlog(`nertcEngine.setAudioProfile:${nertcEngine.setAudioProfile(3, 1)}`)
      assert.strictEqual(0, 0)
    });

    it('enumerateVideoCaptureDevices',()=>{
      let video_capture_devices = nertcEngine.enumerateVideoCaptureDevices()
      g_video_capture_devices = JSON.stringify(video_capture_devices, null, 2)
      addlog(`nertcEngine.enumerateVideoCaptureDevices:${g_video_capture_devices}`)
      if (video_capture_devices.length > 0) {
        ret = nertcEngine.setVideoDevice(video_capture_devices[0].device_id)
        addlog(`nertcEngine.setVideoDevice. ret:${ret}`)

        let cur_video_device = nertcEngine.getVideoDevice()
        addlog(`nertcEngine.getVideoDevice. cur_video_device:${cur_video_device}`)
      }
      assert.strictEqual(ret, 0)
    });

    it('setParameters', ()=>{
      let param = {
        'record_host_enabled': false,
        'record_audio_enabled': false,
        'record_video_enabled': false,
        'record_type': 0,
        'auto_subscribe_audio': true,
        'publish_self_stream_enabled': false,
        'log_level': 3,
        'audio_processing_aec_enable': true,
        'audio_processing_agc_enable': true,
        'audio_processing_ns_enable': true,
        'audio_processing_ai_ns_enable': false,
        'audio_processing_external_audiomix_enable': false,
        'audio_processing_earphone': false,
        'video_sendonpub_type': 1 
      }
      ret = nertcEngine.setParameters(JSON.stringify(param))
      addlog(`setParameters ret: ${ret}`)
      assert.strictEqual(ret, 0)
    });

/*************************加入通话前接口end******************************/


/*************************加入通话后接口******************************/
    it('getToken', async ()=>{
      await getToken()      
    });

    it('JoinChannel', function(done){
      nertcEngine.once('onConnectionStateChange', (state, reason)=>{
        addlog(`onConnectionStateChange, state:${state} reason:${reason}`)
         if(state != 2 || reason != 5){
            done(-1)
         }
      })

      nertcEngine.once('onJoinChannel', function (cid, uid, result, elapsed) {
        addlog(`onJoinChannel, result:${result}`)
        if(0 == result){
          done()
        }else {
          done(result)
        }
      })
      ret = nertcEngine.joinChannel(g_token, g_channel, g_uid)
      addlog(`do join channel ret:${ret}`)
      //要加延时，不然回调会出现句柄错误(回调中出现)
    }).timeout(5000);
    
    it('enableLocalAudio', ()=>{
      ret = nertcEngine.enableLocalAudio(true)
      addlog(`nertcEngine.enableLocalAudio:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('enumerateScreenCaptureSourceInfo', ()=>{
      let thumbWidth = '320'
      let thumbHeight = '180'
      let iconWidth = '32'
      let iconHeight = '32'
      let lst = nertcEngine.enumerateScreenCaptureSourceInfo(parseInt(thumbWidth), parseInt(thumbHeight), parseInt(iconWidth), parseInt(iconHeight))
      addlog(`nertcEngine.enumerateScreenCaptureSourceInfo. lst:${lst}`)
      assert.strictEqual(ret, 0)
    });

    it('startAudioDump', ()=>{
      ret = nertcEngine.startAudioDump()
      addlog(`nertcEngine.startAudioDump:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('stopAudioDump', ()=>{
      ret = nertcEngine.stopAudioDump()
      addlog(`nertcEngine.stopAudioDump:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('enableLocalVideo', ()=>{
      ret = nertcEngine.enableLocalVideo(true)
      addlog(`nertcEngine.enableLocalVideo:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('startAudioRecording', ()=>{
      let fileDir = process.platform === 'darwin' ? path.join(process.env.HOME, 'nertc_logs') : path.join(process.cwd(), 'nertc_logs')
      let fileName = path.join(fileDir, 'audio.aac')
      addlog(`startAudioRecording fileName:${fileName}`)
      ret = nertcEngine.startAudioRecording(fileName, 3200, 0)
      addlog(`nertcEngine.startAudioRecording. ret:${ret}`)

      ret = nertcEngine.stopAudioRecording()
      addlog(`nertcEngine.stopAudioRecording. ret:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('audioMixing', ()=>{
      let opt = {}
      opt.path = `http://music.163.com/song/media/outer/url?id=5274363.mp3`
      opt.loop_count = 1
      opt.send_enabled = true
      opt.send_volume = 100
      opt.playback_enabled = true
      opt.playback_volume = 100
      ret = nertcEngine.startAudioMixing(opt)
      addlog(`nertcEngine.startAudioMixing ret:${ret}`)

      let dur = nertcEngine.getAudioMixingDuration()
      addlog(`nertcEngine.getAudioMixingDuration. dur:${dur}`)

      ret = nertcEngine.setAudioMixingPosition(99)
      addlog(`setAudioMixingPosition. ret:${ret}`)

      let pos = nertcEngine.getAudioMixingCurrentPosition()
      addlog(`getAudioMixingCurrentPosition pos:${pos}`)

      ret = nertcEngine.setAudioMixingSendVolume(20)
      addlog(`nertcEngine.setAudioMixingSendVolume ret:${ret}`)
      
      let vol = nertcEngine.getAudioMixingSendVolume()
      addlog(`nertcEngine.getAudioMixingSendVolume. vol:${vol}`)

      ret = nertcEngine.setAudioMixingPlaybackVolume(25)
      addlog(`nertcEngine.setAudioMixingPlaybackVolume ret:${ret}`)
      
      vol = nertcEngine.getAudioMixingPlaybackVolume()
      addlog(`nertcEngine.getAudioMixingPlaybackVolume. vol:${vol}`)

      ret = nertcEngine.pauseAudioMixing()
      addlog(`nertcEngine.pauseAudioMixing ret:${ret}`)

      ret = nertcEngine.resumeAudioMixing()
      addlog(`nertcEngine.resumeAudioMixing ret:${ret}`)

      ret = nertcEngine.stopAudioMixing()
      addlog(`nertcEngine.stopAudioMixing. ret:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('effect', ()=>{
      let opt = [{
        'path': `http://music.163.com/song/media/outer/url?id=1433584979.mp3`,
        'loop_count': 2,
        'send_enabled': true,
        'send_volume': 100,
        'playback_enabled': true,
        'playback_volume': 100
      }
    ]

      let opt2 = [
        {
          'path': `http://music.163.com/song/media/outer/url?id=1336840616.mp3`,
          'loop_count': 2,
          'send_enabled': true,
          'send_volume': 100,
          'playback_enabled': true,
          'playback_volume': 100
        }
      ]
      ret = nertcEngine.playEffect(1, opt)
      addlog(`nertcEngine.playEffect. ret:${ret}`)

      ret = nertcEngine.playEffect(2, opt2)
      addlog(`nertcEngine.playEffect. ret:${ret}`)

      ret = nertcEngine.pauseEffect(1)
      addlog(`nertcEngine.pauseEffect. ret:${ret}`)

      ret = nertcEngine.resumeEffect(1)
      addlog(`nertcEngine.resumeEffect. ret:${ret}`)

      ret = nertcEngine.pauseAllEffects()
      addlog(`nertcEngine.pauseAllEffects. ret:${ret}`)

      ret = nertcEngine.resumeAllEffects()
      addlog(`nertcEngine.resumeAllEffects. ret:${ret}`)

      ret = nertcEngine.setEffectSendVolume(2, 50)
      addlog(`nertcEngine.setEffectSendVolume. ret:${ret}`)

      ret = nertcEngine.getEffectSendVolume(2, 50)
      addlog(`nertcEngine.getEffectSendVolume. ret:${ret}`)

      ret = nertcEngine.setEffectPlaybackVolume(2, 60)
      addlog(`nertcEngine.setEffectPlaybackVolume. ret:${ret}`)

      ret = nertcEngine.getEffectPlaybackVolume(2)
      addlog(`nertcEngine.getEffectPlaybackVolume. ret:${ret}`)

      ret = nertcEngine.stopEffect(1)
      addlog(`nertcEngine.stopEffect. ret:${ret}`)

      ret = nertcEngine.stopAllEffects()
      addlog(`nertcEngine.stopAllEffects. ret:${ret}`)

      assert.strictEqual(ret, 0)
    });

    it('enableEarback', ()=>{
      ret = nertcEngine.enableEarback(true, 70)
      addlog(`nertcEngine.enableEarback. ret:${ret}`)

      ret = nertcEngine.setEarbackVolume(90)
      addlog(`nertcEngine.setEarbackVolume. ret:${ret}`)

      assert.strictEqual(ret, 0)
    });

    it('enableAudioVolumeIndication', ()=>{
      ret = nertcEngine.enableAudioVolumeIndication(true, 100)
      addlog(`nertcEngine.enableAudioVolumeIndication. ret:${ret}`)
      assert.strictEqual(ret, 0)
    });

    //必须伴随视频流一起发送 sendSEIMsgEx一样
    // it('sendSEIMsg', ()=>{
    //   const u8string = new TextEncoder().encode('test send seimsg')
    //   ret = nertcEngine.sendSEIMsg(u8string.buffer)
    //   addlog(`nertcEngine.sendSEIMsg. ret:${ret}`)
    //   assert.strictEqual(ret, 0)
    // });

    it('getVersion', ()=>{
      addlog(`nertcEngine.getVersion. ret:${nertcEngine.getVersion()}`)
      assert.strictEqual(ret, 0)
    });

    // it('uploadSdkInfo', ()=>{
    //   ret= nertcEngine.uploadSdkInfo()
    //   addlog(`nertcEngine.uploadSdkInfo. ret:${ret}`)
    //   assert.strictEqual(0, 0)
    // });


    it('recordDevices', ()=>{
      let recordDevices = nertcEngine.enumerateRecordDevices()
      addlog(`nertcEngine.enumerateRecordDevices. ret:${recordDevices}`)
      if(recordDevices.length > 0){
        let type = typeof recordDevices[0].device_id
        addlog(`${type}, ${recordDevices[0].device_id}`)
        ret = nertcEngine.setRecordDevice(recordDevices[0].device_id)
        addlog(`nertcEngine.muteLocalVideoStream:${ret}`)

        let cur_record_device = nertcEngine.getRecordDevice()
        addlog(`nertcEngine.getRecordDevice:${cur_record_device}`)

        ret = nertcEngine.setRecordDeviceVolume(75)
        addlog(`nertcEngine.setRecordDeviceVolume:${ret}`)

        let vol = nertcEngine.getRecordDeviceVolume()
        addlog(`nertcEngine.getRecordDeviceVolume:${vol}`)

        ret = nertcEngine.setRecordDeviceMute(true)
        addlog(`nertcEngine.setRecordDeviceMute:${ret}`)

        let record_mute = nertcEngine.getRecordDeviceMute()
        addlog(`nertcEngine.getRecordDeviceMute:${record_mute}`)

        ret = nertcEngine.setRecordDeviceMute(false)
        addlog(`nertcEngine.setRecordDeviceMute:${ret}`)

        ret = nertcEngine.adjustRecordingSignalVolume(200)
        addlog(`nertcEngine.adjustRecordingSignalVolume:${ret}`)
      }
      assert.strictEqual(ret, 0)
    });

    it('playoutDevice', ()=>{
      let devices = nertcEngine.enumeratePlayoutDevices()
      addlog(`nertcEngine.enumeratePlayoutDevices. ret:${devices}`)

      if(devices.length > 0){
        ret = nertcEngine.setPlayoutDevice(devices[0].device_id)
        addlog(`nertcEngine.setPlayoutDevice:${ret}`)

        let cur_device = nertcEngine.getPlayoutDevice()
        addlog(`nertcEngine.getPlayoutDevice:${cur_device}`)

        ret = nertcEngine.setPlayoutDeviceVolume(85)
        addlog(`nertcEngine.setPlayoutDeviceVolume:${ret}`)

        let vol = nertcEngine.getPlayoutDeviceVolume()
        addlog(`nertcEngine.getPlayoutDeviceVolume:${vol}`)

        ret = nertcEngine.setPlayoutDeviceMute(true)
        addlog(`nertcEngine.setPlayoutDeviceMute:${ret}`)

        let playout_mute = nertcEngine.getPlayoutDeviceMute()
        addlog(`nertcEngine.getPlayoutDeviceMute:${playout_mute}`)

        ret = nertcEngine.setPlayoutDeviceMute(false)
        addlog(`nertcEngine.setPlayoutDeviceMute:${ret}`)

        ret = nertcEngine.adjustPlaybackSignalVolume(150)
        addlog(`nertcEngine.adjustPlaybackSignalVolume:${ret}`)

        assert.strictEqual(ret, 0)
      }
    });

    it('muteLocalVideoStream',()=>{
      ret = nertcEngine.muteLocalVideoStream(true)
      addlog(`nertcEngine.muteLocalVideoStream:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('getConnectionState',()=>{
      addlog(`nertcEngine.getConnectionState:${nertcEngine.getConnectionState()}`)
      assert.strictEqual(0, 0)
    });

    it('muteLocalAudioStream',()=>{
      addlog(`nertcEngine.muteLocalAudioStream:${nertcEngine.muteLocalAudioStream(true)}`)
      assert.strictEqual(0, 0)
    });

    it('setVideoConfig',()=>{
      let cfg = {
        'max_profile': 2,
        'width': 0,
        'height': 0,
        'crop_mode': 0,
        'framerate': 30,
        'min_framerate': 0,
        'bitrate': 0,
        'min_bitrate': 0,
        'degradation_preference': 0
      }
      addlog(`nertcEngine.setVideoConfig:${nertcEngine.setVideoConfig(cfg)}`)
      assert.strictEqual(0, 0)
    });

    it('enableDualStreamMode',()=>{
      addlog(`nertcEngine.enableDualStreamMode:${nertcEngine.enableDualStreamMode(true)}`)
      assert.strictEqual(0, 0)
    });

    it('setLocalVideoMirrorMode',()=>{
      addlog(`nertcEngine.setLocalVideoMirrorMode:${nertcEngine.setLocalVideoMirrorMode(1)}`)
      assert.strictEqual(0, 0)
    });

    it('onRtcStats',(done)=>{
      nertcEngine.once('onRtcStats', (stats)=>{
        addlog(`onRtcStats, stats:${JSON.stringify(stats, null, 2)}`)
        done()
      })
    }).timeout(5000);

    // it('onLocalAudioStats',(done)=>{
    //   nertcEngine.once('onLocalAudioStats', (stats)=>{
    //     addlog(`onLocalAudioStats, stats:${JSON.stringify(stats, null, 2)}`)
    //     done()
    //   })
    // }).timeout(5000);

    it('onClientRoleChanged', (done)=>{
      nertcEngine.once('onClientRoleChanged', (oldRole, newRole)=>{
        addlog(`onClientRoleChanged, oldRole:${oldRole} newRole:${newRole}`)
        done()
      })

      ret = nertcEngine.setClientRole(0)
      addlog(`nertcEngine.setClientOldRole:${ret}`)
      ret = nertcEngine.setClientRole(1)
      addlog(`nertcEngine.setClientNewRole:${ret}`)
      // assert.strictEqual(ret, 0)
    });


    it('leaveChannel', function(done){
      nertcEngine.once('onReleasedHwResources', function (result) {
        addlog(`onReleasedHwResources: result:${result}`)
        if(0 != result){
          done(-1);
        }
      })

      nertcEngine.once('onLeaveChannel', function (result) {
        addlog(`onLeaveChannel: result:${result}`)
        if(result != 0){
          done(-1);
        } else {
          done();
        }
      })
      ret = nertcEngine.leaveChannel()
      addlog(`do leave channel ret: ${ret}`)
    });

   


/*************************加入通话后接口end******************************/

    it('release', ()=>{
      nertcEngine.release()
      assert.strictEqual(0, 0)
    });



  });
}).timeout(50000);

