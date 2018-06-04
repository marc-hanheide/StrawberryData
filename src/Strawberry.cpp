#include "Strawberry.hpp"

Strawberry::DataStructure::DataStructure(const rs2::device &device) :
        DataStructure(device.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER)) {}

Strawberry::DataStructure::DataStructure(const char *device_serial_number) :
        DataStructure(std::string(device_serial_number)) {}

Strawberry::DataStructure::DataStructure(std::string device_serial_number) {
    serial_number_ = device_serial_number;
    parent_ = boost::filesystem::path("data/" + serial_number_ + "/");
}

const void Strawberry::DataStructure::UpdateFolderPaths() {
    UpdateTimestamp();

    folder_ = boost::filesystem::path(parent_.string() + date_ + "/");
    sub_folder_ = boost::filesystem::path(folder_.string() + time_ + "/");

    // Create directories if they do not exist
    if(!boost::filesystem::is_directory(sub_folder_) || !boost::filesystem::exists(sub_folder_))
        boost::filesystem::create_directories(sub_folder_);
}

const std::string Strawberry::DataStructure::FilePath(RsType file_type, bool meta) {
    auto i = static_cast<int>(file_type);
    return sub_folder_.string() + file_names_[i] + ext_[meta ? 2 : file_type == RsType::POINT_CLOUD ? 1 : 0];
}

const void Strawberry::DataStructure::UpdateTimestamp() {
    std::chrono::high_resolution_clock::time_point p = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(p.time_since_epoch());
    std::time_t t = std::chrono::duration_cast<std::chrono::seconds>(ms).count();

    std::stringstream date, time;
    date <<  std::put_time(std::localtime(&t), "%Y_%m_%d");
    time << std::put_time(std::localtime(&t), "%H_%M_%S_") << std::setfill('0') << std::setw(3) << ms.count() % 1000;
    date_ = date.str();
    time_ = time.str();
}