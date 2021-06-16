#pragma once
#include <knx.h>

// Parameter with single occurance
#define ENO_StartupDelay               0      // int32_t
#define ENO_StartupDelaySelection      4      // char*, 2 Byte
#define ENO_Heartbeat                  6      // int32_t

// Parameter per channel
#define ENO_ParamBlockOffset 10
#define ENO_ParamBlockSize 19
#define ENO_CHProfilSelection          0      // 8 Bits, Bit 7-0
#define ENO_CHProfilSelectionRPS       1      // 8 Bits, Bit 7-0
#define ENO_CHProfilSelection1BS       1      // 8 Bits, Bit 7-0
#define ENO_CHProfilSelection4BS       1      // 8 Bits, Bit 7-0
#define ENO_CHProfilSelectionVLD       1      // 8 Bits, Bit 7-0
#define ENO_CHDirectionKnxEnocean      2      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS02              3      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS04              3      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS05              3      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS06              3      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS07              3      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS08              3      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS09              3      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS10              3      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS11              3      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS12              3      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS13              3      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS14              3      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS20              3      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS30              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD00              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD01              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD02              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD03              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD04              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD05              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD06              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD10              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD11              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD20              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD30              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD31              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD32              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD33              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD34              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD40              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLD50              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLDA0              3      // 8 Bits, Bit 7-0
#define ENO_CHProfilVLDB0              3      // 8 Bits, Bit 7-0
#define ENO_CHRockerProfil             4      // 8 Bits, Bit 7-0
#define ENO_CHRockerFunktionA          5      // 8 Bits, Bit 7-0
#define ENO_CHRockerASzeneA            6      // 8 Bits, Bit 7-0
#define ENO_CHRockerASzeneB            7      // 8 Bits, Bit 7-0
#define ENO_CHRockerFunktionB          8      // 8 Bits, Bit 7-0
#define ENO_CHRockerBSzeneA            9      // 8 Bits, Bit 7-0
#define ENO_CHRockerBSzeneB           10      // 8 Bits, Bit 7-0
#define ENO_CHRockerFunktionC         11      // 8 Bits, Bit 7-0
#define ENO_CHRockerCSzeneA           12      // 8 Bits, Bit 7-0
#define ENO_CHRockerCSzeneB           13      // 8 Bits, Bit 7-0
#define ENO_CHProfilRockerFunktionKNX 14      // 8 Bits, Bit 7-0
#define ENO_CHId0                     15      // 4 Bits, Bit 7-4
#define     ENO_CHId0Mask 0xF0
#define     ENO_CHId0Shift 4
#define ENO_CHId1                     15      // 4 Bits, Bit 3-0
#define     ENO_CHId1Mask 0x0F
#define     ENO_CHId1Shift 0
#define ENO_CHId2                     16      // 4 Bits, Bit 7-4
#define     ENO_CHId2Mask 0xF0
#define     ENO_CHId2Shift 4
#define ENO_CHId3                     16      // 4 Bits, Bit 3-0
#define     ENO_CHId3Mask 0x0F
#define     ENO_CHId3Shift 0
#define ENO_CHId4                     17      // 4 Bits, Bit 7-4
#define     ENO_CHId4Mask 0xF0
#define     ENO_CHId4Shift 4
#define ENO_CHId5                     17      // 4 Bits, Bit 3-0
#define     ENO_CHId5Mask 0x0F
#define     ENO_CHId5Shift 0
#define ENO_CHId6                     18      // 4 Bits, Bit 7-4
#define     ENO_CHId6Mask 0xF0
#define     ENO_CHId6Shift 4
#define ENO_CHId7                     18      // 4 Bits, Bit 3-0
#define     ENO_CHId7Mask 0x0F
#define     ENO_CHId7Shift 0
#define ENO_CHProfilFunktionKNX       16      // 8 Bits, Bit 7-0
#define ENO_CHVLDProfilFunktionKNX    17      // 8 Bits, Bit 7-0

// Communication objects per channel (multiple occurance)
#define ENO_KoOffset 10
#define ENO_KoBlockSize 5
#define ENO_KoGO_BASE__1 0
#define ENO_KoGO_BASE__2 1
#define ENO_KoGO_BASE__3 2
#define ENO_KoGO_BASE__4 3
#define ENO_KoGO_BASE__5 4

// Communication objects with single occurance
#define ENO_KoHeartbeat 1

