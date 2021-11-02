#pragma once
#include <knx.h>

// Parameter with single occurance
#define ENO_StartupDelay               0      // int32_t
#define ENO_StartupDelaySelection      4      // char*, 2 Byte
#define ENO_Heartbeat                  6      // int32_t
#define ENO_RepeaterFunc              10      // 1 Bit, Bit 7
#define     ENO_RepeaterFuncMask 0x80
#define     ENO_RepeaterFuncShift 7
#define ENO_SetBaseIdFunc             11      // 1 Bit, Bit 7
#define     ENO_SetBaseIdFuncMask 0x80
#define     ENO_SetBaseIdFuncShift 7
#define ENO_Id0                       12      // 4 Bits, Bit 7-4
#define     ENO_Id0Mask 0xF0
#define     ENO_Id0Shift 4
#define ENO_Id1                       12      // 4 Bits, Bit 3-0
#define     ENO_Id1Mask 0x0F
#define     ENO_Id1Shift 0
#define ENO_Id2                       13      // 4 Bits, Bit 7-4
#define     ENO_Id2Mask 0xF0
#define     ENO_Id2Shift 4
#define ENO_Id3                       13      // 4 Bits, Bit 3-0
#define     ENO_Id3Mask 0x0F
#define     ENO_Id3Shift 0
#define ENO_Id4                       14      // 4 Bits, Bit 7-4
#define     ENO_Id4Mask 0xF0
#define     ENO_Id4Shift 4
#define ENO_Id5                       14      // 4 Bits, Bit 3-0
#define     ENO_Id5Mask 0x0F
#define     ENO_Id5Shift 0
#define ENO_Id6                       15      // 4 Bits, Bit 7-4
#define     ENO_Id6Mask 0xF0
#define     ENO_Id6Shift 4
#define ENO_Id7                       15      // 4 Bits, Bit 3-0
#define     ENO_Id7Mask 0x0F
#define     ENO_Id7Shift 0

