#ifndef INSTANCE_HANDLER_TPP
#define INSTANCE_HANDLER_TPP
#include "InstanceHandler.h"

namespace androidresampler {
    template<typename T>
    void InstanceHandler::setHandle(JNIEnv *env, jobject obj, T *t) {
        jlong handle = reinterpret_cast<jlong>(t);
        setHandleFieldValue(env, obj, handle);
    }

    template<typename T>
    T *InstanceHandler::getHandle(JNIEnv *env, jobject obj) {
        jlong handle = getHandleFieldValue(env, obj);
        return reinterpret_cast<T *>(handle);
    }
}
#endif // INSTANCE_HANDLER_TPP
