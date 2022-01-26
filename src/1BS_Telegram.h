#pragma

#include "EnoceanProfils.h"

/*******************************************************************************************
 *  1BS
 *******************************************************************************************/

void handle_1BS(PACKET_SERIAL_TYPE *f_Pkt_st, uint8_t profil, uint16_t firstComObj, uint16_t firstParameter)
{
    bool bvalue;

    ONEBS_TELEGRAM_TYPE *l1bsTlg_p;

#ifdef KDEBUG
    SERIAL_PORT.print("Profil: 1BS - ");
    SERIAL_PORT.println(firstParameter + ENO_CHProfilSelection4BS);
#endif

    switch (knx.paramWord(firstParameter + ENO_CHProfilSelection1BS))
    {
    case D5_00_01:
#ifdef KDEBUG
        SERIAL_PORT.println("D5-00-01");
#endif
        l1bsTlg_p = (ONEBS_TELEGRAM_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);

        // D5-00-01 Contact: 0 = open / 1 = close
        // ETS Parameter to define state for open / Close
        if (l1bsTlg_p->u81bsTelData.State == 1) // CLOSE
        {
            if (((knx.paramByte(firstParameter + ENO_CHWindowcloseValue)) >> ENO_CHWindowcloseValueShift) & 1)
                bvalue = true;
            else
                bvalue = false;
        }
        else
        { // OPEN
            if (((knx.paramByte(firstParameter + ENO_CHWindowcloseValue)) >> ENO_CHWindowcloseValueShift) & 1)
                bvalue = false;
            else
                bvalue = true;
        }
        knx.getGroupObject(firstComObj).value(bvalue, getDPT(VAL_DPT_1));

#ifdef KDEBUG
        SERIAL_PORT.print(F("detected: 1BS State: "));
        SERIAL_PORT.println(l1bsTlg_p->u81bsTelData.State);
#endif
        break;

   

    default:
        break;
    }
}