// Parameter per channel
#define ENO_ParamBlockOffset 16
#define ENO_ParamBlockSize 22
#define ENO_CHProfilSelection          0      // 8 Bits, Bit 7-0
#define ENO_CHProfilSelectionRPS       1      // char*, 2 Byte
#define ENO_CHProfilSelection1BS       1      // char*, 2 Byte
#define ENO_CHProfilSelection4BS       1      // char*, 2 Byte
#define ENO_CHProfilSelectionVLD       1      // char*, 2 Byte
#define ENO_CHDirectionKnxEnocean      3      // 8 Bits, Bit 7-0
#define ENO_CHProfil4BS02              4      // char*, 2 Byte
#define ENO_CHProfil4BS04              4      // char*, 2 Byte
#define ENO_CHProfil4BS05              4      // char*, 2 Byte
#define ENO_CHProfil4BS06              4      // char*, 2 Byte
#define ENO_CHProfil4BS07              4      // char*, 2 Byte
#define ENO_CHProfil4BS08              4      // char*, 2 Byte
#define ENO_CHProfil4BS09              4      // char*, 2 Byte
#define ENO_CHProfil4BS10              4      // char*, 2 Byte
#define ENO_CHProfil4BS11              4      // char*, 2 Byte
#define ENO_CHProfil4BS12              4      // char*, 2 Byte
#define ENO_CHProfil4BS13              4      // char*, 2 Byte
#define ENO_CHProfil4BS14              4      // char*, 2 Byte
#define ENO_CHProfil4BS20              4      // char*, 2 Byte
#define ENO_CHProfil4BS30              4      // char*, 2 Byte
#define ENO_CHProfilVLD00              4      // char*, 2 Byte
#define ENO_CHProfilVLD01              4      // char*, 2 Byte
#define ENO_CHProfilVLD02              4      // char*, 2 Byte
#define ENO_CHProfilVLD03              4      // char*, 2 Byte
#define ENO_CHProfilVLD04              4      // char*, 2 Byte
#define ENO_CHProfilVLD05              4      // char*, 2 Byte
#define ENO_CHProfilVLD06              4      // char*, 2 Byte
#define ENO_CHProfilVLD10              4      // char*, 2 Byte
#define ENO_CHProfilVLD11              4      // char*, 2 Byte
#define ENO_CHProfilVLD20              4      // char*, 2 Byte
#define ENO_CHProfilVLD30              4      // char*, 2 Byte
#define ENO_CHProfilVLD31              4      // char*, 2 Byte
#define ENO_CHProfilVLD32              4      // char*, 2 Byte
#define ENO_CHProfilVLD33              4      // char*, 2 Byte
#define ENO_CHProfilVLD34              4      // char*, 2 Byte
#define ENO_CHProfilVLD40              4      // char*, 2 Byte
#define ENO_CHProfilVLD50              4      // char*, 2 Byte
#define ENO_CHProfilVLDA0              4      // char*, 2 Byte
#define ENO_CHProfilVLDB0              4      // char*, 2 Byte
#define ENO_CHProfilVLD14              4      // char*, 2 Byte
#define ENO_CHRockerProfil             6      // 8 Bits, Bit 7-0
#define ENO_CHRockerFunktionA          7      // 8 Bits, Bit 7-0
#define ENO_CHRockerASzeneA            8      // 8 Bits, Bit 7-0
#define ENO_CHRockerASzeneB            9      // 8 Bits, Bit 7-0
#define ENO_CHRockerFunktionB         10      // 8 Bits, Bit 7-0
#define ENO_CHRockerBSzeneA           11      // 8 Bits, Bit 7-0
#define ENO_CHRockerBSzeneB           12      // 8 Bits, Bit 7-0
#define ENO_CHRockerFunktionC         13      // 8 Bits, Bit 7-0
#define ENO_CHRockerCSzeneA           14      // 8 Bits, Bit 7-0
#define ENO_CHRockerCSzeneB           15      // 8 Bits, Bit 7-0
#define ENO_CHProfilRockerFunktionKNX 16      // 8 Bits, Bit 7-0
#define ENO_CHId0                     17      // 4 Bits, Bit 7-4
#define     ENO_CHId0Mask 0xF0
#define     ENO_CHId0Shift 4
#define ENO_CHId1                     17      // 4 Bits, Bit 3-0
#define     ENO_CHId1Mask 0x0F
#define     ENO_CHId1Shift 0
#define ENO_CHId2                     18      // 4 Bits, Bit 7-4
#define     ENO_CHId2Mask 0xF0
#define     ENO_CHId2Shift 4
#define ENO_CHId3                     18      // 4 Bits, Bit 3-0
#define     ENO_CHId3Mask 0x0F
#define     ENO_CHId3Shift 0
#define ENO_CHId4                     19      // 4 Bits, Bit 7-4
#define     ENO_CHId4Mask 0xF0
#define     ENO_CHId4Shift 4
#define ENO_CHId5                     19      // 4 Bits, Bit 3-0
#define     ENO_CHId5Mask 0x0F
#define     ENO_CHId5Shift 0
#define ENO_CHId6                     20      // 4 Bits, Bit 7-4
#define     ENO_CHId6Mask 0xF0
#define     ENO_CHId6Shift 4
#define ENO_CHId7                     20      // 4 Bits, Bit 3-0
#define     ENO_CHId7Mask 0x0F
#define     ENO_CHId7Shift 0
#define ENO_CHA52006RFC               21      // 3 Bits, Bit 6-4
#define     ENO_CHA52006RFCMask 0x70
#define     ENO_CHA52006RFCShift 4
#define ENO_CHA52006SPS               21      // 1 Bit, Bit 2
#define     ENO_CHA52006SPSMask 0x04
#define     ENO_CHA52006SPSShift 2
#define ENO_CHA52006TSL               21      // 1 Bit, Bit 1
#define     ENO_CHA52006TSLMask 0x02
#define     ENO_CHA52006TSLShift 1
#define ENO_CHWindowcloseValue        21      // 1 Bit, Bit 7
#define     ENO_CHWindowcloseValueMask 0x80
#define     ENO_CHWindowcloseValueShift 7

// Communication objects per channel (multiple occurance)
#define ENO_KoOffset 10
#define ENO_KoBlockSize 10
#define ENO_KoGO_BASE__1 0
#define ENO_KoGO_BASE__2 1
#define ENO_KoGO_BASE__3 2
#define ENO_KoGO_BASE__4 3
#define ENO_KoGO_BASE__5 4
#define ENO_KoGO_BASE__6 5
#define ENO_KoGO_BASE__7 6
#define ENO_KoGO_BASE__8 7
#define ENO_KoGO_BASE__9 8
#define ENO_KoGO_BASE__10 9

// Communication objects with single occurance
#define ENO_KoHeartbeat 1

