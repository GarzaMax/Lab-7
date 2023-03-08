//
// Created by nivce on 07.02.2023.
//

#include "MP3Reader.h"

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

std::string Header::GetFileID() {
    return file_id_;
}

short Header::GetIDVersion() const {
    return id3v2_version_;
}

short Header::GetIDRevision() const {
    return id3v2_revision_;
}

bool Header::GetUnsyncFlag() const {
    return unsync_flag_;
}

bool Header::GetExtendedHeaderFlag() const {
    return extended_header_flag_;
}

bool Header::GetExperimentalFlag() const {
    return experimental_indicator_flag_;
}

bool Header::GetFooterFlag() const {
    return footer_flag_;
}

size_t Header::GetTagSize() const {
    return tag_size_;
}


void FrameEntity::SetFrameID(std::string frame_id) {
    frame_id_ = frame_id;
}

size_t FrameEntity::GetFrameSize() const {
    return frame_size_;
}

std::string FrameEntity::GetFrameID() {
    return frame_id_;
}

bool FrameEntity::GetTagSaveFlag() const {
    return tag_save_flag_;
}

bool FrameEntity::GetFileSaveFlag() const {
    return file_save_flag_;
}

bool FrameEntity::GetReadOnlyFlag() {
    return read_only_flag_;
}

bool FrameEntity::GetGroupIdentityFlag() const {
    return group_identity_flag_;
}

bool FrameEntity::GetCompressionFlag() const {
    return compression_flag_;
}

bool FrameEntity::GetEncryptFlag() const {
    return encrypt_flag_;
}

bool FrameEntity::GetUnsyncFlag() const {
    return unsync_flag_;
}

bool FrameEntity::GetDataSizeIdFlag() const {
    return data_size_indicator_flag_;
}

FrameEntity::FrameEntity(std::ifstream& file) {
    std::string tmp_size_container;
    for (int i = 0; i < 4; i++) tmp_size_container += file.get();
    frame_size_ = static_cast<uint8_t>(tmp_size_container[0]) << 24U |
                  static_cast<uint8_t>(tmp_size_container[1]) << 16U |
                  static_cast<uint8_t>(tmp_size_container[2]) << 8U |
                  static_cast<uint8_t>(tmp_size_container[3]);
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

RBUFFrame::RBUFFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    for (int i = 0; i < 3; i++) buffer_size_[i] = file.get();
    std::bitset<8> tmp_bitset(file.get());
    included_info_flag_ = tmp_bitset[7];
    for (int i = 0; i < 4; i++) offset_to_next_frame_[i];
}

POPMFrame::POPMFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    std::getline(file, email_);
    rating_ += file.get();
    std::string buffer;
    for (int i = 0; i < GetFrameSize() - email_.length() - 2; i++) buffer += file.get();
    counter_ = std::stoi(buffer);
}

PCNTFrame::PCNTFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    std::string buffer;
    for (int i = 0; i < GetFrameSize(); i++) buffer += file.get();
    counter_ = std::stoi(buffer);
}

COMMFrame::COMMFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    encode_ = file.get();
    for (int i = 0; i < 3; i++) language_[i] = file.get();
    std::getline(file, short_description_);
    for (int i = 0; i < GetFrameSize() - 4 - short_description_.length(); i++) factial_text_ += file.get();
}

SYLTFrame::SYLTFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    encode_ = file.get();
    for (int i = 0; i < 3; i++) language_[i] = file.get();
    time_stamp_format_ = file.get();
    info_type_ = file.get();
    std::getline(file, info_);
}

USLTFrame::USLTFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    text_encode_ = file.get();
    for (int i = 0; i < 3; i++) language_[i] = file.get();
    std::getline(file, description_);
    for (int i = 0; i < GetFrameSize() - 4 - description_.length(); i++) lyrics_ += file.get();
}

ECTOFrame::ECTOFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    for (int i = 0; i < GetFrameSize() / 2; i++)
        time_stamps_list_.emplace_back(file.get(), file.get());
}

WXXXFrame::WXXXFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    text_encode_ = file.get();
    std::getline(file, description_);
    for (int i = 0; i < GetFrameSize() - 2 - description_.length(); i++) url_ += file.get();
}

URLFrame::URLFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    for (int i = 0; i < GetFrameSize(); i++) url_link_ += file.get();
}

TXXXFrame::TXXXFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    text_encode_ = file.get();
    std::getline(file, info_string_);
    for (int i = 0; i < GetFrameSize() - 2 - info_string_.length(); i++) description_ += file.get();
}

