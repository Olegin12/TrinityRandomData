#include <iostream>
#include <stdint.h>
#include <fstream>

    struct VersionHeader
    {
        uint16_t major_version;
        uint16_t minor_version;
    };

namespace DeviceType
{
    enum DeviceType : uint16_t
    {
        UNKNOWN     = 00,
        REGISTRATOR = 01,
        VALVE       = 02,
        MANDREL     = 03
    };
}


struct DeviceListHeader // well_device_card
{
    uint16_t well_id;
    uint16_t device_id;
    uint16_t device_type;
    uint16_t sensor_number;
    uint16_t installation_depth;
    uint16_t reserved[9];
};

namespace SensorType
{
    enum SensorType : uint16_t
    {
        UNKNOWN        = 0,
        PRESS_INTERNAL = 1,
        PRESS_EXTERNAL = 2,
        TEMP_INTERNAL  = 3,
        TEMP_EXTERNAL  = 4,
        PPR            = 5, // Pulse per revolution
        MOTOR_CURRENT  = 6,
        MOTOR_TEMP     = 7,
        HEAD_VOLTAGE   = 8,
        BOARD_VOLTAGE  = 9,
        BOARD_TEMP     = 10,
        CTRL_1         = 11,
        CTRL_2         = 12
    };
}

namespace DataType
{
    enum DataType : uint16_t
    {
        UNKNOWN = 0,
        UINT_16_DATA = 1,
        UINT_32_DATA = 2,
        FLOAT_DATA   = 3
    };
}

namespace VisualisationType
{
    enum VisualisationType : uint16_t
    {
        UNKNOWN       = 0,
        VISUALISATION = 1,
        LOGGING       = 2,
        CONTROL       = 3
    };
}

namespace UnitsType
{
    enum UnitsType : uint16_t
    {
        UNKNOWN = 0,
        CELSIUM = 1,
        MPA     = 2,
        VOLTS   = 3,
        RPM     = 4,
        AMPERS  = 5,
        ADC     = 6,
        METERS  = 7,
        SANTIMETERS = 8,
        FOOTS   = 9
    };
}


struct ChannelListHeader
{
    uint16_t device_id;
    uint16_t sensor_type;
    uint16_t data_type;
    uint16_t visual_type;
    uint16_t hw_version;
    uint16_t fw_version;
    uint16_t sensor_depth_offset;
    uint16_t calibration_flag;
    uint16_t units_type;
    uint16_t reserved[7];
};


using std::cout;
using std::endl;
using std::ofstream;
using std::ios;

int main()
{
    // Open binary file
    ofstream wf("data.bin", ios::out | ios::binary);

    if(!wf) {
        cout << "Cannot open file!" << endl;
        return 1;
    }

    // WRITE PROTOCOL VERSION ==========================================================================================

    // Protocol version
    VersionHeader version_header;
    version_header.major_version = 00;
    version_header.minor_version = 00;

    wf.write((char *) &version_header, sizeof(version_header));

    // WRITE DEVICE LIST ===============================================================================================

    uint16_t device_number = 2;
    wf.write((char *) &device_number, 2);

    for (int i = 0; i < device_number; i++)
    {
        // For device list
        DeviceListHeader device_list_header;
        device_list_header.well_id = 1001;
        device_list_header.device_id = 2001;
        device_list_header.device_type = DeviceType::DeviceType::MANDREL;
        device_list_header.sensor_number = 2;
        device_list_header.installation_depth = 100; // cm

        wf.write((char *) &device_list_header, sizeof(device_list_header));
    }

    // Write CHANNEL LIST ==============================================================================================

    uint16_t channel_number = 4;
    wf.write((char *) &channel_number, 2);

    for (int i = 0; i < channel_number; i++)
    {
        // For header list
        ChannelListHeader channel_list_header;
        channel_list_header.device_id = 2001;
        channel_list_header.sensor_type = SensorType::SensorType::PRESS_INTERNAL;
        channel_list_header.data_type   = DataType::DataType::FLOAT_DATA;
        channel_list_header.visual_type = VisualisationType::VisualisationType::VISUALISATION;
        channel_list_header.hw_version = 1;
        channel_list_header.fw_version = 1;
        channel_list_header.sensor_depth_offset = 100;
        channel_list_header.calibration_flag = 1;
        channel_list_header.units_type = UnitsType::UnitsType::MPA;

        wf.write((char *) &channel_list_header, sizeof(channel_list_header));
    }

    // Close binary file
    wf.close();
    if(!wf.good()) {
        cout << "Error occurred at writing time!" << endl;
        return 1;
    }

    std::cout << "Done!" << std::endl;
    return 0;
}
