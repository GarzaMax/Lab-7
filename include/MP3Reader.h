#pragma once

#include <any>
#include <bitset>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <utility>

std::string RecogniseFrameType(std::string FrameTag) {

    if (FrameTag == "TXXX") {
        return "TXXXFrame";
    } else if (FrameTag[0] == 'T') {
        return "TextFrame";
    } else if (FrameTag == "UFID") {
        return "UFIDFrame";
    } else if (FrameTag == "MCDI") {
        return "MCDIFrame";
    } else if (FrameTag == "WXXX") {
        return "WXXXFrame";
    } else if (FrameTag[0] == 'W') {
        return "URLFrame";
    } else if (FrameTag == "ECTO") {
        return "ECTOFrame";
    } else if (FrameTag == "USLT") {
        return "USLTFrame";
    } else if (FrameTag == "SYLT") {
        return "SYLTFrame";
    } else if (FrameTag == "COMM") {
        return "COMMFrame";
    } else if (FrameTag == "RVA2") {
        return "RVA2Frame";
    } else if (FrameTag == "EQU2") {
        return "EQU2Frame";
    } else if (FrameTag == "PCNT") {
        return "PCNTFrame";
    } else if (FrameTag == "POPM") {
        return "POPMFrame";
    } else if (FrameTag == "RBUF") {
        return "RBUFFrame";
    } else if (FrameTag == "LINK") {
        return "LINKFrame";
    } else if (FrameTag == "POSS") {
        return "POSSFrame";
    } else if (FrameTag == "USER") {
        return "USERFrame";
    } else if (FrameTag == "OWNE") {
        return "OWNEFrame";
    } else if (FrameTag == "COMR") {
        return "COMRFrame";
    } else if (FrameTag == "ENCR") {
        return "ENCRFrame";
    } else if (FrameTag == "GRID") {
        return "GRIDFrame";
    } else if (FrameTag == "PRIV") {
        return "PRIVFrame";
    } else {
        return "NotRecognized";
    }

}

class Header {
private:
    std::string file_id_;
    short id3v2_version_;
    short id3v2_revision_;
    bool unsync_flag_;
    bool extended_header_flag_;
    bool experimental_indicator_flag_;
    bool footer_flag_;
    size_t tag_size_;
public:
    Header(std::ifstream& file) {
        for (int i = 0; i < 3; i++) file_id_ += file.get();
        id3v2_version_ = (short) file.get();
        id3v2_revision_ = (short) file.get();
        std::bitset<8> flags_container((int) file.get());
        unsync_flag_ = flags_container[7];
        extended_header_flag_ = flags_container[6];
        experimental_indicator_flag_ = flags_container[5];
        footer_flag_ = flags_container[4];
        std::string tmp_size_container;
        for (int i = 0; i < 4; i++) tmp_size_container += file.get();
        tag_size_ = static_cast<uint8_t>(tmp_size_container[0]) << 24U |
                    static_cast<uint8_t>(tmp_size_container[1]) << 16U |
                    static_cast<uint8_t>(tmp_size_container[2]) << 8U |
                    static_cast<uint8_t>(tmp_size_container[3]);
    }

    ~Header() = default;

    Header& operator=(Header other) {
        file_id_ = other.file_id_;
        id3v2_version_ = other.id3v2_version_;
        id3v2_revision_ = other.id3v2_revision_;
        unsync_flag_ = other.unsync_flag_;
        extended_header_flag_ = other.extended_header_flag_;
        experimental_indicator_flag_ = other.experimental_indicator_flag_;
        footer_flag_ = other.footer_flag_;
        tag_size_ = other.tag_size_;
    }

    std::string GetFileID();

    short GetIDVersion() const;

    short GetIDRevision() const;

    bool GetUnsyncFlag() const;

    bool GetExperimentalFlag() const;

    bool GetExtendedHeaderFlag() const;

    bool GetFooterFlag() const;

    size_t GetTagSize() const;
};

class ExtendedHeader {
private:
    size_t extended_header_size_;
public:

};

class FrameEntity {
private:
    std::string frame_id_;
    size_t frame_size_;
    bool tag_save_flag_;
    bool file_save_flag_;
    bool read_only_flag_;
    bool group_identity_flag_;
    bool compression_flag_;
    bool encrypt_flag_;
    bool unsync_flag_;
    bool data_size_indicator_flag_;

public:
    FrameEntity(std::ifstream& file);

    ~FrameEntity() = default;

    std::string GetFrameID();

    size_t GetFrameSize() const;

    bool GetTagSaveFlag() const;

    bool GetFileSaveFlag() const;

    bool GetReadOnlyFlag();

    bool GetGroupIdentityFlag() const;

    bool GetCompressionFlag() const;

    bool GetEncryptFlag() const;

    bool GetUnsyncFlag() const;

    bool GetDataSizeIdFlag() const;

};

// UFID Frame definition

class UFIDFrame : public FrameEntity {
private:
    std::string owner_header_;
    std::bitset<64> binary_data_;
public:

    UFIDFrame(std::ifstream& file);

