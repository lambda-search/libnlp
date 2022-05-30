
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/


#ifndef LIBNLP_NORMALIZE_OPTION_H_
#define LIBNLP_NORMALIZE_OPTION_H_

namespace libnlp::normalize {

    enum normalize_options : int32_t {
        DEFAULT = 0,                        // 仅为语义作用，表示选择默认选项
        FULL_WIDTH_TO_HALF = 0x1,           // 全角到半角（默认打开）
        UPPER_CASE_TO_LOWER = 0x2,          // 大写转小写（默认关闭）
        COMPLICATED_TO_SIMPLFIED = 0x4,     // 繁体转简体（默认打开）
        BLANK_TO_ONE_SPACE = 0x8,            // 连续空白字符转空格 + 开头结尾空格删除（默认打开）
        PUNCTUAT_TO_SPACE = 0x10,           // 标点转空格（默认关闭）
        REMOVE_BAD_CHAR = 0x20,             // 去除非法字符（默认打开）
        REMOVE_SOME_SPACE = 0x40,           // 去除中文与中文之间、中文与英文之间的空格（即只保留英文与英文间的空格）（默认关闭）
        PUNCTUAT_REMOVE = 0x80,             // 标点去除（默认关闭）
        ARABIC_TO_NUMBER = 0x100,           // 中文数字转英文（默认关闭）
        REMOVE_EMOJI = 0x200,               // 去除Emoji表情（默认关闭）
        REPLACE_SYNONYM = 0x400,            // 同义词归一化（默认关闭）
        TRIM_LEFT_RIGHT_SPACE = 0x800,
    };

}  // namespace libnlp::normalize

#endif // LIBNLP_NORMALIZE_OPTION_H_

