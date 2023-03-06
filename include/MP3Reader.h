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

    short GetIDVersion();

    short GetIDRevision();

    bool GetUnsyncFlag();

    bool GetExperimentalFlag();

    bool GetExtendedHeaderFlag();

    bool GetFooterFlag();

    bool GetTagSize();
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
    FrameEntity(std::ifstream& file) {
        for (int i = 0; i < 4; i++) frame_id_ += file.get();
        std::string tmp_size_container;
        for (int i = 0; i < 4; i++) tmp_size_container += file.get();
        frame_size_ = static_cast<uint8_t>(tmp_size_container[0]) << 24U |
                      static_cast<uint8_t>(tmp_size_container[1]) << 16U |
                      static_cast<uint8_t>(tmp_size_container[2]) << 8U |
                      static_cast<uint8_t>(tmp_size_container[3]);
        std::string tmp_flag_container;
        for (int i = 0; i < 2; i++) tmp_flag_container += file.get();
        std::bitset<8> frame_flags((int) file.get());
        tag_save_flag_ = frame_flags[6];
        file_save_flag_ = frame_flags[5];
        read_only_flag_ = frame_flags[4];
        frame_flags = (int) file.get();
        group_identity_flag_ = frame_flags[6];
        compression_flag_ = frame_flags[3];
        encrypt_flag_ = frame_flags[2];
        unsync_flag_ = frame_flags[1];
        data_size_indicator_flag_ = frame_flags[0];
    }

    ~FrameEntity() = default;

    std::string GetFrameID();

    size_t GetFrameSize();

    bool GetTagSaveFlag();

    bool GetFileSaveFlag();

    bool GetReadOnlyFlag();

    bool GetGroupIdentityFlag();

    bool GetCompressionFlag();

    bool GetEncryptFlag();

    bool GetUnsyncFlag();

    bool GetDataSizeIdFlag();

};

// UFID Frame definition

class UFIDFrame : public FrameEntity {
private:
    std::string owner_header_;
    std::bitset<64> binary_data_;
public:

    UFIDFrame(std::ifstream& file) :
            FrameEntity(file) {
        std::getline(file, owner_header_);
        std::string binary_data_container;
        for (int i = 0; i < GetFrameSize() - owner_header_.size(); i++) binary_data_container += file.get();
        std::bitset<64> tmp(binary_data_container);
        binary_data_ = tmp;
    }

    ~UFIDFrame() = default;

};

// Music CD ID definition

class MCDI : public FrameEntity {
private:
    std::string cd_header_;

};

// Text Frames section

class TextFrame : public FrameEntity {
private:
    wchar_t text_encode_;
    std::string info_string_;
};

class TXXXFrame : public TextFrame {
private:
    std::string description_;
};

// URL Frames section

class URLFrame : public FrameEntity {
private:
    std::string url_link_;
};

class WXXXFrame : public URLFrame {
private:
    wchar_t text_encode_;
    std::string description_;
};

// Time stamp frames section

class ECTOFrame : public FrameEntity {
private:
    std::vector<std::pair<wchar_t, wchar_t>> time_stamps_list_;
};

// Unsync lyrics copy

class USLTFrame : public FrameEntity {
private:
    wchar_t text_encode_;
    wchar_t language_[3];
    std::string description_;
    std::string lyrics_;
};

// Sync lyrics copy

class SYLTFrame : public FrameEntity {
private:
    wchar_t encode_;
    wchar_t language_[3];
    wchar_t time_stamp_format_;
    wchar_t info_type_;
    std::string info_;
};

// Comments Frame

class COMMFrame : public FrameEntity {
private:
    wchar_t encode_;
    wchar_t language_[3];
    std::string short_description_;
    std::string factial_text_;
};

// Relative Volume Adjustment frame

class Chanel {
private:
    wchar_t chanel_type_;
    wchar_t volume_adjustment_;
    wchar_t peak_bytes_;
    wchar_t peak_volume_;
};

class RVA2Frame : public FrameEntity {
private:
    std::string id_;
    std::vector<Chanel> chanels_info_;
};

// Equalisation Frame

class EQU2Frame : public FrameEntity {
private:
    wchar_t interpolation_method_;
    std::string id_;
};

// Plays counter

class PCNTFrame : public FrameEntity {
private:
    uint32_t counter_;
};

// Popularity measure

class POPMFrame : public FrameEntity {
private:
    std::string email_;
    wchar_t rating_;
    uint32_t counter_;
};

// Recomended buffer size

class RBUFFrame : public FrameEntity {
private:
    wchar_t buffer_size_[3];
    bool included_info_flag_;
    wchar_t offset_to_next_frame_[4];
};

// Linked information

class LINKFrame : public FrameEntity {
private:
    wchar_t id_of_frame_[4];
    std::string url_;
    std::string additional_info_;
};

// Sync position

class POSSFrame {
private:
    wchar_t timestamp_format_;
    wchar_t position_;
};

// User Agreement Frame

class USERFrame {
private:
    wchar_t text_encoding_;
    wchar_t language_[3];
    std::string factial_text_;
};

// Owner Frame

class OWNEFrame {
private:
    wchar_t text_encoding_;
    std::string paid_price_;
    std::string purchase_date_;
    std::string seller_;
};

// Commercial Frame

class COMRFrame : public FrameEntity {
private:
    wchar_t encoding_;
    std::string price_string_;
    std::string actual_to_;
    std::string contact_url_;
    wchar_t promote_as_;
    std::string seller_name_;
    std::string description_;
    std::string MIME_picture_type_;
    std::string seller_logo_;
};

// Encrypt registration method

class ENCRFrame : public FrameEntity {
private:
    std::string owner_id_;
    wchar_t method_symbol_;
    std::string encrypt_data_;
};

// Group ID Registration frame

class GRIDFrame : public FrameEntity {
private:
    std::string owner_id_;
    wchar_t group_symbol_;
    std::string group_data_;
};

// Private Frame

class PRIVFrame : public FrameEntity {
private:
    wchar_t owner_id;
    std::string private_data_;
};

class MP3Entity {
private:
    std::string file_path_;
    std::ifstream file_;
    Header* file_header_;
    std::map<std::string, std::any>;
public:
    MP3Entity(const std::string& file_path) :
            file_path_(file_path) {
        file_.open(file_path_);
        if (file_.is_open()) {
            std::cout << file_path_ << " opened successfully";
        } else {
            std::cout << "FileOpenError: " << file_path_ << " open corrupted";
        }

        file_header_ = new Header(file_);

        if (file_header_->GetExtendedHeaderFlag()) {
            std::string tmp_ehsize;
            for (int i = 0; i < 4; i++) tmp_ehsize += file_.get();
            size_t extended_header_size = static_cast<uint8_t>(tmp_ehsize[0]) << 24U |
                                          static_cast<uint8_t>(tmp_ehsize[1]) << 16U |
                                          static_cast<uint8_t>(tmp_ehsize[2]) << 8U |
                                          static_cast<uint8_t>(tmp_ehsize[3]);
            file_.seekg(extended_header_size - 4, std::ios::cur);
        }


    }

};