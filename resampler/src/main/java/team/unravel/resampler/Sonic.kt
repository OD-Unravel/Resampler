package team.unravel.resampler

import com.github.nihad.sampleconverter.common.IHandle

class Sonic constructor(sampleRate: Int, numChannels: Int) : IHandle, AutoCloseable{

  // Just insure that close gets called, in case the user forgot.
  protected fun finalize() {
    // It is safe to call this twice, in case the user already did.
    close()
  }

  var sampleRate: Int
    // Get the sample rate of the stream.
    get() = getSampleRateNative()
    // Set the sample rate of the stream.  This will drop any samples that have not been read.
    set(newSampleRate) {
      setSampleRateNative(newSampleRate)
    }
  var numChannels: Int
    // Get the number of channels.
    get() = getNumChannelsNative()
    // Set the number of channels.  This will drop any samples that have not been read.
    set(newNumChannels) {
      setNumChannelsNative(newNumChannels)
    }
  var pitch: Float
    // Get the pitch of the stream.
    get() = getPitchNative()
    // Set the pitch of the stream.
    set(newPitch) {
      setPitchNative(newPitch)
    }
  var speed: Float
    // Get the speed of the stream.
    get() = getSpeedNative()
    //Set the speed of the stream.
    set(newSpeed) {
      setSpeedNative(newSpeed)
    }
  var rate: Float
    // Get the rate of the stream.
    get() = getRateNative()
    // Set the rate of the stream.  Rate means how fast we play, without pitch correction
    set(newRate) {
      setRateNative(newRate)
    }
  var chordPitch: Boolean
    // Get the chord pitch setting.
    get() = getChordPitchNative()
    // Set chord pitch mode on or off.  Default is off.  See the documentation
    set(useChordPitch) {
      setChordPitchNative(useChordPitch)
    }

  var volume: Float
    // Get the scaling factor of the stream.
    get() = getVolumeNative()
    // Set the scaling factor of the stream.
    set(newVolume) {
      setVolumeNative(newVolume)
    }

  private external fun initialize(sampleRate: Int, channels: Int)

  // Note that changing the sample rate or num channels will cause a flush.
  private external fun setSampleRateNative(newSampleRate: Int)
  private external fun getSampleRateNative(): Int
  private external fun setNumChannelsNative(newNumChannels: Int)
  private external fun getNumChannelsNative(): Int
  private external fun setPitchNative(newPitch: Float)
  private external fun getPitchNative(): Float
  private external fun setSpeedNative(newSpeed: Float)
  private external fun getSpeedNative(): Float
  private external fun setRateNative(newRate: Float)
  private external fun getRateNative(): Float
  private external fun setChordPitchNative(useChordPitch: Boolean)
  private external fun getChordPitchNative(): Boolean
  private external fun setVolumeNative(newVolume: Float)
  private external fun getVolumeNative(): Float
  // Use this to write 16-bit data to be speed up or down into the stream.
  // Return false if memory realloc failed, otherwise true.
  external fun putBytes(buffer: ByteArray, lenBytes: Int): Boolean

  // Use this to read 16-bit data out of the stream.  Sometimes no data will
  // be available, and zero is returned, which is not an error condition.
  external fun receiveBytes(ret: ByteArray, lenBytes: Int): Int

  // Return the number of samples in the output buffer
  external fun availableBytes(): Int
  // Force the sonic stream to generate output using whatever data it currently
  // has.  No extra delay will be added to the output, but flushing in the middle of
  // words could introduce distortion.
  external fun flush()
  // Call this to clean up memory after you're done processing sound.
  external override fun close()

  // Create a sonic stream.  Return false only if we are out of memory and cannot
  // allocate the stream. Set numChannels to 1 for mono, and 2 for stereo.
  init {
    initialize(sampleRate, numChannels)
  }

  companion object {
    init {
      System.loadLibrary("resampler")
    }
  }

  override val nativeHandle: Long = 0
}
