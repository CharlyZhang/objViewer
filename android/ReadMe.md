# objViewer for Android
copy files in `src` of root directory to corresponding path in Android project, just for debugging the native code.

##  Difference

### code
1. won't read *.cfg files.
2. create shaders via Java code, which means  `Application3D::createShaders` should be called explictly.

### Usage
1. modify `loadObjModel` params in *GL2JNIView.java* to choose which model to load


