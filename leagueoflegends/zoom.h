#ifndef ZOOM_H
#define ZOOM_H
typedef unsigned long long QWORD;
void startZoom();
//void unloadZoom();
//void ejectZoom
inline QWORD zoomAddress;

inline bool hidden;
inline bool shouldInject;
inline bool shouldWrite;
inline bool useKeyboard;
inline bool useMouseWheel;
inline bool isLeagueFocused;

inline float angleCamera;
inline float zoomSlider;
#endif