    ~UFIDFrame() = default;

};

// Music CD ID definition

class MCDI : public FrameEntity {
private:
    std::vector<unsigned char> cd_header_;
public:
    MCDI(std::ifstream& file);
};

// Text Frames section

class TextFrame : public FrameEntity {
private:
    unsigned char text_encode_;
    std::string info_string_;
public:
    TextFrame(std::ifstream& file);
};

class TXXXFrame : public FrameEntity {
private:
    unsigned char text_encode_;
    std::string info_string_;
    std::string description_;
public:
    TXXXFrame(std::ifstream& file);
};

// URL Frames section

class URLFrame : public FrameEntity {
private:
    std::string url_link_;
public:
    URLFrame(std::ifstream& file);
};

class WXXXFrame : public FrameEntity {
private:
    unsigned char text_encode_;
    std::string description_;
    std::string url_;
public:
    WXXXFrame(std::ifstream& file);
};

// Time stamp frames section

class ECTOFrame : public FrameEntity {
private:
    std::vector<std::pair<unsigned char, unsigned char>> time_stamps_list_;
public:
    ECTOFrame(std::ifstream& file);
};

// Unsync lyrics copy

class USLTFrame : public FrameEntity {
private:
    unsigned char text_encode_;
    unsigned char language_[3];
    std::string description_;
    std::string lyrics_;
public:
    USLTFrame(std::ifstream& file);
};

// Sync lyrics copy

class SYLTFrame : public FrameEntity {
private:
    unsigned char encode_;
    unsigned char language_[3];
    unsigned char time_stamp_format_;
    unsigned char info_type_;
    std::string info_;
public:
    SYLTFrame(std::ifstream& file);
};

// Comments Frame

class COMMFrame : public FrameEntity {
private:
    unsigned char encode_;
    unsigned char language_[3];
    std::string short_description_;
    std::string factial_text_;
public:
    COMMFrame(std::ifstream& file);
};

// Relative Volume Adjustment frame

struct Chanel {
    unsigned char chanel_type_;
    unsigned char volume_adjustment_;
    unsigned char peak_bytes_;
    unsigned char peak_volume_;
};

class RVA2Frame : public FrameEntity {
private:
    std::string id_;
    std::vector<Chanel> chanels_info_;
};

// Equalisation Frame

class EQU2Frame : public FrameEntity {
private:
    unsigned char interpolation_method_;
    std::string id_;
};

// Plays counter

class PCNTFrame : public FrameEntity {
private:
    uint32_t counter_;
public:
    PCNTFrame(std::ifstream& file);
};

// Popularity measure

class POPMFrame : public FrameEntity {
private:
    std::string email_;
    unsigned char rating_;
    uint32_t counter_;
public:
    POPMFrame(std::ifstream& file);
};

// Recomended buffer size

class RBUFFrame : public FrameEntity {
private:
    unsigned char buffer_size_[3];
    bool included_info_flag_;
    unsigned char offset_to_next_frame_[4];
public:
    RBUFFrame(std::ifstream& file);
};

// Linked information

class LINKFrame : public FrameEntity {
private:
    unsigned char id_of_frame_[4];
    std::string url_;
    std::string additional_info_;
public:
    LINKFrame(std::ifstream& file);
};

// Sync position

class POSSFrame : public FrameEntity {
private:
    unsigned char timestamp_format_;
    unsigned char position_;
public:
    POSSFrame(std::ifstream& file) :
            FrameEntity(file) {
        timestamp_format_ = file.get();
        position_ = file.get();
    }

};

// User Agreement Frame

class USERFrame : public FrameEntity {
private:
    unsigned char text_encoding_;
    unsigned char language_[3];
    std::string factial_text_;
public:
    USERFrame(std::ifstream& file);
};

// Owner Frame

class OWNEFrame : public FrameEntity {
private:
    unsigned char text_encoding_;
    std::string paid_price_;
    std::string purchase_date_;
    std::string seller_;
public:
    OWNEFrame(std::ifstream& file);
};

// Commercial Frame

class COMRFrame : public FrameEntity {
private:
    unsigned char encoding_;
    std::string price_string_;
    std::string actual_to_;
    std::string contact_url_;
    unsigned char promote_as_;
    std::string seller_name_;
    std::string description_;
    std::string MIME_picture_type_;
    std::string seller_logo_;
};

// Encrypt registration method

class ENCRFrame : public FrameEntity {
private:
    std::string owner_id_;
    unsigned char method_symbol_;
    std::string encrypt_data_;
};

// Group ID Registration frame

class GRIDFrame : public FrameEntity {
private:
    std::string owner_id_;
    unsigned char group_symbol_;
    std::string group_data_;
};

// Private Frame

class PRIVFrame : public FrameEntity {
private:
    unsigned char owner_id;
    std::string private_data_;
};

class MP3Entity {
private:
    std::string file_path_;
    std::ifstream file_;
    Header* file_header_;
    std::multimap<std::string, std::any> frames_container_;
public:
    MP3Entity(const std::string& file_path);

};