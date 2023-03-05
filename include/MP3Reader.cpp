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


