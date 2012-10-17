//
//  hook_vdadecoder.c
//  RayInjector
//
//  Created by Raymond Hung on 12/9/26.
//
//

#include <stdio.h>
#include <CoreFoundation/CoreFoundation.h>
#include "util.h"
#include "dlfcn.h"
/*
 OSStatus VDADecoderDecode(VDADecoder      decoder,
 uint32_t        decodeFlags,
 CFTypeRef       compressedBuffer,
 CFDictionaryRef frameInfo)
 */

typedef OSStatus (*TYPE_VDADecoderDecode) (void *decoder,uint32_t flag,CFTypeRef buffer,CFDictionaryRef frameInfo);
TYPE_VDADecoderDecode original_VDADecoderDecode = NULL;
OSStatus VDADecoderDecode(void* decoder,uint32_t flag,CFTypeRef buffer,CFDictionaryRef frameInfo)
{
    static FILE *fout = NULL;
    static unsigned long total = 0;
    static unsigned long count = 0;
    static bool bFirstCalled = true;
    if (original_VDADecoderDecode == NULL)
    {
        original_VDADecoderDecode = (TYPE_VDADecoderDecode)dlsym(RTLD_NEXT, "VDADecoderDecode");
        if (original_VDADecoderDecode == NULL)
        {
            void* lib = dlopen("/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk/System/Library/Frameworks/VideoDecodeAcceleration.framework/VideoDecodeAcceleration", RTLD_NOW);
            original_VDADecoderDecode = (TYPE_VDADecoderDecode)dlsym(lib, "VDADecoderDecode");
        }
        printf("hooked VDADecoderDecode:%x replaced:%x\n",original_VDADecoderDecode,&VDADecoderDecode);
        fout = fopen("decoder_input_vda.log","wb");
    }
    
    OSStatus ret = original_VDADecoderDecode(decoder,flag,buffer,frameInfo);
    
    if (bFirstCalled)
    {
        bFirstCalled = false;
        unsigned char* data = (unsigned char*)CFDataGetBytePtr(buffer);
        int len = CFDataGetLength(buffer);
        printf("VDADecoderDecode: decoder input=%d bytes\n",len);
        
        char str[256];
        sprintf(str,"decoder input prefix[%d]:",count++);
        dump16Bytes(data,str);
    }

    return ret;
}