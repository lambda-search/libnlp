
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/


#include <fstream>

#include "cmdline_output.h"
#include "libnlp/normalize/cc/cc_config.h"
#include "libnlp/normalize/cc/converter.h"
#include "libnlp/base/utf8_util.h"

std::optional<std::string> inputFileName;
std::optional<std::string> outputFileName;
std::string configFileName;
bool noFlush;
libnlp::cc::cc_config config;
libnlp::cc::converter_ptr converter;

FILE *get_output_stream() {
    if (!outputFileName) {
        return stdout;
    } else {
        FILE *fp = fopen((*outputFileName).c_str(), "w");
        if (!fp) {
            throw libnlp::FileNotWritable((*outputFileName));
        }
        return fp;
    }
}

void ConvertLineByLine() {
    std::istream &inputStream = std::cin;
    FILE *fout = get_output_stream();
    bool isFirstLine = true;
    while (!inputStream.eof()) {
        if (!isFirstLine) {
            fputs("\n", fout);
        } else {
            isFirstLine = false;
        }
        std::string line;
        std::getline(inputStream, line);
        const std::string &converted = converter->convert(line);
        fputs(converted.c_str(), fout);
        if (!noFlush) {
            // Flush every line if the output stream is stdout.
            fflush(fout);
        }
    }
    fclose(fout);
}

void convert(std::string fileName) {
    const int BUFFER_SIZE = 1024 * 1024;
    static bool bufferInitialized = false;
    static std::string buffer;
    static char *bufferBegin;
    static const char *bufferEnd;
    static char *bufferPtr;
    static size_t bufferSizeAvailble;
    if (!bufferInitialized) {
        bufferInitialized = true;
        buffer.resize(BUFFER_SIZE + 1);
        bufferBegin = const_cast<char *>(buffer.c_str());
        bufferEnd = buffer.c_str() + BUFFER_SIZE;
        bufferPtr = bufferBegin;
        bufferSizeAvailble = BUFFER_SIZE;
    }

    bool needToRemove = false;
    if (outputFileName && fileName == *outputFileName) {
        // Special case: input == output
        std::ifstream src(fileName, std::ios::binary);
#ifdef _WIN32
        const std::string tempFileName = std::tmpnam(nullptr);
#else
        // std::tmpnam is deprecated
        std::string tempFileName;
        const char *tmpDirEnv = std::getenv("TMPDIR");
        if (tmpDirEnv != nullptr) {
            tempFileName = tmpDirEnv;
        }
#ifdef P_tmpdir
        if (tempFileName.empty()) {
            tempFileName = P_tmpdir;
        }
#endif
        if (tempFileName.empty()) {
            tempFileName = "/tmp";
        }
        tempFileName += "/libnlpXXXXXX";
        int fd = mkstemp(const_cast<char *>(tempFileName.c_str()));
        if (fd == 0) {
            throw libnlp::FileNotWritable(tempFileName);
        }
#endif
        std::ofstream dst(tempFileName, std::ios::binary);
        dst << src.rdbuf();
        dst.close();
        fileName = tempFileName;
        needToRemove = true;
    }

    FILE *fin = fopen(fileName.c_str(), "r");
    if (!fin) {
        throw libnlp::FileNotFound(fileName);
    }
    FILE *fout = get_output_stream();
    while (!feof(fin)) {
        size_t length = fread(bufferPtr, sizeof(char), bufferSizeAvailble, fin);
        bufferPtr[length] = '\0';
        size_t remainingLength = 0;
        std::string remainingTemp;
        if (length == bufferSizeAvailble) {
            // fread may breaks UTF8 character
            // Find the end of last character
            char *lastChPtr = bufferBegin;
            while (lastChPtr < bufferEnd) {
                size_t nextCharLen = libnlp::UTF8Util::NextCharLength(lastChPtr);
                if (lastChPtr + nextCharLen > bufferEnd) {
                    break;
                }
                lastChPtr += nextCharLen;
            }
            remainingLength = bufferEnd - lastChPtr;
            if (remainingLength > 0) {
                remainingTemp = libnlp::UTF8Util::FromSubstr(lastChPtr, remainingLength);
                *lastChPtr = '\0';
            }
        }
        // Perform conversion
        const std::string &converted = converter->convert(buffer);
        fputs(converted.c_str(), fout);
        if (!noFlush) {
            // Flush every line if the output stream is stdout.
            fflush(fout);
        }
        // Reset pointer
        bufferPtr = bufferBegin + remainingLength;
        bufferSizeAvailble = BUFFER_SIZE - remainingLength;
        if (remainingLength > 0) {
            strncpy(bufferBegin, remainingTemp.c_str(), remainingLength);
        }
    }
    fclose(fout);
    if (needToRemove) {
        // Remove temporary file.
        std::remove(fileName.c_str());
    }
}

int main(int argc, const char *argv[]) {
    try {
        TCLAP::CmdLine cmd("Open Chinese convert (nlpcc) Command Line Tool", ' ',
                           VERSION);
        CmdLineOutput cmdLineOutput;
        cmd.setOutput(&cmdLineOutput);

        TCLAP::ValueArg<std::string> configArg(
                "c", "config", "Configuration file", false /* required */,
                "s2t.json" /* default */, "file" /* type */, cmd);
        TCLAP::ValueArg<std::string> outputArg(
                "o", "output", "Write converted text to <file>.", false /* required */,
                "" /* default */, "file" /* type */, cmd);
        TCLAP::ValueArg<std::string> inputArg(
                "i", "input", "Read original text from <file>.", false /* required */,
                "" /* default */, "file" /* type */, cmd);
        TCLAP::ValueArg<bool> noFlushArg(
                "", "noflush", "Disable flush for every line", false /* required */,
                false /* default */, "bool" /* type */, cmd);
        cmd.parse(argc, argv);
        configFileName = configArg.getValue();
        noFlush = noFlushArg.getValue();
        if (inputArg.isSet()) {
            inputFileName = std::optional<std::string>(inputArg.getValue());
        }
        if (outputArg.isSet()) {
            outputFileName = std::optional<std::string>(outputArg.getValue());
            noFlush = true;
        }
        converter = config.create_from_file(configFileName);
        if (!inputFileName) {
            ConvertLineByLine();
        } else {
            convert(*inputFileName);
        }
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId()
                  << std::endl;
    } catch (libnlp::Exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