TextFrame::TextFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    text_encode_ = file.get();
    for (int i = 0; i < GetFrameSize() - 1; i++) info_string_ += file.get();
}

MCDIFrame::MCDIFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    for (int i = 0; i < GetFrameSize(); i++) cd_header_.push_back(file.get());
}

UFIDFrame::UFIDFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    std::getline(file, owner_header_);
    std::string binary_data_container;
    for (int i = 0; i < GetFrameSize() - owner_header_.size(); i++) binary_data_container += file.get();
    std::bitset<64> tmp(binary_data_container);
    binary_data_ = tmp;
}

LINKFrame::LINKFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    for (int i = 0; i < 4; i++) id_of_frame_[i] = file.get();
    std::getline(file, url_);
    for (int i = 0; i < GetFrameSize() - 5 - url_.length(); i++) additional_info_ += file.get();
}

USERFrame::USERFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    text_encoding_ = file.get();
    for (int i = 0; i < 3; i++) language_[i] = file.get();
    for (int i = 0; i < GetFrameSize() - 4; i++) factial_text_ += file.get();
}

OWNEFrame::OWNEFrame(std::ifstream& file, std::string frame_name) :
        FrameEntity(file) {
    SetFrameID(frame_name);
    text_encoding_ = file.get();
    std::getline(file, paid_price_);
    std::getline(file, purchase_date_);
    for (int i = 0; i < GetFrameSize() - 3 - paid_price_.length() - purchase_date_.length(); i++) seller_ += file.get();
}

MP3Entity::MP3Entity(const std::string& file_path) :
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

    size_t tag_size = file_header_->GetTagSize();
    while (tag_size != 0) {
        std::string frame_name;
        for (int i = 0; i < 4; i++) frame_name += file_.get();
        if (frame_name[0] == '\000' && frame_name[1] == '\000' && frame_name[2] == '\000') {
            tag_size = 0;
        }
        if (RecogniseFrameType(frame_name) == "TextFrame") {
            std::any Frame((TextFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<TextFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "TXXXFrame") {
            std::any Frame((TXXXFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<TXXXFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "UFIDFrame") {
            std::any Frame((UFIDFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<UFIDFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "MCDIFrame") {
            std::any Frame((MCDIFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<MCDIFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "WXXXFrame") {
            std::any Frame((WXXXFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<WXXXFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "URLFrame") {
            std::any Frame((URLFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<URLFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "ECTOFrame") {
            std::any Frame((ECTOFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<ECTOFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "USLTFrame") {
            std::any Frame((USLTFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<USLTFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "SYLTFrame") {
            std::any Frame((SYLTFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<SYLTFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "COMMFrame") {
            std::any Frame((COMMFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<COMMFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "RVA2Frame") {
//            std::any Frame((RVA2Frame(file_, frame_name)));
//            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
//            tag_size -= std::any_cast<RVA2Frame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "EQU2Frame") {
//            std::any Frame((EQU2Frame(file_, frame_name)));
//            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
//            tag_size -= std::any_cast<EQU2Frame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "PCNTFrame") {
            std::any Frame((PCNTFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<PCNTFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "POPMFrame") {
            std::any Frame((POPMFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<POPMFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "RBUFFrame") {
            std::any Frame((RBUFFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<RBUFFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "LINKFrame") {
            std::any Frame((LINKFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<LINKFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "POSSFrame") {
            std::any Frame((POSSFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<POSSFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "USERFrame") {
            std::any Frame((USERFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<USERFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "OWNEFrame") {
            std::any Frame((OWNEFrame(file_, frame_name)));
            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
            tag_size -= std::any_cast<OWNEFrame>(Frame).GetFrameSize();
        } else if (RecogniseFrameType(frame_name) == "COMRFrame") {
//            std::any Frame((COMRFrame(file_, frame_name)));
//            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
        } else if (RecogniseFrameType(frame_name) == "ENCRFrame") {
//            std::any Frame((ENCRFrame(file_, frame_name)));
//            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
        } else if (RecogniseFrameType(frame_name) == "GRIDFrame") {
//            std::any Frame((GRIDFrame(file_, frame_name)));
//            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
        } else if (RecogniseFrameType(frame_name) == "PRIVFrame") {
//            std::any Frame((PRIVFrame(file_, frame_name)));
//            frames_container_.insert(std::pair<std::string, std::any>(frame_name, Frame));
        } else {
            FrameEntity Biba(file_);
            std::string tmp_buffer;
            for (int i = 0; i < Biba.GetFrameSize(); i++) tmp_buffer += file_.get();
            tag_size -= Biba.GetFrameSize();
        }
    }


}

