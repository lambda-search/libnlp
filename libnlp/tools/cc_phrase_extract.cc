
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/


#include <fstream>

#include "libnlp/tools/cmdline_output.h"
#include "libnlp/normalize/cc/phrase_extract.h"

using libnlp::Exception;
using libnlp::cc::phrase_extract;
using libnlp::UTF8StringSlice;

void Extract(const std::vector<std::string> &inputFiles,
             const std::string &outputFile) {
    std::ostringstream buffer;
    for (const auto &inputFile : inputFiles) {
        std::ifstream ifs(inputFile);
        const std::string contents((std::istreambuf_iterator<char>(ifs)),
                                   (std::istreambuf_iterator<char>()));
        buffer << contents;
    }
    const std::string &text = buffer.str();
    phrase_extract extractor;
    extractor.set_word_max_length(2);
    extractor.set_prefix_set_length(1);
    extractor.set_suffix_set_length(1);
    extractor.Extract(text);
    std::ofstream ofs(outputFile);
    for (const auto &word : extractor.words()) {
        const phrase_extract::phrase_signals &signals = extractor.signal(word);
        const double entropy = signals.prefixEntropy + signals.suffixEntropy;
        const double logProbablity = extractor.log_probability(word);
        ofs << word << " " << signals.frequency << " " << logProbablity << " "
            << signals.cohesion << " " << entropy << " " << signals.prefixEntropy
            << " " << signals.suffixEntropy << std::endl;
    }
    ofs.close();
}

int main(int argc, const char *argv[]) {
    try {
        TCLAP::CmdLine cmd("Open Chinese convert (nlpcc) Phrase Extractor", ' ',
                           VERSION);
        CmdLineOutput cmdLineOutput;
        cmd.setOutput(&cmdLineOutput);
        TCLAP::UnlabeledMultiArg<std::string> fileNames(
                "fileName", "Input files", true /* required */, "files");
        cmd.add(fileNames);
        TCLAP::ValueArg<std::string> outputArg(
                "o", "output", "Output file", true /* required */, "" /* default */,
                "file" /* type */, cmd);
        cmd.parse(argc, argv);
        Extract(fileNames.getValue(), outputArg.getValue());
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId()
                  << std::endl;
    } catch (Exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
