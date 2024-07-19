#include <jni.h>
#include <string>
#include "resampler/MultiChannelResampler.h"
#include <oboe/Oboe.h>
#include "common/InstanceHandler.h"

using namespace oboe;
using namespace androidresampler;

namespace resamplermapping {
    void populateInputBuffer(const jbyte *sourceBuffer, int sourceSample, float *inputBuffer, int sourceChannelCount,
                             int targetChannelCount);

    extern "C"
    JNIEXPORT void JNICALL
    Java_team_unravel_resampler_Resampler_close(JNIEnv *env, jobject thiz) {
        auto *resampler = InstanceHandler::getHandle<oboe::resampler::MultiChannelResampler>(env,
                                                                                                               thiz);
        InstanceHandler::setHandleFieldValue(env, thiz, -1);
        InstanceHandler::clearGlobalReferences(env, thiz);
        delete resampler;
    }

    extern "C"
    JNIEXPORT void JNICALL
    Java_team_unravel_resampler_Resampler_initResampler(JNIEnv *env, jobject thiz, jint source_sample_rate,
                                                        jint target_sample_rate, jint target_channel_count) {
        jobject g_object = InstanceHandler::createGlobalReference(env, thiz, thiz);
        auto *resampler = oboe::resampler::MultiChannelResampler::make(target_channel_count, source_sample_rate,
                                                                       target_sample_rate,
                                                                       oboe::resampler::MultiChannelResampler::Quality::Best);
        InstanceHandler::setHandle(env, g_object, resampler);
    }

    extern "C"
    JNIEXPORT jint JNICALL
    Java_team_unravel_resampler_Resampler_processAudioBuffers(JNIEnv *env, jobject thiz, jobject source_buffer,
                                                              jint sample_count, jobject target_buffer,
                                                              jint target_buffer_size, jint source_channel_count,
                                                              jint target_channel_count) {
        auto *oboeResampler = InstanceHandler::getHandle<oboe::resampler::MultiChannelResampler>(env,
                                                                                                                   thiz);

        if (oboeResampler == nullptr) return 0;

        if (oboeResampler != nullptr && source_channel_count > 0 && target_channel_count > 0) {
            auto sourceBuffer = (jbyte *) env->GetDirectBufferAddress(source_buffer);
            auto targetBuffer = (jbyte *) env->GetDirectBufferAddress(target_buffer);

            int framesProcessed = 0;
            int inputFramesLeft = sample_count;

            float *resamplerInputBuffer = new float[target_channel_count];
            float *resamplerOutputBuffer = new float[target_channel_count];

            while (inputFramesLeft > 0) {
                if (oboeResampler->isWriteNeeded()) {
                    populateInputBuffer(sourceBuffer, sample_count - inputFramesLeft, resamplerInputBuffer,
                                        (int) source_channel_count, (int) target_channel_count);
                    oboeResampler->writeNextFrame(resamplerInputBuffer);
                    inputFramesLeft--;
                } else {
                    oboeResampler->readNextFrame(resamplerOutputBuffer);
                    for (int channel = 0; channel < target_channel_count; channel++) {
                        float value = resamplerOutputBuffer[channel];
                        if (value < -32768) {
                            value = -32768;
                        } else if (value > 32767) {
                            value = 32767;
                        }
                        int index = framesProcessed * target_channel_count + channel;
                        if ((index * 2 + 1) < target_buffer_size) {
                            targetBuffer[index * 2 + 0] = ((short) value) & 0xFF;
                            targetBuffer[index * 2 + 1] = ((short) value >> 8) & 0xFF;
                        }
                    }
                    framesProcessed++;
                }
            }

            if (resamplerInputBuffer != nullptr) {
                delete[] resamplerInputBuffer;
                resamplerInputBuffer = nullptr;
            }
            if (resamplerOutputBuffer != nullptr) {
                delete[] resamplerOutputBuffer;
                resamplerOutputBuffer = nullptr;
            }

            return framesProcessed;
        }
        return 0;
    }

    float getSourceValue(const jbyte *sourceBuffer, int index) {
        // bytes contained in audio buffer produced by MediaCodec make up little endian shorts
        // first we recreate short values, then cast them to floats, expected by Oboe resampler
        return (float) ((short) (((sourceBuffer[index * 2 + 1] & 0xFF) << 8) | sourceBuffer[index * 2] & 0xFF));
    }

    void populateInputBuffer(const jbyte *sourceBuffer, int sourceSample, float *inputBuffer, int sourceChannelCount,
                             int targetChannelCount) {
        int sourceBufferIndex = sourceSample * sourceChannelCount;
        if (sourceChannelCount == targetChannelCount) {
            // no channel mixing (mono to mono or stereo to stereo), just copy data over
            for (int channel = 0; channel < sourceChannelCount; channel++) {
                inputBuffer[channel] = getSourceValue(sourceBuffer, sourceBufferIndex + channel);
            }
        } else if (sourceChannelCount == 1) {
            // mono to stereo, duplicate source value to both output channel
            for (int channel = 0; channel < targetChannelCount; channel++) {
                inputBuffer[channel] = getSourceValue(sourceBuffer, sourceBufferIndex);
            }
        } else if (targetChannelCount == 1) {
            // stereo to mono, calculate the average source channel values and use it as mono channel value
            float monoValue = 0;
            for (int channel = 0; channel < sourceChannelCount; channel++) {
                monoValue += getSourceValue(sourceBuffer, sourceBufferIndex + channel) / (float) sourceChannelCount;
            }
            inputBuffer[0] = monoValue;
        }
    }
}
