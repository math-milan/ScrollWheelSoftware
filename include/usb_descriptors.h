/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef USB_DESCRIPTORS_H_
#define USB_DESCRIPTORS_H_

enum
{
  REPORT_ID_MOUSE = 1,
};


const uint8_t desc_hid_report[] = {
    0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,        // USAGE (Mouse)
    0xA1, 0x01,        // COLLECTION (Application)
        0x05, 0x01,    //   USAGE_PAGE (Generic Desktop)
        0x09, 0x02,    //   USAGE (Mouse)
        0xA1, 0x02,    //   COLLECTION (Logical)
            0x85, 0x01,  //   REPORT_ID (0x01)
            0x09, 0x01,  //   USAGE (Pointer)
            0xA1, 0x00,  //   COLLECTION (Physical)
                0x05, 0x09,  //   USAGE_PAGE (Buttons)
                0x19, 0x01,  //   USAGE_MINIMUM (1)
                0x29, 0x05,  //   USAGE_MAXIMUM (5)
                0x95, 0x05,  //   REPORT_COUNT (5)
                0x75, 0x01,  //   REPORT_SIZE (1)
                0x25, 0x01,  //   LOGICAL_MAXIMUM (1)
                0x81, 0x02,  //   INPUT (Var)
                0x75, 0x03,  //   REPORT_SIZE (3)
                0x95, 0x01,  //   REPORT_COUNT (1)
                0x81, 0x03,  //   INPUT (Constant)
                0x05, 0x01,  //   USAGE_PAGE (Generic Desktop)
                0x09, 0x30,  //   USAGE (X)
                0x09, 0x31,  //   USAGE (Y)
                0x95, 0x02,  //   REPORT_COUNT (2)
                0x75, 0x08,  //   REPORT_SIZE (8)
                0x15, 0x81,  //   LOGICAL_MINIMUM (-127)
                0x25, 0x7F,  //   LOGICAL_MAXIMUM (127)
                0x81, 0x06,  //   INPUT (Var, Rel)
                0xA1, 0x02,  //   COLLECTION (Logical)
                    0x85, 0x02,  //   REPORT_ID (0x02)
                    0x09, 0x48,  //   USAGE (Resolution Multiplier)
                    0x95, 0x01,  //   REPORT_COUNT (1)
                    0x75, 0x04,  //   REPORT_SIZE (4)
                    0x15, 0x00,  //   LOGICAL_MINIMUM (0)
                    0x25, 0x15,  //   LOGICAL_MAXIMUM (15)
                    0x35, 0x01,  //   PHYSICAL_MINIMUM (1)
                    0x45, 0x16,  //   PHYSICAL_MAXIMUM (16)
                    0xB1, 0x02,  //   FEATURE (Var)
                    0x85, 0x01,  //   REPORT_ID (0x01)
                    0x09, 0x38,  //   USAGE (Wheel)
                    0x35, 0x00,  //   PHYSICAL_MINIMUM (0)
                    0x45, 0x00,  //   PHYSICAL_MAXIMUM (0)
                    0x15, 0x81,  //   LOGICAL_MINIMUM (-127)
                    0x25, 0x7F,  //   LOGICAL_MAXIMUM (127)
                    0x75, 0x08,  //   REPORT_SIZE (8)
                    0x81, 0x06,  //   INPUT (Var, Rel)
                0xC0,        // END COLLECTION (Logical)
                0xA1, 0x02,  //   COLLECTION (Logical)
                    0x85, 0x02,  //   REPORT_ID (0x02)
                    0x09, 0x48,  //   USAGE (Resolution Multiplier)
                    0x75, 0x04,  //   REPORT_SIZE (4)
                    0x15, 0x00,  //   LOGICAL_MINIMUM (0)
                    0x25, 0x15,  //   LOGICAL_MAXIMUM (1)
                    0x35, 0x01,  //   PHYSICAL_MINIMUM (1)
                    0x45, 0x16,  //   PHYSICAL_MAXIMUM (4)
                    0xB1, 0x02,  //   FEATURE (Var)
                    0x35, 0x00,  //   PHYSICAL_MINIMUM (0)
                    0x45, 0x00,  //   PHYSICAL_MAXIMUM (0)
                    0x85, 0x01,  //   REPORT_ID (0x01)
                    0x05, 0x0C,  //   USAGE_PAGE (Consumer Devices)
                    0x15, 0x81,  //   LOGICAL_MINIMUM (-127)
                    0x25, 0x7F,  //   LOGICAL_MAXIMUM (127)
                    0x75, 0x08,  //   REPORT_SIZE (8)
                    0x09, 0x38,  //   USAGE (AC Pan)
                    0x81, 0x06,  //   INPUT (Var, Rel)
                0xC0,        // END COLLECTION (Logical)
            0xC0,        // END COLLECTION (Physical)
        0xC0,        // END COLLECTION (Logical)
    0xC0         // END COLLECTION (Application)
};


#endif /* USB_DESCRIPTORS_H_ */
