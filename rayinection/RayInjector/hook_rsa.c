//
//  hook_rsa.c
//  RayInjector
//
//  Created by  on 7/4/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <util.h>
#include <dlfcn.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/RSA.h>


//========
typedef unsigned int u32;
typedef char s8;


static s8 ConvertDWordToByte( u32 *Data, u32 Offset ) {
    
    u32 tmp, off, bs;
    
    
    off = Offset / 4;
    bs = Offset % 4;
    if( bs ) {
        
        tmp = *(Data + off);
        tmp = ((tmp >> (bs * 8)) & 0xFF);
        return tmp;
    }
    
    tmp = ((*(Data + off)) & 0xFF);
    return tmp;
}


static void DumpData( u32 *Data, u32 Length, u32 BaseAddr ) {
    
    u32 i, j;
    u32 c;
    
    
    printf( "\n\n== Dump Memory Start ==\n\n" );
    printf( " Address | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F|   ASCII DATA   \n" );
    printf( "---------------------------------------------------------------------------\n" );
    
    
    for( i = 0 ; i <= Length ; i++ ) {
        
        
        if( !(i % 16) ) {
            
            
            if( (i > 15) ) {
                
                for( j = i - 16 ; j < i ; j++ ) {
                    
                    c = ConvertDWordToByte( Data, j );
                    if( ((c >= '!') && (c <= '~')) ) {
                        
                        printf( "%c", c );
                    }
                    else {
                        
                        printf( "." );
                    }
                }
            }
            
            
            if( i ) {
                
                printf( "\n" );
            }
            
            
            if( i == Length ) {
                
                break;
            }
            
            
            printf( "%8.8X : ", i + BaseAddr );
        }
        
        
        printf( "%2.2X ", ConvertDWordToByte( Data, i ) & 0xFF );
    }
    
    
    printf( "---------------------------------------------------------------------------\n" );
    printf( "\n== Dump Memory End ==\n\n" );
}
//=======


typedef int (*TYPE_RSA_size) (void* rsa);
TYPE_RSA_size rsa_size = NULL;

typedef int (*TYPE_RSA_private_decrypt) (int flen, const unsigned char *from,
                                         unsigned char *to, RSA *rsa,int padding);

TYPE_RSA_private_decrypt original_RSA_private_decrypt = NULL;
int RSA_private_decrypt(int flen, const unsigned char *from,
                            unsigned char *to, RSA *rsa,int padding)
{
    FILE *fp;
    
    if (rsa_size == NULL)
        rsa_size = (TYPE_RSA_size)dlsym(RTLD_NEXT, "RSA_size");
    
    printf( "RSA_private_decryptHOOK: FIND RSA PRIV START\n" );
    
    fp = fopen( "/Users/merck/Desktop/private.key" , "a+");
    PEM_write_PrivateKey( fp, rsa, NULL, NULL, 0, NULL, NULL );
    fclose( fp );
    
    printf( "RSA_private_decryptHOOK: FIND RSA PRIV END\n" );
    
    if (original_RSA_private_decrypt == NULL)
    {
        original_RSA_private_decrypt = dlsym(RTLD_NEXT, "RSA_private_decrypt");
        printf("hooked RSA_private_decrypt:%x\n",original_RSA_private_decrypt);
    }
    
    int ret = original_RSA_private_decrypt(flen,from,to,rsa,padding);
    
    DumpData( to, strlen(to), 0x0 );
    
    printf("after rsa private dec\n");
    return ret;
}


typedef int (*TYPE_RSA_private_encrypt) (int flen, const unsigned char *from,
                                         unsigned char *to, RSA *rsa,int padding);

TYPE_RSA_private_encrypt original_RSA_private_encrypt = NULL;

