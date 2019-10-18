#ifndef __VSConfigLibs__
#define __VSConfigLibs__


#define __VSL_TEXTURE_LOADING__ 1

#define __VSL_MODEL_LOADING__ 1


// Note: font loading requires texture loading
// pick one
#define __VSL_FONT_LOADING__ 1
//#define __VSL_FONT_LOADING__ 0

#if (__VSL_FONT_LOADING__ == 1) && (__VSL_TEXTURE_LOADING__ == 0)
#error font loading requires texture loading
#endif

#endif
