/*
 * Open Chinese convert
 *
 * Copyright 2010-2014 Carbo Kuo <byvoid@byvoid.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "libnlp/dict/dict_converter.h"
#include "cmdline_output.h"
#include "libnlp/base/exception.h"

using namespace libnlp::cc;

int main(int argc, const char *argv[]) {
    try {
        TCLAP::CmdLine cmd("Open Chinese convert (nlp_cc) Dictionary Tool", ' ',
                           VERSION);
        CmdLineOutput cmdLineOutput;
        cmd.setOutput(&cmdLineOutput);

        std::vector<std::string> dictFormats{"text", "ocd2", "ocd"};
        TCLAP::ValuesConstraint<std::string> allowedVals(dictFormats);

        TCLAP::ValueArg<std::string> toArg("t", "to", "Output format",
                                           true /* required */, "" /* default */,
                                           &allowedVals /* type */, cmd);
        TCLAP::ValueArg<std::string> fromArg("f", "from", "Input format",
                                             true /* required */, "" /* default */,
                                             &allowedVals /* type */, cmd);
        TCLAP::ValueArg<std::string> outputArg(
                "o", "output", "Path to output dictionary", true /* required */,
                "" /* default */, "file" /* type */, cmd);
        TCLAP::ValueArg<std::string> inputArg(
                "i", "input", "Path to input dictionary", true /* required */,
                "" /* default */, "file" /* type */, cmd);
        cmd.parse(argc, argv);
        libnlp::dict::convert_dictionary(inputArg.getValue(), outputArg.getValue(),
                                        fromArg.getValue(), toArg.getValue());
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId()
                  << std::endl;
    } catch (libnlp::Exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