int     RSA_private_encrypt(int flen, const unsigned char *from,
                            unsigned char *to, RSA *rsa,int padding)
{
    if (rsa_size == NULL)
        rsa_size = (TYPE_RSA_size)dlsym(RTLD_NEXT, "RSA_size");
    
    if (original_RSA_private_encrypt == NULL)
    {
        original_RSA_private_encrypt = dlsym(RTLD_NEXT, "RSA_private_encrypt");
        printf("hooked RSA_private_encrypt:%x\n",original_RSA_private_encrypt);
    }
    
    printf( "RSA_private_encryptHOOK: before encrypt\n" );
    DumpData( from, flen, 0x0 );
    
    int ret = original_RSA_private_encrypt(flen,from,to,rsa,padding);
    printf("after rsa private enc\n");
    return ret;
}


typedef int (*TYPE_RSA_public_decrypt) (int flen, const unsigned char *from,
                                        unsigned char *to, RSA *rsa,int padding);

TYPE_RSA_public_decrypt original_RSA_public_decrypt = NULL;

int     RSA_public_decrypt(int flen, const unsigned char *from,
                           unsigned char *to, RSA *rsa,int padding)
{
    FILE *fp;
    
    if (rsa_size == NULL)
        rsa_size = (TYPE_RSA_size)dlsym(RTLD_NEXT, "RSA_size");
    
    if (original_RSA_public_decrypt == NULL)
        original_RSA_public_decrypt = dlsym(RTLD_NEXT, "RSA_public_decrypt");
    
    
    printf( "RSA_public_decryptHOOK: FIND RSA PUB START\n" );
    
    fp = fopen( "/Users/merck/Desktop/public.key" , "a+");
    PEM_write_RSAPublicKey( fp, rsa );
    fclose( fp );
    
    printf( "RSA_public_decryptHOOK: FIND RSA PUB END\n" );

    
    
    if (original_RSA_public_decrypt == NULL)
    {
        void* lib = dlopen("/usr/lib/libcrypto.0.9.8.dylib", RTLD_NOW);
        original_RSA_public_decrypt = (TYPE_RSA_public_decrypt)dlsym(lib,"RSA_public_decrypt");
    }
    printf("hooked RSA_public_decrypt:%x\n",original_RSA_public_decrypt);
    int ret = original_RSA_public_decrypt(flen,from,to,rsa,padding);
    
    
    printf( "RSA_public_decryptHOOK: flen = %d, strlen(to) = %d\n", flen, strlen(to) );
    
    DumpData( to, flen, 0x0 );
    
    printf("after rsa public dec\n");
    return ret;
}

//RSA * d2i_RSAPrivateKey(RSA **a, const unsigned char **pp, long length);
typedef RSA* (*TYPE_d2i_RSAPrivateKey) (RSA **a,const unsigned char **pp,long length);
TYPE_d2i_RSAPrivateKey original_d2i_RSAPrivateKey = NULL;
RSA* d2i_RSAPrivateKey(RSA **a,const unsigned char **pp,long length)
{
    FILE *fp;
    
    if (original_d2i_RSAPrivateKey == NULL)
    {
        original_d2i_RSAPrivateKey = (TYPE_d2i_RSAPrivateKey)dlsym(RTLD_NEXT,"d2i_RSAPrivateKey");
        if (original_d2i_RSAPrivateKey == NULL)
        {
            void* lib = dlopen("/usr/lib/libcrypto.0.9.8.dylib", RTLD_NOW);
            original_d2i_RSAPrivateKey = (TYPE_d2i_RSAPrivateKey)dlsym(lib,"d2i_RSAPrivateKey");
        }
        printf("hooked d2i_rsa:%x\n",original_d2i_RSAPrivateKey);
    }
    
    void* ret = original_d2i_RSAPrivateKey(a,pp,length);
    printf("d2i_RSAPrivateKey:len=%d\n",length);
    
    
    printf( "d2i_RSAPrivateKeyHOOK: FIND D2IRSA PRIV START\n" );

    fp = fopen( "/Users/merck/Desktop/d2i_private.key" , "a+");
    //PEM_write_PrivateKey( fp, (RSA *)ret, NULL, NULL, 0, NULL, NULL );
    PEM_write_RSAPublicKey( fp, (RSA *)ret );
    fclose( fp );
    
    printf( "d2i_RSAPrivateKeyHOOK: FIND D2IRSA PRIV END\n" );
    
    
    DumpData( ret, length, 0x0 );
    
    
    return ret;
}

