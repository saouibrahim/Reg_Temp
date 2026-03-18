#include "include/releve.h"

void releve(temp_t *temperature, FT_HANDLE ftHandle)
{
    FT_STATUS ftStatus = FT_Open(0, &ftHandle);
    if (ftStatus != FT_OK)
    {
        // FT_Open failed
        printf("FT_Open failed: %d\n", ftStatus);
        return;
    }

    // Check how many bytes are waiting in the receive queue
    FT_GetStatus(ftHandle, &RxBytes, &TxBytes, &EventDWord);
    if (RxBytes >= sizeof(RxBuffer))
    {
        ftStatus = FT_Read(ftHandle, RxBuffer, sizeof(RxBuffer), &BytesReceived); // read 2 Bytes and put it into the buffer
        if (ftStatus == FT_OK)
        {

            // FT_Read OK
            pprintf("Read OK — Bytes received: %lu\n", BytesReceived);

            __int16 sot_ext = 0;
            __int16 sot_int = 0;
            __int8 ext_found = 0; // bitmask: bit0=byte1 found, bit1=byte2, bit2=byte3
            __int8 int_found = 0; // idem here,, also bitmask for bit 0 found etc...

            for (int i = 0; i < 6; i++)
            {
                __int8 byte = (__int8)RxBuffer[i];
                __int8 id = (byte >> 4) & 0x0F; // top nibble = identifier (MSB part of the byte, 4 first bits from the left)
                __int8 data = byte & 0x0F;      // bottom nibble = SOT bits (LSB part of the byte, 4 last bits from the left)

                switch (id)
                {
                case 0x0: // EXT byte 1: SOT bits 11-8
                    sot_ext |= ((__int16)data << 8);
                    ext_found |= (1 << 0);
                    break;
                case 0x1: // EXT byte 2: SOT bits 7-4
                    sot_ext |= ((__int16)data << 4);
                    ext_found |= (1 << 1);
                    break;
                case 0x2: // EXT byte 3: SOT bits 3-0
                    sot_ext |= ((__int16)data);
                    ext_found |= (1 << 2);
                    break;
                case 0x8: // INT byte 1: SOT bits 11-8
                    sot_int |= ((__int16)data << 8);
                    int_found |= (1 << 0);
                    break;
                case 0x9: // INT byte 2: SOT bits 7-4
                    sot_int |= ((__int16)data << 4);
                    int_found |= (1 << 1);
                    break;
                case 0xA: // INT byte 3: SOT bits 3-0
                    sot_int |= ((__int16)data);
                    int_found |= (1 << 2);
                    break;
                default:
                    printf("Unknown byte id: 0x%X at index %d\n", id, i);
                    break;
                }
            }

            // Only assign if all 3 bytes of each sensor were received
            if (ext_found == 0x7)
            {
                temperature->exterieure = sot_ext;
            }
            else
            {
                printf("Incomplete exterior temperature data (found mask: 0x%X)\n", ext_found);
            }

            if (int_found == 0x7)
            {
                temperature->interieure = sot_int;
            }
            else
            {
                printf("Incomplete interior temperature data (found mask: 0x%X)\n", int_found);
            }
        }
        else
        {
            // FT_Read Failed
            printf("FT_Read failed: %d\n", ftStatus);
            FT_Close(ftHandle);
            return;
        }
    }
    FT_Close(ftHandle);
}