/**
 * @brief Lookup tables: represents an ID as a human readable values.
 * Most of the data comes from the following projects:
 * 1. https://github.com/open-power/errl
 * 2. https://github.com/open-power/libopalevents
 * 3. https://github.com/open-power/hostboot
 *
 * Copyright (c) 2019 YADRO
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ltables.hpp"

namespace eSEL
{

// clang-format off

const LookupTable<uint8_t> SubsystemName = {
   { 0x00, "Not Applicable" },
   { 0x10, "Processor subsystem" },
   { 0x11, "Processor FRU" },
   { 0x12, "Processor chip including internal cache" },
   { 0x13, "Processor unit (CPU)" },
   { 0x14, "Processor/system bus controller & interface" },
   { 0x20, "Memory subsystem" },
   { 0x21, "Memory controller" },
   { 0x22, "Memory bus interface including SMI" },
   { 0x23, "Memory DIMM" },
   { 0x24, "Memory card/FRU" },
   { 0x25, "External cache" },
   { 0x30, "I/O (hub, bridge, bus)" },
   { 0x31, "I/O hub RIO" },
   { 0x32, "I/O bridge, general (PHB, PCI/PCI, PCI/ISA, EADS, etc.)" },
   { 0x33, "I/O bus interface" },
   { 0x34, "I/O processor" },
   { 0x35, "I/O hub others (SMA, Torrent, etc.)" },
   { 0x36, "RIO loop and associated RIO hub" },
   { 0x37, "RIO loop and associated RIO bridge" },
   { 0x38, "PHB" },
   { 0x39, "EADS/EADS-X global" },
   { 0x3a, "EADS/EADS-X slot" },
   { 0x3b, "InfiniBand hub" },
   { 0x3c, "Infiniband bridge" },
   { 0x40, "I/O (adapter, device, peripheral)" },
   { 0x41, "I/O adapter - communication" },
   { 0x46, "I/O device" },
   { 0x47, "I/O device - DASD" },
   { 0x4c, "I/O peripheral" },
   { 0x4d, "I/O peripheral - local workstation" },
   { 0x4e, "Storage mezzanine expansion subsystem" },
   { 0x50, "CEC Hardware" },
   { 0x51, "CEC Hardware - service processor A" },
   { 0x52, "CEC Hardware - service processor B" },
   { 0x53, "CEC Hardware - node controller" },
   { 0x54, "Reserved for CEC hardware" },
   { 0x55, "CEC hardware - VPD device and interface (smart chip and I2C device)" },
   { 0x56, "CEC hardware - I2C devices and interface (non VPD)" },
   { 0x57, "CEC hardware - CEC chip interface (JTAG, FSI, etc.)" },
   { 0x57, "CEC hardware - CEC chip interface (JTAG, FSI, etc.)" },
   { 0x58, "CEC hardware - clock & control" },
   { 0x59, "CEC hardware - Op. panel" },
   { 0x5a, "CEC hardware - time of day hardware including its battery" },
   { 0x5b, "CEC hardware - storage/memory device (NVRAM, Flash, SP DRAM, etc.)" },
   { 0x5c, "CEC hardware - Service processor-Hypervisor hardware interface (PSI, PCI, etc.)" },
   { 0x5d, "CEC hardware - Service network" },
   { 0x5e, "CEC hardware - Service processor-Hostboot hardware interface (FSI Mailbox)" },
   { 0x60, "Power/Cooling System" },
   { 0x61, "Power supply" },
   { 0x62, "Power control hardware" },
   { 0x63, "Fan, air moving devices" },
   { 0x64, "DPSS" },
   { 0x70, "Other Subsystems" },
   { 0x71, "Hypervisor subsystem & hardware (excluding code)" },
   { 0x72, "Test tool" },
   { 0x73, "Removable media" },
   { 0x74, "Multiple subsystems" },
   { 0x75, "Not applicable (unknown, invalid value, etc.)" },
   { 0x76, "Reserved" },
   { 0x77, "CMM A" },
   { 0x78, "CMM B" },
   { 0x7a, "Connection Monitoring - Hypervisor lost communication with service processor" },
   { 0x7b, "Connection Monitoring - Service processor lost communication with hypervisor" },
   { 0x7c, "Connection Monitoring - Service processor lost communication with hypervisor" },
   { 0x7e, "Connection Monitoring - Hypervisor lost communication with logical partition" },
   { 0x7e, "Connection Monitoring - Hypervisor lost communication with BPA" },
   { 0x7f, "Connection Monitoring - Hypervisor lost communication with another hypervisor" },
   { 0x80, "Platform Firmware" },
   { 0x81, "Service processor firmware" },
   { 0x82, "Hypervisor firmware" },
   { 0x83, "Partition firmware" },
   { 0x84, "SLIC firmware" },
   { 0x85, "SPCN firmware" },
   { 0x86, "Bulk power firmware side A" },
   { 0x87, "Hypervisor code/firmware" },
   { 0x88, "Bulk power firmware side B" },
   { 0x89, "Virtual service processor firmware (VSP)" },
   { 0x8a, "Hostboot" },
   { 0x8b, "OCC" },
   { 0x90, "Software" },
   { 0x91, "Operating system software" },
   { 0x92, "XPF software" },
   { 0x93, "Application software" },
   { 0xa0, "External Environment" },
   { 0xa1, "Input power source (AC)" },
   { 0xa2, "Room ambient temperature" },
   { 0xa3, "User error" }
};

const LookupTable<uint8_t> EventSeverity = {
   { 0x00, "Informational Event" },
   { 0x10, "Recoverable Error" },
   { 0x20, "Predictive Error" },
   { 0x21, "Predicting degraded performance." },
   { 0x22, "Predicting fault may be corrected after platform re-IPL." },
   { 0x23, "Predicting fault may be corrected after IPL, degraded performance" },
   { 0x24, "Predicting loss of redundancy" },
   { 0x40, "Unrecoverable Error" },
   { 0x41, "Error bypassed with degraded performance" },
   { 0x44, "Error bypassed with loss of redundancy" },
   { 0x45, "Error bypassed with loss of redundancy and performance" },
   { 0x48, "Error bypassed with loss of function" },
   { 0x50, "Critical Error" },
   { 0x51, "Critical error system termination" },
   { 0x52, "Critical error failure likely or imminent" },
   { 0x53, "Critical error partition(s) terminal" },
   { 0x54, "Critical error partition(s) failure likely or imminent" },
   { 0x60, "Error on diag test" },
   { 0x61, "Error on diag test, resource may produce incorrect results" },
   { 0x70, "Symptom" },
   { 0x71, "Symptom recovered" },
   { 0x72, "Symptom predictive" },
   { 0x74, "Symptom unrecoverable" },
   { 0x75, "Symptom critical" },
   { 0x76, "Symptom diagnosis error" }
};

const LookupTable<uint8_t> EventScope = {
   { 0x01, "Single partition" },
   { 0x02, "Multiple partitions" },
   { 0x03, "Single platform" },
   { 0x04, "Possibly multiple platforms" }
};

const LookupTable<uint8_t> EventType = {
   { 0x00, "Not applicable" },
   { 0x01, "Miscellaneous, informational only." },
   { 0x02, "Tracing event" },
   { 0x08, "Dump notification" },
   { 0x10, "Previously reported error has been corrected by system" },
   { 0x20, "System resources manually deconfigured by user" },
   { 0x21, "System resources deconfigured by system due to prior error event" },
   { 0x22, "Resource deallocation event notification" },
   { 0x30, "Customer environmental problem has returned to normal" },
   { 0x40, "Concurrent maintenance event" },
   { 0x60, "Capacity upgrade event" },
   { 0x70, "Resource sparing event" },
   { 0x80, "Dynamic reconfiguration event" },
   { 0xD0, "Normal system/platform shutdown or powered off" },
   { 0xE0, "Platform powered off by user without normal shutdown" }
};

// clang-format on

} // namespace eSEL
