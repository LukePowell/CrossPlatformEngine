#include "jni.h"
#include "Value.h"


JNIEXPORT jint JNICALL Java_edu_digipen_androidport_MainActivity_value
  (JNIEnv *, jobject)
  {
  	return 15;
  }