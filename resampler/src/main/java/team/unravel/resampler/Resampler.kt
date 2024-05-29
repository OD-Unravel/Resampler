package team.unravel.resampler

import com.github.nihad.sampleconverter.common.IHandle
import java.nio.ByteBuffer
import kotlin.math.min

class Resampler constructor(
  sourceSampleRate: Int,
  val sourceChannelCount: Int,
  targetSampleRate: Int,
  val targetChannelCount: Int
) : IHandle, AutoCloseable {
  init {
    if (sourceChannelCount > 1 && targetChannelCount > 1 && sourceChannelCount != targetChannelCount) {
      throw UnsupportedOperationException("Multi channel to Multi channel mixing is not supported")
    }
    initResampler(sourceSampleRate, targetSampleRate, targetChannelCount)
  }

  private external fun initResampler(sourceSampleRate: Int, targetSampleRate: Int, targetChannelCount: Int);

  fun processAudioBuffers(sourceBuffer: ByteBuffer, outputBuffer: ByteBuffer) : Int {
      val sourceSampleCount = sourceBuffer.remaining() / (BYTES_PER_SAMPLE * sourceChannelCount)
      val targetSampleCount = processAudioBuffers(sourceBuffer, sourceSampleCount, outputBuffer, outputBuffer.capacity(), sourceChannelCount, targetChannelCount)

      val targetBufferSize = targetSampleCount * BYTES_PER_SAMPLE * targetChannelCount
      outputBuffer.rewind()

      val limit = min(targetBufferSize, outputBuffer.capacity())

      outputBuffer.limit(limit)
    return limit;
  }

  private external fun processAudioBuffers(sourceBuffer: ByteBuffer, sampleCount: Int, targetBuffer: ByteBuffer, targetBufferSize: Int, sourceChannelCount: Int, targetChannelCount: Int) : Int

  external override fun close();

  override val nativeHandle: Long = 0

  companion object {
    // Used to load the 'resampler' library on application startup.
    init {
      System.loadLibrary("resampler")
    }

    val BYTES_PER_SAMPLE = 2
  }
}