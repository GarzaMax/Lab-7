//
// Created by nivce on 07.02.2023.
//

#include "MP3Reader.h"


char* Header::GetFileId() {
    return file_id_;
}

char* Header::GetVersion() {
    return id3v2_version_;
}

char* Header::GetREvision() {
    return id3v2_revision_;
}

bool Header::GetUnsyncFlag() {
    return unsync_flag_;
}

bool Header::GetExtendedHeaderFlag() {
    return extended_header_flag_;
}

bool Header::GetExperimentalIndicatorFlag() {
    return experimental_indicator_flag_;
}

bool Header::GetFooterFlag() {
    return footer_flag_;
}

size_t Header::GetTagSize() {
    return tag_size_;
}


size_t FrameEntity::GetFrameSize() {
    return frame_size_;
}

std::string FrameEntity::GetFrameID() {
    return frame_id_;
}

bool FrameEntity::GetTagSaveFlag() {
    return tag_save_flag_;
}

bool FrameEntity::GetFileSaveFlag() {
    return file_save_flag_;
}

bool FrameEntity::GetReadOnlyFlag() {
    return read_only_flag_;
}

bool FrameEntity::GetGroupIdentityFlag() {
    return group_identity_flag_;
}

bool FrameEntity::GetCompressionFlag() {
    return compression_flag_;
}

bool FrameEntity::GetEncryptFlag() {
    return encrypt_flag_;
}

bool FrameEntity::GetUnsyncFlag() {
    return unsync_flag_;
}

bool FrameEntity::GetDataSizeIdFlag() {
    return data_size_indicator_flag_;
}
