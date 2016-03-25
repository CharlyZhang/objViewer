# objViewer for Android

###  Difference from other platform

1. won't read *.cfg files.
2. create shaders via Java code, which means  `Application3D::createShaders` should be called explictly.
3. modify `loadObjModel` params in *GL2JNIView.java* to choose which model to load


